#include "nspell.h"
#include "nunit.h"
#include "nplayer.h"
#include "nmap.h"
#include "nutil.h"
#include "nlog_mt.h"
#include "ninfo_data.h"
#include "nregion.h"
#include "character_controller/nmovement_define.h"
#include "timer/ntimestamp.h"

#include "nevent_args.h"

#include "nmsg_spell.h"

namespace nexus {

	nspell::nspell() : m_caster_ptr(NULL), m_original_caster_id(0), m_caster_item_id(0), m_cast_index(0), m_status(ESStatus_None), 
		m_periodic(0), m_delay_time(0), m_timer(0), m_target_id(0), m_time_stamp(0.0f), m_client_time(false), m_spell_info_ptr(NULL), 
		m_trigger_spell_info_ptr(NULL), m_trigger_aura_info_ptr(NULL), m_unit_target(NULL), m_hit_info(0), m_damage(0), m_heal(0), 
		m_trigger_hit(false), m_trigger_crit(false), m_trigger_dead(false)
	{
		memset(m_effect_info_ptr, 0, sizeof(m_effect_info_ptr));
		memset(m_triggered, 0, sizeof(m_triggered));
	}

	nspell::~nspell()
	{
	}

	nspell* nspell::construct(nunit* caster_ptr, nspell_info const* spell_info_ptr, uint64 target_id, const vector3& target_pos, float time_stamp)
	{
		nspell* spell_ptr = new nspell;
		spell_ptr->init(caster_ptr, spell_info_ptr, target_id, target_pos, time_stamp);
		return spell_ptr;
	}

	void nspell::destroy(nspell* spell_ptr)
	{
		delete spell_ptr;
	}

	bool nspell::init(nunit* caster_ptr, nspell_info const* spell_info_ptr, uint64 target_id, const vector3& target_pos, float time_stamp)
	{
		if (!caster_ptr || !spell_info_ptr)
		{
			NLOG_DEBUG(_T("nspell init param error!"));
			return false;
		}

		m_caster_ptr			= caster_ptr;
		m_spell_info_ptr		= spell_info_ptr;
		m_target_id				= target_id;
		m_target_pos			= target_pos;
		m_time_stamp			= time_stamp;

		// ��ʼ������Ч��ָ��
		for (uint32 i = 0; i < MAX_EFFECT_NUM; ++i)
		{
			if (0 != m_spell_info_ptr->spell_effect_id[i].name_crc)
			{
				m_effect_info_ptr[i] = s_spell_effect_info.get_info(m_spell_info_ptr->spell_effect_id[i].name_crc);
#ifdef _DEBUG
				if (!m_effect_info_ptr[i])
				{
					NLOG_DEBUG(_T("s_spell_effect_info.get_info error, id = %s"), m_spell_info_ptr->spell_effect_id[i].name_str.c_str());
				}
#endif
			}
		}

		// test 
		uint32 effect_id = 512;
		nplayer* mod_owner_ptr = m_caster_ptr->get_spell_mod_owner();

		if (mod_owner_ptr)
		{
			mod_owner_ptr->apply_spell_modifier(m_spell_info_ptr, 0, ESMT_SpellModEffect, effect_id);
		}

		// ���ݼ���ģ���ʼ������״̬����
		return true;
	}

	void nspell::set_trigger_spell(uint64 original_caster_id, nspell_info const* spell_info_ptr)
	{
		m_original_caster_id		= original_caster_id;
		m_trigger_spell_info_ptr	= spell_info_ptr;	
	}

	void nspell::set_trigger_aura(uint64 original_caster_id, nspell_aura_info const* aura_info_ptr)
	{
		m_original_caster_id		= original_caster_id;
		m_trigger_aura_info_ptr		= aura_info_ptr;	
	}

	void nspell::prepare()
	{
		if (ESStatus_None != m_status)
		{
			NLOG_DEBUG(_T("nspell prepare status error!"));
			return;
		}

		// �����⼼���Ƿ���������, �Ƿ����, ���뵽unit�����ͷŵ�spell����
		m_caster_ptr->add_cur_spell(this);

		// ���ڷǴ�������, �ж��������ͷŵ�spell֮���Ƿ��г�ͻ(����ִ����һ������)
		if (!is_triggered() && m_caster_ptr->is_spell_in_progress())
		{
			_send_cast_result(ESCR_Failed_SpellInProgress);
			return;
		}

		// ��⼼���Ƿ����ͷ�
		uint8 result = _can_cast_spell();

		if (ESCR_Success != result)
		{
			_send_cast_result(result);
			return;
	}

		// ��ʼ��׼��״̬
		m_status		= ESStatus_Preparing;
		m_delay_time	= 0;
		m_timer			= 0;

		if (!is_triggered())
	{
			// ��������
			_set_orient();

			m_client_time = true;

			// ���ͼ��ܿ�ʼʹ���¼�, ��ϼ���buff�ȵ�
			nevent_spell_prepare_args event_args;
			event_args.spell_id = get_spell_id();
			m_caster_ptr->get_event_processor().send_event(&event_args, sizeof(event_args));
		}

		// �ж�׼���׶εļ���Ч���Ƿ񴥷�
		_trigger_effect(ESETT_SpellPrepare);
		_set_cast_timer();
		_set_category_cd();

		if (m_timer > 0 && ESCast_Cast == m_spell_info_ptr->cast_type)
		{
			_send_spell_prepare(m_delay_time, m_timer);
	}
		else
		{
			cast();
		}
	}

	void nspell::cancel(uint8 result)
	{
		if (ESStatus_Preparing == m_status || ESStatus_Casting == m_status)
		{
			_send_interrupted(result);

			// ����Ǳ�����ϴ���Ч��
			if (ESCR_Failed_Cancel != result)
		{
				_trigger_effect(ESETT_SpellInterrupt);
		}

		finish();
	}
	}

	void nspell::update(uint32 elapse)
	{
		if (m_delay_time > 0)
		{
			m_timer -= elapse;

			switch (m_status)
			{
			case ESStatus_Preparing:
				{
					if (m_timer <= 0)
					{
						cast();
					}
				}
				break;
			case ESStatus_Casting:
				{
					if (m_timer <= 0 || !_is_alive_unit_targets())
					{
						finish();
					}

					// ������������Ƿ���Ҫ��������״̬, ͬ����������仯, �����������buff��ɾ����buff���
				}
				break;
			case ESStatus_Delayed:
				{
					if (m_timer <= 0)
					{
						_cast_effect();
						finish();
					}
				}
				break;
			default:
				{
				}
				break;
			}
		}

		_update_delay_target(elapse);

		// �ж��Ƿ����ɾ���ö���
		if (ESStatus_Finished == m_status && m_delay_targets.empty())
		{
			m_status = ESStatus_Deleted;
	}
	}

	void nspell::cast()
	{
		if (ESStatus_Preparing != m_status)
		{
			NLOG_DEBUG(_T("nspell cast status error!"));
			return;
		}

		m_status = ESStatus_Casting;
		m_delay_time	= 0;
		m_timer			= 0;

		// �����������ж��Ƿ���������
		if (ESCast_Cast == m_spell_info_ptr->cast_type)
		{
			uint8 result = _can_cast_spell();

			if (ESCR_Success != result)
			{
				_send_cast_result(result);
				return;
			}
		}

		if (!is_triggered())
		{
			// ���ͼ����ͷ��¼�, ��ϼ���buff�ȵ�
			nevent_spell_cast_args event_args;
			event_args.spell_id = get_spell_id();
			m_caster_ptr->get_event_processor().send_event(&event_args, sizeof(event_args));
		}

		// �۳���������
		_take_cost();
		// �۳����������ͷŵ�����Ʒ
		_take_cast_item();
		// ���ü���cd
		_set_spell_cd();

		// ���ͼ����ͷ���Ϣ, ������ʽ���ڴ˷���, ����������Ҫ����id
		_send_spell_cast();

		if (m_spell_info_ptr->speed > 0.0f)
		{
			// ������ʱʱ��
			float dist = 0.0f;

			if (0 != m_target_id)
			{
				nmap* map_ptr = m_caster_ptr->get_map();

				if (map_ptr)
				{
					nunit* unit_ptr = map_ptr->get_unit(m_target_id);

					if (unit_ptr)
					{
						dist = m_caster_ptr->get_distance(unit_ptr);
					}
				}
			}
			else if (ECTT_Point == m_spell_info_ptr->cast_target_type)
			{
				dist = m_caster_ptr->get_distance(m_target_pos);
			}

			// �츳��Ӱ��
			m_delay_time = int32(dist / m_spell_info_ptr->speed * 1000.0f);
			m_timer = m_delay_time;
			m_status = ESStatus_Delayed;
	}
		else
		{
			_cast_effect();

			if (0 == m_delay_time)
			{
				finish();
			}
		}
	}

	void nspell::finish()
	{
		if (ESStatus_Deleted == m_status || ESStatus_Finished == m_status)
		{
			NLOG_DEBUG(_T("nspell finish status error!"));
			return;
		}

		// ɾ���������ܸ���buff
		if (is_channeled_spell())
		{
			nmap* map_ptr = m_caster_ptr->get_map();

			if (map_ptr)
			{
				nunit* unit_ptr = NULL;

				for (nchannel_target_list::const_iterator iter = m_channel_targets.begin(); iter != m_channel_targets.end(); ++iter)
				{
					unit_ptr = (m_caster_ptr->get_id() == (*iter)) ? m_caster_ptr : map_ptr->get_unit(*iter);

					if (unit_ptr)
					{
						unit_ptr->remove_auras_by_caster_spell(get_spell_id(), m_caster_ptr->get_id());
					}
				}
			}

			_send_spell_channel(m_delay_time, 0);
	}

		// �޸�״̬
		m_status = ESStatus_Finished;
		m_delay_time	= 0;
		m_timer			= 0;
	}

	void nspell::on_damage(uint64 unit_id, int32 value)
	{
		if (get_interrupt_flags() & ESIF_Damage)
		{
			cancel(ESCR_Failed_Interrupted);
			return;
		}

		if (get_interrupt_flags() & ESIF_Damage2)
		{
			// С���
			m_timer += m_delay_time / 4;

			if (m_timer > m_delay_time)
			{
				m_timer = m_delay_time;
			}

			// ����???
			return;
		}
	}

	uint8 nspell::_can_cast_spell(bool strict)
	{
		// �ж��ͷ����Ƿ�������״̬
		if (m_caster_ptr->has_flag(EUS_Dead))
		{
			return ESCR_Failed_CasterDead;
		}

		// �ж��Ƿ���ս��״̬
		if (!m_spell_info_ptr->can_cast_in_combat && m_caster_ptr->has_flag(EUS_InCombat))
		{
			return ESCR_Failed_InCombat;
		}

		// �ж��Ƿ�־�
		if (!m_spell_info_ptr->can_cast_while_feared && m_caster_ptr->has_aura_effect(EAET_Fear))
		{
			return ESCR_Failed_InFear;
		}

		if (!m_spell_info_ptr->can_cast_while_silence && m_caster_ptr->has_aura_effect(EAET_Silence))
		{
			return ESCR_Failed_InSilence;
		}

		if (!m_spell_info_ptr->can_cast_while_root && m_caster_ptr->has_aura_effect(EAET_Root))
		{
			return ESCR_Failed_InRoot;
		}

		if (!m_spell_info_ptr->can_cast_while_move)
		{
			if (vec_length_sq(m_caster_ptr->get_acceleration()) > 0.0f || gameframework::EMove_Jump == m_caster_ptr->get_movement_type())
			{
				return ESCR_Failed_Move;
			}
		}

		uint8 result = 0;
		result = _check_weapon();

		if (ESCR_Success != result)
		{
			return result;
		}

		result = _check_target();

		if (ESCR_Success != result)
		{
			return result;
		}

		result = _check_cost();

		if (ESCR_Success != result)
		{
			return result;
		}

		result = _check_effect();

		if (ESCR_Success != result)
		{
			return result;
		}

		return ESCR_Success;
	}

	uint8 nspell::_check_weapon()
	{
		// ���������ж�װ������Ʒ�������ƻ��Ǳ�������Ʒ����
		switch (m_spell_info_ptr->weapon_type)
		{
		case ESWT_MainHand:						// ���ֱ���������Ʒ��������
			{

			}
			break;
		case ESWT_OffHand:						// ���ֱ���������Ʒ��������
			{

			}
			break;
		case ESWT_MainAndOff:					// �����ֶ�������Ʒ��������
			{

			}
			break;
		case ESWT_MainOrOff:					// ����������һ��������Ʒ��������
			{

			}
			break;
		default:								// ����
			{

			}
			break;
		}

		return ESCR_Success;
	}

	uint8 nspell::_check_target()
	{

		nmap* map_ptr = m_caster_ptr->get_map();

		if (!map_ptr)
		{
			return ESCR_Failed_InvalidTargets;
		}

		nunit* unit_ptr = NULL;
		vector3		pos;
		// ���������ж��ͷ�Ŀ���Ƿ�����
		switch (m_spell_info_ptr->cast_target_type)
		{
		case ECTT_Self:							// �Լ�
			{
				if (m_target_id != m_caster_ptr->get_id())
				{
					return ESCR_Failed_InvalidTargets;
				}

				pos = m_caster_ptr->get_position();
			}
			break;
		case ECTT_SingleEnemy:					// ������ķ����Ƶ�λ
			{
				unit_ptr = map_ptr->get_unit(m_target_id);

				if (!unit_ptr)
				{
					return ESCR_Failed_InvalidTargets;
			}

				// �ж��Ƿ�����
				if (unit_ptr->has_flag(EUS_Dead))
				{
					return ESCR_Failed_InvalidTargets;
				}

				// �ж����ƹ�ϵ

				pos = unit_ptr->get_position();
			}
			break;
		case ECTT_SingleFriend:					// ����������Ƶ�λ
			{
				unit_ptr = (m_caster_ptr->get_id() == m_target_id) ? m_caster_ptr : map_ptr->get_unit(m_target_id);

				if (!unit_ptr)
				{
					return ESCR_Failed_InvalidTargets;
			}

				// �ж��Ƿ�����
				if (unit_ptr->has_flag(EUS_Dead))
				{
					return ESCR_Failed_InvalidTargets;
				}

				pos = unit_ptr->get_position();
			}
			break;
		case ECTT_EnemyCorpse:					// ����������ʬ��
			{
				unit_ptr = map_ptr->get_unit(m_target_id);

				if (!unit_ptr)
				{
					return ESCR_Failed_InvalidTargets;
			}

				// �ж��Ƿ�����
				if (!unit_ptr->has_flag(EUS_Dead))
				{
					return ESCR_Failed_InvalidTargets;
				}

				pos = unit_ptr->get_position();
			}
			break;
		case ECTT_FriendCorpse:					// ��������ʬ��
			{
				unit_ptr = map_ptr->get_unit(m_target_id);

				if (!unit_ptr)
				{
					return ESCR_Failed_InvalidTargets;
			}

				// �ж��Ƿ�����
				if (!unit_ptr->has_flag(EUS_Dead))
				{
					return ESCR_Failed_InvalidTargets;
				}

				pos = unit_ptr->get_position();
			}
			break;
		case ECTT_Point:						// �����
			{
				// ��������
				navigation_map::ptr nav_map_ptr = map_ptr->get_nav_map();

				if (!nav_map_ptr)
				{
					return ESCR_Failed_InvalidTargets;
			}

				m_target_pos.y = nav_map_ptr->get_world_height(m_target_pos.x, m_target_pos.z);
				pos = m_target_pos;
			}
			break;
		default:								// ����
			{
				return ESCR_Success;
			}
			break;
		}

		// ͳһ�жϾ���
		float dist = vec_distance(m_caster_ptr->get_position(), pos);

		if (dist < m_spell_info_ptr->min_range)
		{
			return ESCR_Failed_TooClose;
		}

		if (dist > m_spell_info_ptr->max_range)
		{
			return ESCR_Failed_OutofRange;
		}

		// no_line_of_sight�ж��Ƿ��������ڵ�
		if (!m_spell_info_ptr->no_line_of_sight && m_caster_ptr->line_of_sight(pos))
		{
			return ESCR_Failed_LineofSight;
		}

		return ESCR_Success;
	}

	uint8 nspell::_check_cost()
	{
		// ���������жϵ�ǰ�����Ƿ�����
		switch (m_spell_info_ptr->cost_type)
		{
		case ESCT_Null:							// ��Ч, ������
			{
				return ESCR_Success;
			}
			break;
		case ESCT_EnergyValue:					// ��ֵ��������
			{
				if (m_caster_ptr->get_data().get_uint32_value(EUF_UnitEnergy) < m_spell_info_ptr->cost_value)
				{
					return ESCR_Failed_NoEnergy;
			}
			}
			break;
		case ESCT_ManaValue:					// ��ֵ������ʶ
			{
				if (m_caster_ptr->get_data().get_uint32_value(EUF_UnitMana) < m_spell_info_ptr->cost_value)
				{
					return ESCR_Failed_NoMana;
			}
			}
			break;
		case ESCT_ManaPct:						// �����ֵ�İٷֱ�������ʶ
			{

			}
			break;
		case ESCT_ManaPerLevel:					// ÿһ�����ĵ���ʶֵ
			{

			}
			break;
		case ESCT_EnergyPerLevel:				// ÿһ�����ĵ�����ֵ
			{

			}
			break;
		case ESCT_ManaAndItem:					// ��ʶֵ����Ʒ
			{

			}
			break;
		case ESCT_EnergyAndItem:				// ����ֵ����Ʒ
			{

			}
			break;
		case ESCT_Item:							// ��Ʒ
			{

			}
			break;
		default:								// ����
			{

			}
			break;
		}

		return ESCR_Success;
	}

	uint8 nspell::_check_effect()
	{
		// ĳЩЧ�����ж��Ƿ��ܳɹ�, ��������Ʒ��
		return ESCR_Success;
	}

	void nspell::_set_orient()
	{
		if (!m_spell_info_ptr->auto_orient_target)
		{
			return;
		}

		if (ECTT_Point == m_spell_info_ptr->cast_target_type)
		{
			m_caster_ptr->set_orient_to(m_target_pos);
			return;
		}

		if (m_target_id == m_caster_ptr->get_id())
		{
			return;
		}

		nmap* map_ptr = m_caster_ptr->get_map();

		if (!map_ptr)
		{
			return;
		}

		nunit* target_ptr = map_ptr->get_unit(m_target_id);

		if (!target_ptr)
		{
			return;
		}

		m_caster_ptr->set_orient_to(target_ptr->get_position());
	}

	void nspell::_set_cast_timer()
	{
		int32 cast_time = 0;

		if (ESCast_Cast == m_spell_info_ptr->cast_type)
		{
			cast_time = int32(m_spell_info_ptr->casting_time * 1000);
		}

		// �����츳��ʱ���Ӱ��
		if (cast_time > 0)
			{

		}

		m_delay_time = cast_time;
		m_timer = m_delay_time;
	}

	void nspell::_set_channel_timer()
				{
		int32 periodic_time = 0;
		int32 tick_count = 0;

		if (ESCast_Channel == m_spell_info_ptr->cast_type)
					{
			periodic_time = int32(m_spell_info_ptr->periodic_time * 1000);
			tick_count = m_spell_info_ptr->max_tick_count;
		}

		// �����츳��ʱ���Ӱ��, Ҫ��֤ͨ�����ܵ�tick����



		m_delay_time = periodic_time * tick_count;
		m_timer = m_delay_time;
		m_periodic = periodic_time;

		if (m_channel_targets.empty())
		{
			m_timer = 1;
	}
	}

	void nspell::_set_category_cd()
	{
		if (EOT_PLAYER != m_caster_ptr->get_type())
		{
			return;
		}

		// �����츳��cd��Ӱ��
		nplayer* player_ptr = (nplayer*)m_caster_ptr;

		if (m_spell_info_ptr->global_cd_time > 0.0f)
		{
			time_t expire_time = time(NULL) + int32(m_spell_info_ptr->global_cd_time);
			player_ptr->add_category_cd(ESCCDT_GlobalCD, expire_time);
		}
	}

	void nspell::_set_spell_cd()
	{
		if (EOT_PLAYER != m_caster_ptr->get_type())
		{
			return;
		}

		// �����츳��cd��Ӱ��
		nplayer* player_ptr = (nplayer*)m_caster_ptr;

		if (m_spell_info_ptr->cd_time > 0.0f)
		{
			time_t expire_time = time(NULL) + int32(m_spell_info_ptr->cd_time);
			player_ptr->add_spell_cd(get_spell_id(), expire_time);
		}
	}

	void nspell::_fill_targets(uint32 effect_mask)
	{
		// ��ղ���ʱ����
		m_targets.clear();

		for (uint32 i = 0; i < MAX_EFFECT_NUM; ++i)
		{
			if (effect_mask & (1 << i))
			{
				switch (m_effect_info_ptr[i]->trigger_target_type)
				{
				case ESTTT_Targets:
					{
						_fill_no_trigger_targets(i);
					}
					break;
				case ESTTT_Caster:
					{
						_add_unit_target(m_caster_ptr, i);
					}
					break;
				case ESTTT_CasterParty:
					{

					}
					break;
				default:
					{
					}
					break;
				}
			}
		}
	}

	void nspell::_fill_no_trigger_targets(uint32 index)
	{
		nmap* map_ptr = m_caster_ptr->get_map();

		if (!map_ptr)
		{
			return;
		}

		nunit* unit_ptr = NULL;

		switch (m_spell_info_ptr->cast_target_type)
		{
		case ECTT_Null:
			{
				std::list<nworld_object*> objs;

				switch (m_spell_info_ptr->effect_target_type)
				{
				case EETT_AllEnemyAroundCaster:
					{
						ncircle_region region;
						region.set_param(m_target_pos, m_spell_info_ptr->radius);
						map_ptr->get_objects(m_caster_ptr, &region, objs, EOT_PLAYER | EOT_CREATURE);

						for (std::list<nworld_object*>::iterator iter = objs.begin(); iter != objs.end(); ++iter)
						{
							// �ж��Ƿ�����
							unit_ptr = (nunit*)(*iter);

							if (unit_ptr->has_flag(EUS_Dead))
							{
								continue;
							}

							// �ж����ƹ�ϵ
							_add_unit_target(unit_ptr, index);
						}
					}
					break;
				default:
					{
					}
					break;
				}
			}
			break;
		case ECTT_Self:
			{
				_add_unit_target(m_caster_ptr, index);
			}
			break;
		case ECTT_SingleEnemy:
		case ECTT_SingleFriend:
		case ECTT_EnemyCorpse:
		case ECTT_FriendCorpse:
			{
					nunit* unit_ptr = map_ptr->get_unit(m_target_id);

					if (unit_ptr)
					{
						_add_unit_target(unit_ptr, index);
			}
				}
			break;
		case ECTT_Point:
			{
				std::list<nworld_object*> objs;

				switch (m_spell_info_ptr->effect_target_type)
				{
				case EETT_AllEnemyInArea1:
					{
						ncircle_region region;
						region.set_param(m_target_pos, m_spell_info_ptr->radius);
						map_ptr->get_objects(m_caster_ptr, &region, objs, EOT_PLAYER | EOT_CREATURE);

						for (std::list<nworld_object*>::iterator iter = objs.begin(); iter != objs.end(); ++iter)
						{
							// �ж��Ƿ�����
							unit_ptr = (nunit*)(*iter);

							if (unit_ptr->has_flag(EUS_Dead))
							{
								continue;
							}

							// �ж����ƹ�ϵ
							_add_unit_target(unit_ptr, index);
						}
					}
					break;
				case EETT_AllEnemyInArea2:
					{
						nfan_shaped_region region;
						region.set_param(m_caster_ptr->get_position(), m_spell_info_ptr->radius, m_caster_ptr->get_rotation().y, m_spell_info_ptr->angle);
						map_ptr->get_objects(m_caster_ptr, &region, objs, EOT_PLAYER | EOT_CREATURE);

						for (std::list<nworld_object*>::iterator iter = objs.begin(); iter != objs.end(); ++iter)
						{
							// �ж��Ƿ�����
							unit_ptr = (nunit*)(*iter);

							if (unit_ptr->has_flag(EUS_Dead))
							{
								continue;
					}

							// �ж����ƹ�ϵ
							_add_unit_target(unit_ptr, index);
						}
					}
					break;
				default:
					{
					}
					break;
				}
			}
			break;
		default:
			{
			}
			break;
		}
	}

	void nspell::_add_unit_target(nunit* target_ptr, uint32 index)
	{
		for (ntarget_list::iterator iter = m_targets.begin(); iter != m_targets.end(); ++iter)
		{
			if (target_ptr == iter->target_ptr)
			{
				iter->effect_mask |= (1 << index);
				return;
			}
		}

		ntarget_info info;
		info.target_ptr		= target_ptr;
		info.effect_mask	= (1 << index);
		info.hit_info		= ESHR_Hit;

		// �����trigger���ܵĻ�������
		if (ESETT_SpellCasting == m_effect_info_ptr[index]->effect_trigger_type && !is_triggered())
		{
			info.hit_info = m_caster_ptr->spell_hit_result(target_ptr, m_spell_info_ptr);
		}

		m_targets.push_back(info);
	}

	void nspell::_add_delay_unit_target(nunit* target_ptr, uint32 index, int32 delay)
	{
		uint64 guid = target_ptr->get_id();

		for (ndelay_target_list::iterator iter = m_delay_targets.begin(); iter != m_delay_targets.end(); ++iter)
		{
			if (guid == iter->target_id)
			{
				iter->effect_mask |= (1 << index);
				return;
			}
		}

		ndelay_target_info info;
		info.target_id		= guid;
		info.effect_mask	= (1 << index);
		info.delay_time		= delay;

		m_delay_targets.push_back(info);
	}

	bool nspell::_is_alive_unit_targets()
	{
		return true;
	}

	bool nspell::_is_need_send_to_client()
	{
		return true;
	}

	void nspell::_take_cost()
	{
		if (is_triggered())
		{
			return;
		}

		float cost_value = m_spell_info_ptr->cost_value;
		// �����츳Ӱ��

		switch (m_spell_info_ptr->cost_type)
		{
		case ESCT_EnergyValue:
			{
				m_caster_ptr->energy_modifier(-(int32)cost_value);
			}
			break;
		case ESCT_ManaValue:
			{
				m_caster_ptr->mana_modifier(-(int32)cost_value);
			}
			break;
		case ESCT_EnergyPct:
			{
				uint32 max_energy = m_caster_ptr->get_data().get_uint32_value(EUF_UnitMaxEnergy);
				cost_value = float(max_energy) * cost_value;
				m_caster_ptr->energy_modifier(-(int32)cost_value);
			}
			break;
		case ESCT_ManaPct:
			{
				uint32 max_mana = m_caster_ptr->get_data().get_uint32_value(EUF_UnitMaxMana);
				cost_value = float(max_mana) * cost_value;
				m_caster_ptr->mana_modifier(-(int32)cost_value);
			}
			break;
		case ESCT_ManaPerLevel:
			{
			}
			break;
		case ESCT_EnergyPerLevel:
			{
			}
			break;
		case ESCT_ManaAndItem:
			{
				m_caster_ptr->mana_modifier(-(int32)cost_value);
				_take_reagents();
			}
			break;
		case ESCT_EnergyAndItem:
			{
				m_caster_ptr->energy_modifier(-(int32)cost_value);
				_take_reagents();
			}
			break;
		case ESCT_Item:
			{
				_take_reagents();
			}
			break;
		default:
			{
			}
			break;
		}
	}

	void nspell::_take_reagents()
	{
		
	}

	void nspell::_take_cast_item()
	{
		// m_cast_item_id ���������Ʒ�Ļ�, ������Ʒ
	}

	void nspell::_send_cast_result(uint8 result)
	{
		tagS2C_CastSpellResult reply;
		reply.spell_id		= get_spell_id();
		reply.result		= result;

		if (EOT_PLAYER == m_caster_ptr->get_type() && m_caster_ptr->get_map())
		{
			((nplayer*)m_caster_ptr)->send_message(&reply, sizeof(reply));
	}
	}

	void nspell::_send_spell_prepare(int32 max_time, int32 cur_time)
	{
		tagS2C_SpellPrepare reply;
		reply.caster_id		= m_caster_ptr->get_id();
		reply.spell_id		= get_spell_id();
		reply.time_stamp	= m_client_time ? m_time_stamp : nexus::get_time();
		reply.max_time		= max_time;
		reply.cur_time		= cur_time;
		m_caster_ptr->send_message_to_aoi(&reply, sizeof(reply));
	}

	void nspell::_send_spell_cast()
	{
		if (!_is_need_send_to_client() || is_channeled_spell())
		{
			return;
	}

		tagS2C_SpellCasting reply;
		reply.caster_id		= m_caster_ptr->get_id();
		reply.spell_id		= get_spell_id();
		reply.time_stamp	= m_client_time ? m_time_stamp : nexus::get_time();
		reply.max_time		= 0;
		reply.cur_time		= 0;
		reply.target_id		= m_target_id;
		reply.target_pos	= m_target_pos;
		m_caster_ptr->send_message_to_aoi(&reply, sizeof(reply));
	}

	void nspell::_send_spell_channel(int32 max_time, int32 cur_time)
	{
		tagS2C_SpellCasting reply;
		reply.caster_id		= m_caster_ptr->get_id();
		reply.spell_id		= get_spell_id();
		reply.time_stamp	= m_client_time ? m_time_stamp : nexus::get_time();
		reply.max_time		= max_time;
		reply.cur_time		= cur_time;

		// ������Ϣ��ʼ��target, �����Ƕ��target��ģʽ, �ҵ�ǰ������ʱ�����target
		// 
		reply.target_id		= m_target_id;
		reply.target_pos	= m_target_pos;

		m_caster_ptr->send_message_to_aoi(&reply, sizeof(reply));
	}

	void nspell::_send_interrupted(uint8 result)
	{
		tagS2C_SpellFailed reply;
		reply.caster_id		= m_caster_ptr->get_id();
		reply.spell_id		= get_spell_id();
		reply.error			= result;
		m_caster_ptr->send_message_to_aoi(&reply, sizeof(reply));
	}

	void nspell::_send_spell_hit(nunit* target_ptr, uint8 hit_info, int32 value, int32 absorb)
	{
		tagS2C_SpellHit	reply;
		reply.caster_id		= m_caster_ptr->get_id();
		reply.target_id		= target_ptr->get_id();
		reply.info_id		= get_spell_id();
		reply.time_stamp	= m_client_time ? m_time_stamp : nexus::get_time();
		reply.value			= value;
		reply.absorb		= absorb;
		reply.flag			= hit_info;
		reply.type			= ESHT_Spell;
		target_ptr->send_message_to_aoi(&reply, sizeof(reply));
	}

	void nspell::_trigger_effect(uint32 trigger_type)
	{
		uint32 effect_mask = 0;

		for(uint32 i = 0; i < MAX_EFFECT_NUM; ++i)
		{
			if (m_effect_info_ptr[i] && !m_triggered[i] && trigger_type == m_effect_info_ptr[i]->effect_trigger_type)
			{
				if (m_effect_info_ptr[i]->rate < 100.0f && !s_util.roll_chance_f(m_effect_info_ptr[i]->rate))
				{
						continue;
					}

				m_triggered[i] = true;
				effect_mask |= (1 << i);
					}
				}

		if (0 == effect_mask)
						{
			return;
		}

			_fill_targets(effect_mask);

		for (ntarget_list::iterator iter = m_targets.begin(); iter != m_targets.end(); ++iter)
		{
			_do_effect_on_unit(effect_mask, &(*iter));
		}
	}

	void nspell::_cast_effect()
	{
		uint32 effect_mask = 0;

		for (uint32 i = 0; i < MAX_EFFECT_NUM; ++i)
		{
			if (m_effect_info_ptr[i] && !m_triggered[i] && ESETT_SpellCasting == m_effect_info_ptr[i]->effect_trigger_type)
			{
				if (m_effect_info_ptr[i]->rate < 100.0f && !s_util.roll_chance_f(m_effect_info_ptr[i]->rate))
				{
					continue;
				}

				m_triggered[i] = true;
				effect_mask |= (1 << i);
			}
		}

		if (0 == effect_mask)
		{
			NLOG_DEBUG(_T("should have one more effect on cast, id = %s"), m_spell_info_ptr->id.name_str.c_str());
			return;
		}

		_fill_targets(effect_mask);

			// �ж��Ƿ�Ϊ��������
		if (is_channeled_spell())
		{
			for (ntarget_list::iterator iter = m_targets.begin(); iter != m_targets.end(); ++iter)
			{
				if (!((ESHR_Miss & iter->hit_info) || (ESHR_Immune & iter->hit_info) || (ESHR_Reflect & iter->hit_info) || (ESHR_Evade & iter->hit_info)))
				{
					m_channel_targets.push_back(iter->target_ptr->get_id());
				}
			}

				_set_channel_timer();
				_send_spell_channel(m_delay_time, m_timer);
					}

		m_trigger_hit	= false;
		m_trigger_crit	= false;
		m_trigger_dead	= false;

		for (ntarget_list::iterator iter = m_targets.begin(); iter != m_targets.end(); ++iter)
			{
				_do_effect_on_unit(effect_mask, &(*iter));
		}

		// m_targets�Ѿ���Ч, trigger�л��޸�
		if (m_trigger_hit)
		{
			_trigger_effect(ESETT_HitTarget);
	}

		if (m_trigger_crit)
		{
			_trigger_effect(ESETT_Crit);
		}

		if (m_trigger_dead)
		{
			_trigger_effect(ESETT_EffectDead);
		}
	}

	void nspell::_do_effect_on_unit(uint32 mask, ntarget_info* target_info_ptr)
		{
		if (!target_info_ptr)
		{
			return;
		}

		// ����guid���͸�ֵָ��, ָ�����ⲿУ���
		m_unit_target = target_info_ptr->target_ptr;
		m_hit_info = target_info_ptr->hit_info;
		m_damage		= 0;
		m_heal			= 0;
		// ����������Ϣ, ע�����ս���ͳ��
		if ((ESHR_Miss & m_hit_info) || (ESHR_Immune & m_hit_info) || (ESHR_Reflect & m_hit_info))
		{
			m_caster_ptr->set_incombat_with(m_unit_target);
			m_unit_target->set_incombat_with(m_caster_ptr);
			_send_spell_hit(m_unit_target, m_hit_info, 0, 0);
			return;
		}

		if (ESHR_Evade & m_hit_info)
		{
			_send_spell_hit(m_unit_target, m_hit_info, 0, 0);
			return;
		}

		// ���յ�ֻ���˺�, buff֮��Ķ���û������

		uint32 effect_index = 0;

		for (uint32 i = 0; i < MAX_EFFECT_NUM; ++i)
			{
			if (mask & (1 << i))
		{
				effect_index = m_effect_info_ptr[i]->effect_type;

				if (effect_index < ESET_Max)
				{
					(this->*(m_spell_effect_handler[effect_index]))(i);
				}
				else
				{
					NLOG_DEBUG(_T("spell effect type error, effect_type = %u"), effect_index);
				}
	}
		}

		// �����˺�, �����˺���Ϣ
		if (m_damage > 0 || ESHR_Absorb & m_hit_info)
		{
			int32 absorb = 0;
			int32 real_damage = 0;
			real_damage = m_unit_target->do_damage(m_caster_ptr, m_damage);
			_send_spell_hit(m_unit_target, m_hit_info, -m_damage, absorb);

			// ��������Ч��
			if (real_damage < m_damage)
			{
				// ��Ӧ���ڸ�Ŀ����, Ŀ���Ѿ�����, ���罱���Լ��ָ��ȵ�
				m_trigger_dead = true;
			}
		}

		if (m_heal > 0)
		{
			int32 real_heal = 0;
			real_heal = m_unit_target->do_heal(m_caster_ptr, m_heal);
			_send_spell_hit(m_unit_target, m_hit_info, m_heal, 0);
		}

		// ���д���Ч��
		m_trigger_hit = true;

		// ��������Ч��
		if (ESHR_Crit & m_hit_info)
		{
			m_trigger_crit	= true;
	}
	}

	void nspell::_do_effect_on_delay_unit(uint32 mask, ndelay_target_info* target_info_ptr)
	{
		if (!target_info_ptr)
		{
			return;
		}

				// У��ָ��
						nmap* map_ptr = m_caster_ptr->get_map();

				if (!map_ptr)
						{
			return;
							}

		nunit* unit_ptr = map_ptr->get_unit(target_info_ptr->target_id);

				if (!unit_ptr)
					{
			return;
					}

		m_unit_target	= unit_ptr;
		m_hit_info		= m_caster_ptr->spell_hit_result(m_unit_target, m_spell_info_ptr);
		m_damage		= 0;
		m_heal			= 0;
		// ����������Ϣ, ע�����ս���ͳ��
		if ((ESHR_Miss & m_hit_info) || (ESHR_Immune & m_hit_info) || (ESHR_Reflect & m_hit_info))
		{
			m_caster_ptr->set_incombat_with(m_unit_target);
			m_unit_target->set_incombat_with(m_caster_ptr);
			_send_spell_hit(m_unit_target, m_hit_info, 0, 0);
			return;
		}

		if (ESHR_Evade & m_hit_info)
		{
			_send_spell_hit(m_unit_target, m_hit_info, 0, 0);
			return;
			}

		// ���յ�ֻ���˺�, buff֮��Ķ���û������
		bool no_triggered = true;
		uint32 effect_index = 0;

		for (uint32 i = 0; i < MAX_EFFECT_NUM; ++i)
		{
			if (mask & (1 << i))
			{
				if (ESETT_SpellCasting != m_effect_info_ptr[i]->effect_trigger_type)
				{
					no_triggered = false;
			}

				effect_index = m_effect_info_ptr[i]->effect_type;

				if (effect_index < ESET_Max)
				{
					(this->*(m_spell_effect_handler[effect_index]))(i);
		}
				else
				{
					NLOG_DEBUG(_T("spell effect type error, effect_type = %u"), effect_index);
				}
			}
		}

		// �����˺�, �����˺���Ϣ
		if (m_damage > 0 || ESHR_Absorb & m_hit_info)
	{
			int32 absorb = 0;
			int32 real_damage = 0;
			real_damage = m_unit_target->do_damage(m_caster_ptr, m_damage);
			_send_spell_hit(m_unit_target, m_hit_info, -m_damage, absorb);

			// ��������Ч��
			if (no_triggered && real_damage < m_damage)
		{
				// ��Ӧ���ڸ�Ŀ����, Ŀ���Ѿ�����, ���罱���Լ��ָ��ȵ�
				_trigger_effect(ESETT_EffectDead);
			}
		}

		if (m_heal > 0)
			{
			int32 real_heal = 0;
			real_heal = m_unit_target->do_heal(m_caster_ptr, m_heal);
			_send_spell_hit(m_unit_target, m_hit_info, m_heal, 0);
			}

		if (no_triggered)
		{
			// ���д���Ч��
			_trigger_effect(ESETT_HitTarget);

			// ��������Ч��
			if (ESHR_Crit & m_hit_info)
			{
				_trigger_effect(ESETT_Crit);
			}
		}
	}

	void nspell::_update_delay_target(uint32 elapse)
	{
		for (ndelay_target_list::iterator iter = m_delay_targets.begin(); iter != m_delay_targets.end(); )
		{
			iter->delay_time -= elapse;

			if (iter->delay_time <= 0)
			{
				_do_effect_on_delay_unit(iter->effect_mask, &(*iter));
				iter = m_delay_targets.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

} // namespace nexus
