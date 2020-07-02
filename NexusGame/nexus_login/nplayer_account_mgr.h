/**
 *	nexus login - nplayer_account_mgr
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NPLAYER_ACCOUNT_MGR_H_
#define _NLOGIN_NPLAYER_ACCOUNT_MGR_H_

#include "nlogin_utility.h"

namespace nexus{
	
	class nlogin_player;

	/**
	 *	nplayer_account_mgr
	 */
	class nplayer_account_mgr
	{
	public:
		nplayer_account_mgr(void);
		~nplayer_account_mgr(void);

		bool init(void);
		void destroy(void);

		EProofPolicy get_proof_policy(void){ return m_proof_policy; }
		uint32 get_cur_version(void){ return m_cur_version; }

		// ���汾�Ƿ�һ��
		bool check_version(uint32 version);

		// ������ƵĺϷ���
		bool check_name(const nchar* name);

		// ����˺��Ƿ����
		bool exist_name(const nchar* name);

		// ����˺��Ƿ����
		bool exist_account(uint32 account_id);

	private:
		uint32			m_cur_version;		// �������汾��
		EProofPolicy	m_proof_policy;		// �˺���֤����
		
	};


	extern nplayer_account_mgr g_player_account_mgr;

} // namespace nexus


#endif	_NLOGIN_NPLAYER_ACCOUNT_MGR_H_