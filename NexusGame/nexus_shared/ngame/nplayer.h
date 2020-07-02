/**
 *	nexus ngame - nplayer
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NPLAYER_H_
#define _NGAME_NPLAYER_H_

#include "ncommon.h"
#include "nclient_session.h"
#include "nunit.h"
#include "nspell_define.h"

namespace nexus {

	class nclient_session;

	struct tagMsg_D2W_LoadPlayerStat;
	struct tagMsg_D2W_LoadPlayerItem;
	struct tagMsg_D2W_LoadPlayerSpell;
	struct tagMsg_D2W_LoadPlayerAura;
	struct tagMsg_D2W_LoadPlayerCoolDown;

	class nplayer : public nunit
	{
	public:
		typedef boost::unordered_set<uint32> nplayer_spell_set;
		typedef std::map<uint32, time_t> nspell_cd_map;
		typedef std::multimap<uint32, nspell_aura_info const*> nspell_mod_map;
		typedef std::pair<nspell_mod_map::const_iterator, nspell_mod_map::const_iterator> nspell_mod_range_pair;

	public:
		nplayer();
		virtual ~nplayer();

		bool init(nclient_session* session_ptr, uint64 guid);
		virtual void destroy();
		virtual void update(uint32 elapse);

		virtual uint16 check_move(bool only_turn);

		nclient_session* get_session() const { return m_session_ptr; }
		bool send_message(void* msg_ptr, uint32 size)	{ return m_session_ptr->send_message(msg_ptr, size); }
		bool send_message_to_db(void* msg_ptr, uint32 size)	{ return m_session_ptr->send_message_to_db(msg_ptr, size); }

		virtual void make_enter_msg(void* data_ptr, uint32& size) const;

		// loading from db
		void request_load_data_from_db();
		void load_stat_from_db(const tagMsg_D2W_LoadPlayerStat* data_ptr);
		void load_item_from_db(const tagMsg_D2W_LoadPlayerItem* data_ptr);
		void load_spell_from_db(const tagMsg_D2W_LoadPlayerSpell* data_ptr);
		void load_aura_from_db(const tagMsg_D2W_LoadPlayerAura* data_ptr);
		void load_cool_down_from_db(const tagMsg_D2W_LoadPlayerCoolDown* data_ptr);

		// client loading
		void send_init_stat();
		void send_init_item();
		void send_init_spell();
		void send_init_aura();
		void send_init_cool_down();

		// TODO: D. Zhao ....

		// saving
		void save_data_to_db();

		bool is_stat_ok() const;
		bool is_load_ok() const { return 0 == m_load_flag; }
		void set_load_flag(uint32 flag);

		// teleport
		bool is_teleporting() const						{ return m_is_teleporting; }
		void set_teleporting(bool teleporting)			{ m_is_teleporting = teleporting; }
		const vector3& get_teleport_dest_pos() const	{ return m_teleport_dest_pos; }
		float get_teleport_dest_yaw() const				{ return m_teleport_dest_yaw; }
		uint32 get_teleport_dest_map_id() const			{ return m_teleport_dest_map_id; }
		uint32 get_teleport_dest_instance_id() const	{ return m_teleport_dest_instance_id; }
		bool teleport_to(float x, float y, float z, float yaw, uint32 map_id, uint32 instance_id = 0);

		// event
		virtual bool register_event();
		virtual void on_move(const nevent_base_args* base_args);
		virtual void on_dead(const nevent_base_args* base_args);


		uint8 get_class() const							{ return 0; }

		// spell
		bool has_spell(uint32 spell_id) const			{ return m_spells.find(spell_id) != m_spells.end(); }
		bool has_spell_cd(uint32 spell_id) const;
		void add_spell_cd(uint32 spell_id, time_t expire_time);
		void add_category_cd(uint32 category, time_t expire_time);
		void remove_spell_cd(uint32 spell_id, bool update = false);
		void remove_category_cd(uint32 category, bool update = false);

		void cast_generic_spell(uint32 spell_id, uint64 target_id, const vector3& target_pos, float time_stamp, uint32 cast_count);
		void cast_item_spell(uint32 spell_id, uint64 target_id, const vector3& target_pos, float time_stamp, uint64 item_guid);

		void cancel_spell();

		void handle_spell_modifier(nspell_aura_info const* mod_ptr, bool apply);
		template <typename T>
		T apply_spell_modifier(nspell_info const* spell_ptr, uint32 index, uint32 mod_type, T& value);

	protected:
		virtual void _update_base_stats(uint32 stat_index);

		virtual void _update_max_health();
		virtual void _update_max_energy();
		virtual void _update_max_mana();
		virtual void _update_melee_damage();
		virtual void _update_kungfu_damage();
		virtual void _update_spell_damage();
		virtual void _update_agility();
		virtual void _update_health_regeneration();
		virtual void _update_power_Regeneration();

		virtual void _regenerate_health(uint32 elapse);
		virtual void _regenerate_energy(uint32 elapse);
		virtual void _regenerate_mana(uint32 elapse);

	private:
		void _on_load_data_completed();

	private:
		void _update_repeat_spell(uint32 elapse);
		void _clear_repeat_spell();
		void _repeat_spell_interrupt(uint32 flag);
		void _resurrect(uint32 elapse);
		bool _is_affected_by_spell_mod(nspell_info const* spell_ptr, nspell_aura_info const* mod_ptr);

	private:
		nclient_session*			m_session_ptr;								// ��Ӧ������Ựָ��
		uint32						m_load_flag;								// ����loading��־λ

		// ��ǰҪ����Ŀ�ĵ���Ϣ
		bool						m_is_teleporting;							// �Ƿ��ڴ�����
		vector3						m_teleport_dest_pos;						// ����Ŀ�������
		float						m_teleport_dest_yaw;						// ����Ŀ��㳯��
		uint32						m_teleport_dest_map_id;						// ����Ŀ���ͼid
		uint32						m_teleport_dest_instance_id;				// ����Ŀ�긱��ʵ��id

		float						m_nude_stats[EUS_StatEnd];					// ������ʱ�洢

		// ��������
		nplayer_spell_set			m_spells;									// ���ӵ�еļ���id
		nspell_cd_map				m_category_cd;								// ��Ҹ������͵�cd
		nspell_cd_map				m_spell_cd;									// ��Ҹ������ܵ�cd
		uint32						m_casted_spell_id;							// �ϴ������ͷŵļ���
		uint64						m_target_id;								// �ϴ������ͷŵ�Ŀ��id
		vector3						m_target_pos;								// �ϴ������ͷŵ�Ŀ������
		float						m_time_stamp;								// �ϴ������ͷŵ�ʱ���
		uint32						m_cast_count;								// �������
		uint32						m_cast_index;								// ��ǰ�ͷ�����
		int32						m_min_repeat_time;							// ��С�����ͷ�ʱ��
		int32						m_max_repeat_time;							// ��������ͷ�ʱ��
		int32						m_periodic;									// �´��ͷ�ʣ��ʱ��

		nspell_mod_map				m_spell_mods;								// �츳

		// �����ø���
		int32						m_resurrect;								// ����ʣ��ʱ��
	};

	template <typename T>
	T nplayer::apply_spell_modifier(nspell_info const* spell_ptr, uint32 index, uint32 mod_type, T& value)
	{
		if (!spell_ptr)
		{
			return T(0);
		}

		float total_value = 0;
		float total_pct = 0;
		uint32 cur_level = 0;
		T new_value = T(0);
		bool flag1 = false;
		bool flag2 = false;
		nspell_aura_info const* mod_ptr = NULL;

		nspell_mod_range_pair rang_pair = m_spell_mods.equal_range(mod_type);

		for (nspell_mod_map::const_iterator iter = rang_pair.first; iter != rang_pair.second; ++iter)
		{
			mod_ptr = iter->second;
			// �жϸ��츳�Ƿ�Ӱ��ü���
			if (!_is_affected_by_spell_mod(spell_ptr, mod_ptr))
			{
				continue;
			}

			// mod_ptr->index == index
			switch (mod_ptr->spell_mod_sub_type)
			{
			case ESMST_Value:
				{
					total_value += mod_ptr->spell_mod_value1;
					flag1 = true;
				}
				break;
			case ESMST_Pct:
				{
					total_pct += mod_ptr->spell_mod_value1;
					flag1 = true;
				}
				break;
			case ESMST_Replace:
				{
					if (mod_ptr->level > cur_level)
					{
						new_value = mod_ptr->spell_mod_value2.name_crc;
						cur_level = mod_ptr->level;
						flag2 = true;
					}
				}
				break;
			default:
				{
				}
				break;
			}
		}

		if (flag1)
		{
			float diff = (float)value * total_pct + total_value;
			value = T((float)value + diff);
			return T(diff);
		}

		if (flag2)
		{
			value = new_value;
			return T(1);
		}

		return T(0);
	}

} // namespace nexus

#endif // _NGAME_NPLAYER_H_
