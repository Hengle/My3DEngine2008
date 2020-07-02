/**
 *	nexus nshared - ndb_mysql
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _NSHARED_NDB_MYSQL_H_
#define _NSHARED_NDB_MYSQL_H_

#include "ndb_utility.h"

namespace nexus{

	class ndb_query_stream;
	class ndb_query_result;
	class ndb_mysql_queue;

	class ndb_connection
	{
	public:
		nmutex	m_mutex;
		MYSQL*	m_mysql;

		ndb_connection()
		{
			m_mysql = NULL;
		}
	};


	/**
	 *	ndb_mysql
	 */
	class ndb_mysql
	{
	public:
		ndb_mysql(void);
		~ndb_mysql(void);

		// Զ�����ݿ�����
		bool initialize(const char* host, const char* user,const char* port, const char* passwd, const char* db_name);

		// �����������ݿ�
		bool reconnect(void);

		// �ر����ݿ�
		void shut_down(void);

		/**
		 *	���ݿ��ѯ���
		 *	@param query	:	select��䣬�����洢���̲�ѯ
		 *	@remark	query	:	query������Ϊ�����Ʋ�ѯ��䣬ֻΪ��'\0'��β���ַ���
		 */
		bool send_query(ndb_query_result& query_result, const char* query);

		/**
		 *	���ݿ�������
		 *	@param opration : sql��ز������(����insert, delete, update, replace)
		 *	@remark			: ��send_query����������ִ��sql����Ҫ���ؽ����
		 */
		bool excute(const char* query);

		/**
		 *	ת��һ��blob�ַ���
		 *	@param out_len : ����ת�����out_blob�ĳ���
		 *	@remark out_len : ��Ҫ����out_len��ֵ out_len >= (2*len + 1)
		 */
		bool spell_blob_str(const char* blob, uint32 len, char* out_blob, uint32& out_len);

		// ��ȡ��ǰʹ�õ����ݿ�����
		std::string get_db_name(void) { return m_db_name; }

	private:
		// �õ�һ����������
		ndb_connection* get_free_connection(void);

		// ʹ������ͷ�һ������
		void return_connection(ndb_connection* con);

		// �������ݿ����Ӳ���
		void set_cfg(const char* host, const char* user, const char* port, const char* passwd, const char* db_name);

		// ��ʼ��
		void init(void);

		// ����sql���ǰ�Ļ����ж�
		bool pre_send_query(MYSQL* mysql_ptr, const char* query);


	private:
		// Զ�����ݿ�����
		bool _initialize(void);

		// ִ��sql���
		bool _excute_query(MYSQL* mysql_ptr, const char* query);

		// ָ���������Ƿ�����ͨ״̬
		bool _is_connect(MYSQL* mysql_ptr);

		// ��mysql��صĴ�����Ϣ
		void _error_sql_prompt(MYSQL* mysql_ptr, char* error_info);

		// ��mysql�޹صĴ�����Ϣ
		void _error_prompt(char* error_info);

	private:
		int32			m_connect_num;
		ndb_connection*	m_connectiones;

		// sql������
		ndb_sql_queue*	m_sql_queue;

		//mysql������Ҫ����ز���
		std::string m_host;
		std::string m_user;
		std::string m_db_name;
		std::string m_passwd;
		uint32		m_port;
		uint32		m_client_flag;

		//����sql�ͻ����ַ���
		std::string	m_client_character_set;
		//����sql�����ַ���
		std::string m_connect_character_sql;

	public:
		//��ȡ��ǰpc��CPU����
		static uint32 get_cpu_num(void);
	};
} // namespace nexus

#endif	_NSHARED_NDB_MYSQL_H_