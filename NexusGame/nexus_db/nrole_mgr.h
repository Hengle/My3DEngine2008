/**
 *	nexus db - nrole_mgr
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NROLE_MGR_H_
#define _NDB_NROLE_MGR_H_

#include "ntime_regular.h"
#include "nrole_info.h"

namespace nexus{

	/**
	 *	nrole_mgr
	 */
	class nrole_mgr : public ntime_regular
	{
	public:
		nrole_mgr(void);
		~nrole_mgr(void);

		virtual void update(uint32 elapse);

		void unload_cache(void);

		// ��ɫ���߼�����Ϸ����
		bool role_online(uint64 role_id, nlan_session* session);

		// ��ɫ�����Ƴ���Ϸ����
		bool role_offline(uint64 role_id);

		// ������ɫ
		void create_role(ncharacter_cache* data_ptr);

		// ɾ����ɫ
		void delete_role(uint64 role_id);

		nrole_info* get_role_info(uint64 role_id);

		// ����ɫ�Ƿ�����
		bool is_online(uint64 role_id);

		RoleInfoMap& get_role_map(void){ return m_role_infoes; }

		ncharacter_cache* get_character_cache(uint64 role_id);

		//��ɫ�ȴ�����
		void add_waiting_load_role(uint64 role_id);

		//��ȡ��ǰ�ȴ����ص�����
		uint32 get_waiting_load_roles(void);

	protected:
		virtual void store(void);

	private:
		RoleInfoMap	m_role_infoes;

		Uint64List	m_load_role_list;	//������ص�role�б�
	};


} // namespace nexus

#endif	_NDB_NROLE_MGR_H_