#include "nspell_aura.h"
#include "nunit.h"
#include "nplayer.h"
#include "nmap.h"
#include "nlog_mt.h"

#include "nmsg_spell.h"

namespace nexus {

	naura::naura_effect_handler naura::m_aura_effect_handler[EAET_Max] =
	{
		&naura::_handle_null,										// 0,	EAET_Null,					��Ч��
		&naura::_handle_mod_stat_base_value,						// 1,	EAET_ModStatBaseValue,		base_value��ʽ�޸�����ֵ
		&naura::_handle_mod_stat_base_pct,							// 2,	EAET_ModStatBasePct,		base_pct��ʽ�޸�����ֵ
		&naura::_handle_mod_stat_total_value,						// 3,	EAET_ModStatTotalValue,		total_value��ʽ�޸�����ֵ
		&naura::_handle_mod_stat_total_pct,							// 4,	EAET_ModStatTotalPct,		total_pct��ʽ�޸�����ֵ
		&naura::_handle_periodic_damage,							// 5,	EAET_PeriodicDamage,		�����˺�
		&naura::_handle_periodic_heal,								// 6,	EAET_PeriodicHeal,			��������
		&naura::_handle_fear,										// 7,	EAET_Fear,					�־�
		&naura::_handle_silence,									// 8,	EAET_Silence,				��Ĭ
		&naura::_handle_increase_speed,								// 9,	EAET_IncreaseSpeed,			����
		&naura::_handle_decrease_speed,								// 10,	EAET_DecreaseSpeed,			����
		&naura::_handle_root,										// 11,	EAET_Root,					����
		&naura::_handle_spell_mod,									// 12,	EAET_SpellMod,				�츳�޸ļ���
		&naura::_handle_trigger_spell,								// 13,	EAET_TriggerSpell,			��������
	};

	void naura::_handle_mod_stat_base_value(bool apply)
	{
		for (uint32 i = 0; i < MAX_AURA_STAT_NUM; ++i)
		{
			if (EUF_UnitNull == m_spell_aura_info_ptr->stat_index[i])
			{
				continue;
			}

			if (m_spell_aura_info_ptr->stat_index[i] >= EUF_UnitSeparator)
			{
				NLOG_DEBUG(_T("aura info error mod base value, spell info id = %u, arua info id = %u, index = %u, value = %f"), 
					m_spell_info_ptr->id, m_spell_aura_info_ptr->id, m_spell_aura_info_ptr->stat_index[i], m_spell_aura_info_ptr->stat_value[i]);
				continue;
			}

			m_owner_ptr->handle_stat_modifier(EUMT_BaseValue, m_spell_aura_info_ptr->stat_index[i], m_spell_aura_info_ptr->stat_value[i], apply);
		}
	}

	void naura::_handle_mod_stat_base_pct(bool apply)
	{
		for (uint32 i = 0; i < MAX_AURA_STAT_NUM; ++i)
		{
			if (EUF_UnitNull == m_spell_aura_info_ptr->stat_index[i])
			{
				continue;
			}

			if (m_spell_aura_info_ptr->stat_index[i] >= EUF_UnitSeparator)
			{
				NLOG_DEBUG(_T("aura info error mod base pct, spell info id = %u, arua info id = %u, index = %u, value = %f"), 
					m_spell_info_ptr->id, m_spell_aura_info_ptr->id, m_spell_aura_info_ptr->stat_index[i], m_spell_aura_info_ptr->stat_value[i]);
				continue;
			}

			m_owner_ptr->handle_stat_modifier(EUMT_BasePct, m_spell_aura_info_ptr->stat_index[i], m_spell_aura_info_ptr->stat_value[i], apply);
		}
	}

	void naura::_handle_mod_stat_total_value(bool apply)
	{
		for (uint32 i = 0; i < MAX_AURA_STAT_NUM; ++i)
		{
			if (EUF_UnitNull == m_spell_aura_info_ptr->stat_index[i])
			{
				continue;
			}

			if (m_spell_aura_info_ptr->stat_index[i] >= EUF_UnitSeparator)
			{
				NLOG_DEBUG(_T("aura info error mod total value, spell info id = %u, arua info id = %u, index = %u, value = %f"), 
					m_spell_info_ptr->id, m_spell_aura_info_ptr->id, m_spell_aura_info_ptr->stat_index[i], m_spell_aura_info_ptr->stat_value[i]);
				continue;
			}

			m_owner_ptr->handle_stat_modifier(EUMT_TotalValue, m_spell_aura_info_ptr->stat_index[i], m_spell_aura_info_ptr->stat_value[i], apply);
		}
	}

	void naura::_handle_mod_stat_total_pct(bool apply)
	{
		for (uint32 i = 0; i < MAX_AURA_STAT_NUM; ++i)
		{
			if (EUF_UnitNull == m_spell_aura_info_ptr->stat_index[i])
			{
				continue;
			}

			if (m_spell_aura_info_ptr->stat_index[i] >= EUF_UnitSeparator)
			{
				NLOG_DEBUG(_T("aura info error mod total pct, spell info id = %u, arua info id = %u, index = %u, value = %f"), 
					m_spell_info_ptr->id, m_spell_aura_info_ptr->id, m_spell_aura_info_ptr->stat_index[i], m_spell_aura_info_ptr->stat_value[i]);
				continue;
			}

			m_owner_ptr->handle_stat_modifier(EUMT_TotalPct, m_spell_aura_info_ptr->stat_index[i], m_spell_aura_info_ptr->stat_value[i], apply);
		}
	}

	void naura::_handle_periodic_damage(bool apply)
	{
		// �����˺���ֵ
		nunit* caster_ptr = NULL;
		nmap* map_ptr = m_owner_ptr->get_map();

		if (map_ptr)
		{
			caster_ptr = map_ptr->get_unit(m_caster_id);
		}

		int32 damage = int32(m_spell_aura_info_ptr->value1);

		NLOG_DEBUG(_T("aura damage, value = -%d"), damage);
		m_owner_ptr->do_damage(caster_ptr, damage);

		// �������յȵ�

		tagS2C_SpellHit	reply;
		reply.caster_id		= m_caster_id;
		reply.target_id		= m_owner_ptr->get_id();
		reply.info_id		= get_info_id();
		reply.time_stamp	= 0;
		reply.value			= -damage;
		reply.absorb		= 0;
		reply.flag			= ESHR_Hit;
		reply.type			= ESHT_Aura;
		m_owner_ptr->send_message_to_aoi(&reply, sizeof(reply));
	}

	void naura::_handle_periodic_heal(bool apply)
	{
		// ����������ֵ
		nunit* caster_ptr = NULL;
		nmap* map_ptr = m_owner_ptr->get_map();

		if (map_ptr)
		{
			caster_ptr = map_ptr->get_unit(m_caster_id);
	}

		int32 heal = int32(m_spell_aura_info_ptr->value1);

		NLOG_DEBUG(_T("aura damage, value = +%d"), heal);
		m_owner_ptr->do_heal(caster_ptr, heal);

		tagS2C_SpellHit	reply;
		reply.caster_id		= m_caster_id;
		reply.target_id		= m_owner_ptr->get_id();
		reply.info_id		= get_info_id();
		reply.time_stamp	= 0;
		reply.value			= heal;
		reply.absorb		= 0;
		reply.flag			= ESHR_Hit;
		reply.type			= ESHT_Aura;
		m_owner_ptr->send_message_to_aoi(&reply, sizeof(reply));
	}

	void naura::_handle_fear(bool apply)
	{

	}

	void naura::_handle_silence(bool apply)
	{

	}

	void naura::_handle_increase_speed(bool apply)
	{

	}

	void naura::_handle_decrease_speed(bool apply)
	{

	}

	void naura::_handle_root(bool apply)
	{

	}

	void naura::_handle_spell_mod(bool apply)
	{
		if (EOT_PLAYER != m_owner_ptr->get_type())
		{
			return;
		}

		if (m_spell_aura_info_ptr->spell_mod_type >= ESMT_Max)
		{
			NLOG_DEBUG(_T("spell_mod_type >= ESMT_Max, id = %s"), m_spell_aura_info_ptr->id.name_str.c_str());
			return;
	}

		((nplayer*)m_owner_ptr)->handle_spell_modifier(m_spell_aura_info_ptr, apply);
	}

	void naura::_handle_trigger_spell(bool apply)
	{

	}

} // namespace nexus
