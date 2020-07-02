/**
 *	nexus  - ndb_mysql_test - test_other
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	May, 2010
 */

#ifndef _TEST_NNTEST_OTHER_
#define _TEST_NNTEST_OTHER_


#include <map>
#include <hash_map>
#include <boost/function.hpp>


#include "nutil.h"
#include "ncommon.h"

namespace nexus{

	typedef	std::map<int32, int32>			MapInt;
	typedef	MapInt::iterator				MapIntIt;


	typedef	stdext::hash_map<int32, int32>	HashMapInt;
	typedef	HashMapInt::iterator			HashMapIntIt;


	typedef	boost::unordered_map<int, int>	UnorderMapInt;
	typedef	UnorderMapInt::iterator			UnorderMapIntIt;

	class ndb_mysql;

	/**
	 *	ntest_other
	 */
	class ntest_other
	{
	public:
		ntest_other(void);
		~ntest_other(void);

		static void test(void);

	public:
		//���map ��hash_map��boost::unordered_map ��ͬ�����Ĳ����Ĳ���
		void test_map(void);

	/********************************************************************************************************/
	/*	                    �����ͨ��insert��select��ͬ�������Ĳ���										*/
	/********************************************************************************************************/
	public:

		void test_ndb_mysql(void);

		// ����С��Ĳ�ͬ����������ͨ����ʹ洢���̲��������
		void insert_small_dif_level(void);

		// ���Բ�ͬ�������Ĳ�ѯ
		void select_dif_level(void);

		//���ͬ�������Ĳ���
		void insert_big_dif_level(void);

		//���ͬ�������Ĳ�ѯ
		void select_big_dif_level(void);

		//���ʹ�ò�ͬ�Ĵ洢����
		void insert_dif_level_dif_engine(void);

	private:
		// ��ѯָ���������
		uint32 get_total_row(ndb_mysql* ndb_sql_ptr, const char* table_name);

		// ɾ�������������
		void send_del_sql(ndb_mysql* ndb_sql_ptr, const char* table_name);


		void _test_insert_small_proc(uint32* data_ptr, uint32 num);
		void _test_insert_big_proc_innodb(uint32* data_ptr, uint32 num);
		void _test_insert_big_proc_myisam(uint32* data_ptr, uint32 num);

		void _select_big_dif_level(const char* table_name, uint32 rows);
		void _select_small_dif_level(const char* table_name, uint32 rows);

		void _insert_small_common(uint32* data_ptr, uint32 insert_num);
		void _insert_big_common(uint32* data_ptr, uint32 insert_num);

		void _send_small_insert_sql(ndb_mysql* ndb_sql_ptr, uint32 account_id);
		void _send_big_insert_sql(ndb_mysql* ndb_sql_ptr, uint32 account_id);


		// ���ӵ�ָ�������ݿ�
		bool connect_to_db(const char* host, const char* user, const char* database);


	private:
		void call_test_func(boost::function<void()> test_func, const char* test_info);

		template<class map_type>
		void add_to_map(map_type * data_ptr, int32 start, int32 num)
		{
			for (int32 index=0 ; index< num ; ++index )
			{
				(*data_ptr)[index] = start + index;
			}
		}
		
		template<class map_type>
		void iterator_map(map_type* data_ptr)
		{
			if(!data_ptr) return;

			int32 value = 0;
			for( map_type::iterator it=data_ptr->begin(); data_ptr->end()!=it; ++it )
			{
				value = it->second;
			}
		}

		template<class map_type>
		void find_map(map_type* data_ptr)
		{
			if( !data_ptr ) return;

			size_t len = data_ptr->size();
			map_type::iterator it;
			for( size_t index=0; index<len; ++index )
			{
				data_ptr->find(s_util.rand32());
			}
		}

		template<class map_type>
		void delete_map(map_type* data_ptr)
		{
			if( !data_ptr ) return;

			for(map_type::iterator it=data_ptr->begin(); data_ptr->end()!=it; )
			{
				it = data_ptr->erase(it);
			}
		}

	private:
		ndb_mysql*	m_db_mysql;
		std::string	m_db_host;
		std::string m_db_user;
		std::string m_db_name;		// С�����ݿ�
		std::string m_db_big_name;	// ������ݿ�

		std::string m_table;		// С��
		std::string m_table_cmp;	// С��Ƚϱ�
		std::string m_big_table;	// ���
		std::string m_big_table_cmp;// ���Ƚϱ�

		std::string	m_big_table_myisam;	// ���MyISAM�洢����

	private:
		//��������������˺�
		class ngenerate_account_id
		{
		public:
			ngenerate_account_id(uint32 num, uint32 start, uint32 end)
			{
				m_accountes = new uint32[num];
				generate_account_id(num, start, end);
			}

			~ngenerate_account_id()
			{
				if(m_accountes)
				{
					delete [] m_accountes;
					m_accountes = NULL;
				}
			}

			uint32* get_accounts(void)
			{
				return m_accountes;
			}
		private:
			void generate_account_id(uint32 num, uint32 start, uint32 end)
			{
				if( !m_accountes || (0==num) ) return;

				memset(m_accountes, 0, sizeof(uint32)*num);

				MapInt test_only_account_id;

				for( uint32 index=0; index<num; ++index )
				{
					for( ; ; )
					{
						uint32 account_id = s_util.urand(start, end);
						if( test_only_account_id.end() == test_only_account_id.find(account_id) )
						{
							m_accountes[index] = account_id;
							test_only_account_id.insert( std::make_pair(account_id, account_id) );
							break;
						}
					};
				}
			}
		private:
			uint32* m_accountes;
		};

	};

	/**
	 *	��Consol��ȡ����
	 *	@param	describle : ��������������Ϣ
	 */
	template<class T>
	T in_from_console(const char* describle)
	{
		if( describle )
			std::cout << describle << std::endl;

		T param = (T)0;
		std::cin >> param;
		std::cout << std::endl;

		return param;
	}
} // namespace nexus

#endif	_TEST_NNTEST_OTHER_