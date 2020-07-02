/**
 *	nexus ngame - nspell
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NGAME_NSPELL_H_
#define _NGAME_NSPELL_H_

#include "ncommon.h"
#include "nspell_define.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

	class nunit;

	enum ESpellStatus
	{
		ESStatus_None					= 0,			// ��ʼ��״̬
		ESStatus_Preparing				= 1,			// ׼��״̬, (����, ����)
		ESStatus_Casting				= 2,			// �ͷ�״̬, (����, ����)
		ESStatus_Delayed				= 3,			// ��ʱ״̬, (����)
		ESStatus_Finished				= 4,			// �����ͷ����, ����ʱЧ���������
		ESStatus_Deleted				= 5,			// ��ɾ��״̬
		ESStatus_Max
	};

	class nspell : private nnoncopyable
	{
	public:
		nspell();
		virtual ~nspell();

		static nspell* construct(nunit* caster_ptr, nspell_info const* spell_info_ptr, uint64 target_id, const vector3& target_pos, float time_stamp);
		static void destroy(nspell* spell_ptr);

		bool init(nunit* caster_ptr, nspell_info const* spell_info_ptr, uint64 target_id, const vector3& target_pos, float time_stamp);

		void set_trigger_spell(uint64 original_caster_id, nspell_info const* spell_info_ptr);
		void set_trigger_aura(uint64 original_caster_id, nspell_aura_info const* aura_info_ptr);
		void set_caster_item_id(uint64 item_id)					{ m_caster_item_id = item_id; }
		void set_status(uint8 status)							{ m_status = status; }
		void set_cast_index(uint32 index)						{ m_cast_index = index; }

		uint8 get_status() const								{ return m_status; }
		nspell_info const* get_spell_info() const				{ return m_spell_info_ptr; }
		uint32 get_spell_id() const								{ return m_spell_info_ptr->id.name_crc; }
		uint32 get_interrupt_flags() const						{ return m_spell_info_ptr->interrupt_flags; }
		bool is_triggered() const								{ return m_trigger_spell_info_ptr || m_trigger_aura_info_ptr; }
		bool is_channeled_spell() const							{ return ESCast_Channel == m_spell_info_ptr->cast_type; }

		void prepare();
		void cancel(uint8 result);
		void update(uint32 elapse);
		void cast();
		void finish();

		void on_damage(uint64 unit_id, int32 value);

	private:
		uint8 _can_cast_spell(bool strict = true);
		uint8 _check_weapon();
		uint8 _check_target();
		uint8 _check_cost();
		uint8 _check_effect();

		void _set_orient();
		void _set_cast_timer();
		void _set_channel_timer();
		void _set_category_cd();
		void _set_spell_cd();

		void _fill_targets(uint32 effect_mask);
		void _fill_no_trigger_targets(uint32 index);
		void _add_unit_target(nunit* target_ptr, uint32 index);
		void _add_delay_unit_target(nunit* target_ptr, uint32 index, int32 delay);

		bool _is_alive_unit_targets();
		bool _is_need_send_to_client();

		void _take_cost();
		void _take_reagents();
		void _take_cast_item();

		void _send_cast_result(uint8 result);
		void _send_spell_prepare(int32 max_time, int32 cur_time);
		void _send_spell_cast();
		void _send_spell_channel(int32 max_time, int32 cur_time);
		void _send_interrupted(uint8 result);
		void _send_spell_hit(nunit* target_ptr, uint8 hit_info, int32 value, int32 absorb);

	private:
		// buffЧ��������
		void _handle_null(uint32 index) {}
		void _handle_trigger_spell(uint32 index);
		void _handle_apply_aura(uint32 index);
		void _handle_instant_kill(uint32 index);
		void _handle_melee_damage(uint32 index);
		void _handle_kungfu_damage(uint32 index);
		void _handle_spell_damage(uint32 index);
		void _handle_heal(uint32 index);
		void _handle_mod_power(uint32 index);
		void _handle_heal_pct(uint32 index);
		void _handle_mod_power_pct(uint32 index);
		void _handle_dispel(uint32 index);
		void _handle_resurrect(uint32 index);
		void _handle_knock_down(uint32 index);
		void _handle_knock_back(uint32 index);

	private:
		typedef void(nspell::*nspell_effect_handler)(uint32 index);
		static nspell_effect_handler	m_spell_effect_handler[ESET_Max];

	private:
		struct ntarget_info
		{
			nunit*					target_ptr;
			uint32					effect_mask;
			uint8					hit_info;

			ntarget_info() : target_ptr(NULL), effect_mask(0), hit_info(0) {}
		};

		struct ndelay_target_info
		{
			uint64					target_id;
			uint32					effect_mask;
			int32					delay_time;

			ndelay_target_info() : target_id(0), effect_mask(0), delay_time(0) {}
		};

		typedef std::list<ntarget_info> ntarget_list;
		typedef std::list<ndelay_target_info> ndelay_target_list;
		typedef std::list<uint64> nchannel_target_list;

		void _trigger_effect(uint32 trigger_type);
		void _cast_effect();
		void _do_effect_on_unit(uint32 index, ntarget_info* target_ptr);
		void _do_effect_on_delay_unit(uint32 index, ndelay_target_info* target_ptr);
		void _update_delay_target(uint32 elapse);

	private:
		nunit*						m_caster_ptr;				// �ͷ�unit
		uint64						m_original_caster_id;		// trigger���ܵ�ԭʼ�ͷ���id
		uint64						m_caster_item_id;			// ��Ʒ
		uint32						m_cast_index;				// ��ǰ�ͷŵĴ�������
		uint8						m_status;					// ����״̬
		int32						m_periodic;					// ����tick����
		int32						m_delay_time;				// ����ʱ��, ����ʱ��, ����ʱ���
		int32						m_timer;					// ��ʱ

		uint64						m_target_id;				// client������Ϣ
		vector3						m_target_pos;				// client������Ϣ
		float						m_time_stamp;				// client������Ϣ
		bool						m_client_time;				// �Ƿ�ʹ�ÿͻ���ʱ���
		
		nspell_info const*			m_spell_info_ptr;			// ����ģ��
		nspell_info const*			m_trigger_spell_info_ptr;	// trigger�ü��ܵļ���ģ��
		nspell_aura_info const*		m_trigger_aura_info_ptr;	// trigger�ü��ܵ�buffģ��

		nspell_effect_info const*	m_effect_info_ptr[MAX_EFFECT_NUM];	// ����Ч��
		bool						m_triggered[MAX_EFFECT_NUM];		// ����Ч���Ƿ񴥷�

		ntarget_list				m_targets;					// �������ö���
		ndelay_target_list			m_delay_targets;			// ������ʱ���ö���
		nchannel_target_list		m_channel_targets;			// ��������
		// item
		// game_object

		nunit*						m_unit_target;				// Ч���������е�Ӧ�ö���
		uint8						m_hit_info;					// ������Ϣ
		int32						m_damage;					// �˺���ֵ
		int32						m_heal;						// ������ֵ
		bool						m_trigger_hit;				// ���з���
		bool						m_trigger_crit;				// ��������
		bool						m_trigger_dead;				// ��������
		// item
		// game_object
	};

} // namespace nexus

#endif // _NGAME_NSPELL_H_
