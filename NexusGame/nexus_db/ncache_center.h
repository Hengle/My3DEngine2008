/**
 *	nexus db - ncache_center
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NCACHE_CENTER_H_
#define _NDB_NCACHE_CENTER_H_

#include "ncache_utility.h"

#include "ncharacter.h"
#include "nrole_mgr.h"

namespace nexus{

	class nlan_session;

	/**
	 *	ncache_center : ���ݻ�������
	 *	@remark : Ŀǰ��˳����أ�˳���������ʱ�ٶ�������Ҫ��취���
	 */
	class ncache_center
	{
	public:
		friend	class nlan_session;

	public:
		ncache_center(void);
		~ncache_center(void);

		// ���������������ݿ�������
		void start(void);

		// ˢ��ʱ��
		void update(uint32 elapse);

		// �ر�ʱ���ڴ��е����ݶ��洢�����ݿ�
		void destroy(void);

		// ����character
		void insert_character(ncharacter_cache* data_ptr);

		// ɾ��character
		void destroy_character(uint64 role_id);

		nrole_mgr& get_role_mgr(void) { return m_role_mgr; }

	private:
		nrole_mgr	m_role_mgr;
	};

	extern ncache_center g_cache_center;
} // namespace nexus

#endif	_NDB_NCACHE_CENTER_H_