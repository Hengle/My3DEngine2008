/**
 *	nexus login - nlogin_db
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NLOGIN_DB_H_
#define _NLOGIN_NLOGIN_DB_H_

#include "base_depend.h"
#include "nlogin_utility.h"

namespace nexus{

	class ndb_mysql;

	/**
	 *	nlogin_db
	 */
	class nlogin_db
	{
	public:
		nlogin_db(void);
		~nlogin_db(void);

		bool init(std::string host, std::string port, std::string name);
		void destroy(void);

		// ����ָ���˺���Ϣ
		bool cache_account_info(uint32 account_id, naccount_info* info_ptr);

		// ָ���˺��Ƿ����
		bool exist_account(uint32 account_id);

		/**
		 *	��ȡָ���˺ŵ�����
		 *	@return psd : ��DB�л�ȡ������
		 */
		bool query_psd(uint32 account_id, char* psd);

		// ������ҵ�½��־
		void fix_player_login_status(uint32 account_id, EPlayerLoginStatus status);

		// ��ȡ����ܱ�
		bool get_mibao(uint32 account_id, char* mibao, uint32 len);

		// ��ȡ��ҵ�privledge
		uint8 query_account_privilege(uint32 account_id);

		// ��ҵǳ�
		void login_out(uint32 account_id, uint32 last_ip);

		// ��ȡ�˺ű����˺ŵ�����
		uint32 query_account_num(void);

		// �����˺�
		bool insert_account(naccount_info* data_ptr);

		// �޸����е��˺�Ϊָ��״̬
		bool fix_all_login_status(EPlayerLoginStatus status);

	private:
		bool is_exist_db(void){return (NULL !=m_db ); }

	private:
		ndb_mysql*	m_db;

	};

	//����ȫ�����ݿ���ʽӿ�
	extern nlogin_db g_login_db;

} // namespace nexus

#endif	_NLOGIN_NLOGIN_DB_H_