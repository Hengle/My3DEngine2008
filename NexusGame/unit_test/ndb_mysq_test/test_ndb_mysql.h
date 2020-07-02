/**
 *	nexus  - ndb_mysql_test - test_ndb_mysql
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _TEST_NDB_MYSQL_
#define _TEST_NDB_MYSQL_


#include "ndb_mysql/ndb_query_stream.h"
#include "ndb_mysql/ndb_query_result.h"
#include "ndb_mysql/ndb_mysql.h"
#include <boost/function.hpp>

namespace nexus{


	/**
	 *	ntest_ndb_mysql
	 */
	class ntest_ndb_mysql
	{
	public:
		ntest_ndb_mysql(void){}
		~ntest_ndb_mysql(void){}

		// ��������
		bool test_connect_to_db_server(void);

		// ���ָ�����ݿ��еı�
		void simple_select_tables_in_db(void);

		// ��ִ��select
		void simple_test_select(void);

		// ��ִ��insert
		void simple_test_insert(void);

		// �򵥲��� ndb_query_result����[]�����
		void test_qurey_result_operator(void);

		// �򵥲��� get_real_value()
		void simple_test_get_value_template(void);

		// �򵥲���һ�����ݴ洢
		void simple_test_procedure(void);

		// �򵥼��sqlƴ��
		void simple_test_query_stream(void);

		void simple_test_thread(void);

		// �򵥲��� ndb_query_result�����������Ʋ��ҽӿ�
		void test_qurey_result_operator_name(void);

		// ���ndb_query_stream��<<���������
		void test_query_stream_out_operator();

		// ���ƴдblob��
		void test_spell_blob(void);

		// ���ڲ������ݿ�ִ�в�ͬ������ͬ������������ݵ�����
		void test_insert_level(void);
		void test_update_level(void);
		void test_select_level(void);

		// �������������Ĳ���
		void test_index_select(void);

		// �ܵļ�⺯��
		static void test(void);

	private:
		//������ѯ���ؽ����
		void iterator_query_result(void);

		// ���col��Ӧ����ֵ
		void out_to_screen(uint32 col);

		std::string filter_sql_format(char* format, ...);

		void _test_insert(uint32 start_data_level, uint32 end_data_level);
		void _test_update(void);
		void _test_select(uint32 rows);
		void _test_index_select(ndb_query_stream& stream);

		void calc_opration_time(boost::function<void()> sql_test_func);

	private:
		ndb_mysql			m_test_ndb;
		ndb_query_result	m_query_result;
		ndb_query_stream	m_query_stream;
	};

} // namespace nexus

#endif	_TEST_NDB_MYSQL_