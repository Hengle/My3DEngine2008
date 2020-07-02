
#include <stdio.h>
#include <limits>
#include <boost/bind.hpp>

#include "ndb_mysql/ndb_sql_queue.h"
#include "test_ndb_mysql.h"


namespace nexus{

// ��������
bool ntest_ndb_mysql::test_connect_to_db_server(void)
{
	if( m_test_ndb.initialize(NULL, NULL, NULL, NULL,"loong_gmdb") )
	{
		return true;
	}
	return false;
}


// ���ָ�����ݿ��еı�
void ntest_ndb_mysql::simple_select_tables_in_db(void)
{
	if( !test_connect_to_db_server() ) return;

	char* mysql_query = "SHOW TABLES";

	if( m_test_ndb.send_query(m_query_result, mysql_query) )
		iterator_query_result();
}


// ��ִ��select
void ntest_ndb_mysql::simple_test_select(void)
{
	if( !test_connect_to_db_server() ) return;

	char* mysql_query = "select * from account";

	if( m_test_ndb.send_query(m_query_result, mysql_query) )
		iterator_query_result();
}


// ��ִ��insert
void ntest_ndb_mysql::simple_test_insert(void)
{
	if( !test_connect_to_db_server() ) return;

	char	name[12];
	int32	id =0;

	std::cout << "��database��loong_gmdb��account��insert" << std::endl;
	for( ; ; )
	{
		memset(name, '\0', 12);

		std::cout << "������ name(������10������)" << std::endl;
		std::cin>>name;
		std::cout << std::endl;

		std::cout << "������ id(>0:��Ч  ==0:�˳�  <0:��Ч)" << std::endl;
		std::cin>>id;
		std::cout << std::endl;

		if( ('\0'==name[0]) || (id<0) )
		{
			std::cout << "����������" << std::endl;
			continue;
		}

		if(0==id) return;

		m_query_stream.spell_insert("account");
		m_query_stream.append_col_value("id", id);
		m_query_stream.append_col_str_value("name", name);

		ndb_query_result query_result;
		if( m_test_ndb.send_query(query_result, m_query_stream.get_centence()) )
			iterator_query_result();
	}

}


// �򵥲��� ndb_query_result����[]�����
void ntest_ndb_mysql::test_qurey_result_operator(void)
{
	if( !test_connect_to_db_server() ) return;

	char* mysql_query = "select * from account";
	if( !m_test_ndb.send_query(m_query_result, mysql_query) )
		return;

	uint32 max_filed = m_query_result.get_col_count();
	m_query_result.next_row();
	for(	;	;	)
	{
		std::cout << "\n������� �� " << (max_filed-1) << std::endl;
		std::cout << "��ǰ�е�����(-1:�˳�) �� " << std::endl;
		
		uint32 cur_field = -1;
		std::cin >>cur_field;
		std::cout << std::endl;

		if( -1 == cur_field ) break;

		if( (cur_field>=max_filed) || (cur_field<0) )
		{
			std::cout << "�Ƿ��к�" << std::endl;
			continue;
		}

		out_to_screen(cur_field);
	}
}

// �򵥲��� ndb_query_result�����������Ʋ��ҽӿ�
void ntest_ndb_mysql::test_qurey_result_operator_name(void)
{
	if( !test_connect_to_db_server() ) return;

	char* mysql_query = "select * from account";
	if( !m_test_ndb.send_query(m_query_result, mysql_query) )
		return;

	for( ; m_query_result.next_row(); )
	{
		std::cout << "id = " << m_query_result.get_col_value("id") << std::endl;
		std::cout << "name = " << m_query_result.get_col_value("name") << std::endl;
		std::cout << "psd = " << m_query_result.get_col_value("psd") << std::endl;
		std::cout << "privilege = " << m_query_result.get_col_value("privilege") << std::endl;

		std::cout << "id = " << m_query_result["id"] << std::endl;
		std::cout << "name = " << m_query_result["name"] << std::endl;
		std::cout << "psd = " << m_query_result["psd"] << std::endl;
		std::cout << "privilege = " << m_query_result["privilege"] << std::endl;

		std::cout << "type id = " << m_query_result.get_col_type("id") << std::endl;
		std::cout << "type name = " << m_query_result.get_col_type("name") << std::endl;
		std::cout << "type psd = " << m_query_result.get_col_type("psd") << std::endl;
		std::cout << "type privilege = " << m_query_result.get_col_type("privilege") << std::endl;
	}
}


template<class T>
void get_value(ndb_query_stream& query_stream)
{
	for( ; ; )
	{
		T value;
		std::cout << "������ " << (std::numeric_limits<T>::min)() << " -- " << (std::numeric_limits<T>::max)() << "��Χ��ֵ" << std::endl;
		std::cin >> value;
		if ( ((std::numeric_limits<T>::max)()< value) && ((std::numeric_limits<T>::min)()>value) )
			continue;
		query_stream << value << " ";
		std::cout << " " << query_stream.get_centence() << " " << std::endl;
		std::cout << std::endl;
		break;
	}
}

// ���ndb_query_stream��<<���������
void ntest_ndb_mysql::test_query_stream_out_operator()
{
	m_query_stream.clear();
	
	get_value<int64>(m_query_stream);
	get_value<int32>(m_query_stream);
	get_value<int16>(m_query_stream);
	//get_value<int8>(m_query_stream);

	get_value<uint64>(m_query_stream);
	get_value<uint32>(m_query_stream);
	get_value<uint16>(m_query_stream);
	//get_value<uint8>(m_query_stream);

	get_value<bool>(m_query_stream);
	get_value<float>(m_query_stream);
	get_value<double>(m_query_stream);
	get_value<char>(m_query_stream);
	//nchar	nchar_value	= L'\0';	
}


// ���ƴдblob��
void ntest_ndb_mysql::test_spell_blob(void)
{
	if( !m_test_ndb.initialize(NULL, NULL, NULL, NULL, "loong_login"))
		return;

	const uint32	MAX_BLOB_LEN	= 256;
	const uint32	MAX_OUT_BLOB_LEN= MAX_BLOB_LEN*2 + 1;

	char	in_blob[MAX_BLOB_LEN];
	char	out_blob[MAX_OUT_BLOB_LEN];

	{//ƴblob��
		uint32	cur_len = MAX_OUT_BLOB_LEN;
		for(uint32 ch=0; ch<MAX_BLOB_LEN; ++ch)
			in_blob[ch] = ch;

		if( m_test_ndb.spell_blob_str(in_blob, MAX_BLOB_LEN, out_blob, cur_len) )
		{
			m_query_stream.spell_update("account");
			m_query_stream.append_col_str_value("mibao", out_blob);
			if( m_test_ndb.excute(m_query_stream.get_centence()) )
			{
				std::cout << "insert blob is success" << std::endl;
	}
		}
	}

	{//��ѯ loong_login.accout.mibao blob�ֶ�
		m_query_stream.spell_select("account", "mibao");
		m_test_ndb.send_query(m_query_result, m_query_stream.get_centence());

		for( ; m_query_result.next_row(); )
		{
			char mibao[MAX_OUT_BLOB_LEN];
			memset(mibao, 0, MAX_OUT_BLOB_LEN);
			m_query_result.get_real_value_blob("mibao", mibao, MAX_OUT_BLOB_LEN);
			mibao[MAX_OUT_BLOB_LEN-1] = '\0';
			for(int32 index=0; index<MAX_OUT_BLOB_LEN; ++index)
				std::cout << mibao[index];
			std::cout << std::endl;

			// ����ѯ�Ľ�� �� ֮ǰƴ���Ľ�� ��һ�Ƚ��ڴ����������Ƿ����
			if( memcpy(out_blob, mibao, MAX_OUT_BLOB_LEN) )
			{
				std::cout << "mibao �����ͬ" << std::endl;
		}
			return;
	}
	}

}


void ntest_ndb_mysql::_test_insert(uint32 start_data_level, uint32 end_data_level)
{
	if(start_data_level < end_data_level )
	{
		for(uint32 id=start_data_level; id<=end_data_level; ++id)
		{
			m_query_stream.spell_insert("account").append_col_value("id", id).append_col_str_value("name", "name");

			m_test_ndb.excute(m_query_stream.get_centence());
		}
	}
	else
	{
		for(uint32 id=start_data_level; id>=end_data_level; --id)
		{
			m_query_stream.spell_insert("account").append_col_value("id", id).append_col_str_value("name", "name");

			m_test_ndb.excute(m_query_stream.get_centence());
		}
	}
}
void ntest_ndb_mysql::test_insert_level(void)
{
	if( !m_test_ndb.initialize(NULL, NULL, NULL, NULL, "loong_login") )
		return;

	m_query_stream.spell_delete("account");
	m_test_ndb.excute(m_query_stream.get_centence());

	uint32 inter_data_level = 2000;
	uint32 max_level		= 2000;
	uint32 IN_MAX_LEVEL		= 5000000;

	std::cout << "������ " << (std::numeric_limits<uint32>::min)() << " -- " << (std::numeric_limits<uint32>::max)() << "��Χ��ֵ��" << std::endl;
	std::cout << "�����뱾�����ݲ�����( <" << IN_MAX_LEVEL << " )"<< std::endl;
	std::cin >> max_level;
	std::cout << std::endl;
	if(max_level > IN_MAX_LEVEL) max_level = IN_MAX_LEVEL;

	bool is_reverse = false;

	uint32	start_data_level= 1;
	uint32	end_data_level	= 0;
	for(uint32 index=1;		;  ++index)
	{
		start_data_level= end_data_level + 1;
		end_data_level	= start_data_level + inter_data_level*index;
		std::cout << "���β������������" << inter_data_level * (index+1) << " ";
		if( is_reverse )
		{//�������
			calc_opration_time(boost::bind(&ntest_ndb_mysql::_test_insert, this, end_data_level, start_data_level));
		}
		else
		{//�������
			calc_opration_time(boost::bind(&ntest_ndb_mysql::_test_insert, this, start_data_level, end_data_level));
		}

		is_reverse = !is_reverse;

		if(end_data_level >= max_level) return;
	}
}

void ntest_ndb_mysql::_test_update(void)
{
	std::cout << "������ݸ���" << std::endl;
	m_query_stream.spell_update("account").append_col_value<std::string>("id", "id+10000000").append_col_str_value("name", "name_update");
	m_test_ndb.excute(m_query_stream.get_centence());
}


void ntest_ndb_mysql::test_update_level(void)
{
	if( !m_test_ndb.initialize(NULL, NULL, NULL, NULL, "loong_login") )
		return;

	calc_opration_time(boost::bind(&ntest_ndb_mysql::_test_update, this));
}


void ntest_ndb_mysql::_test_select(uint32 rows)
{
	m_query_stream.spell_select("account", "id, name").set_limit(0, rows);

	if( m_test_ndb.send_query(m_query_result, m_query_stream.get_centence()) )
	{
		//iterator_query_result();
	}
}


void ntest_ndb_mysql::test_select_level(void)
{
	if( !m_test_ndb.initialize(NULL, NULL, NULL, NULL, "loong_login") )
		return;

	uint32 max_level		= 2000;
	uint32 IN_MAX_LEVEL		= 5000000;

	std::cout << "������ " << (std::numeric_limits<uint32>::min)() << " -- " << (std::numeric_limits<uint32>::max)() << "��Χ��ֵ��" << std::endl;
	std::cout << "�����뱾�β�ѯ��������( <" << IN_MAX_LEVEL << " )"<< std::endl;
	std::cin >> max_level;
	std::cout << std::endl;
	if(max_level > IN_MAX_LEVEL) max_level = IN_MAX_LEVEL;

	calc_opration_time(boost::bind(&ntest_ndb_mysql::_test_select, this, max_level));
}


void ntest_ndb_mysql::_test_index_select(ndb_query_stream& stream)
{
	if( m_test_ndb.send_query(m_query_result, stream.get_centence()) )
	{
		iterator_query_result();
	}
}

// �������������Ĳ���
void ntest_ndb_mysql::test_index_select(void)
{
	if( !m_test_ndb.initialize(NULL, NULL, NULL, NULL, "loong_login") )
		return;

	
	{
		std::cout << "ִ����������ѯ" << std::endl;
		m_query_stream.clear();
		m_query_stream << "drop index id on account";
		m_test_ndb.excute(m_query_stream.get_centence());

		m_query_stream.spell_select("account", "*").set_where() << "id=10002001";
		calc_opration_time(boost::bind(&ntest_ndb_mysql::_test_index_select, this, m_query_stream));		

	}

	{
		std::cout << "ִ����������ѯ" << std::endl;
		m_query_stream.clear();
		m_query_stream << "create unique index id on account(id)";
		m_test_ndb.excute(m_query_stream.get_centence());

		m_query_stream.spell_select("account", "*").set_where() << "id=10002001";
		calc_opration_time(boost::bind(&ntest_ndb_mysql::_test_index_select, this, m_query_stream));
	
	}
	
}


// �򵥲��� get_real_value()
void ntest_ndb_mysql::simple_test_get_value_template(void)
{
	if( !test_connect_to_db_server() ) return;

	char* mysql_query = "select * from account";
	if( !m_test_ndb.send_query(m_query_result, mysql_query) )
		return;

	m_query_result.next_row();
	for( uint32 col=0; col<m_query_result.get_col_count(); ++col )
	{
		out_to_screen(col);
			}
			}

// �򵥲���һ�����ݴ洢
void ntest_ndb_mysql::simple_test_procedure(void)
{
	//if( !m_test_procedure_ndb.initialize(NULL, NULL, NULL,"test_proces") )
	//	return;

	//char* mysql_query = "CALL p_10()";
	//ndb_query_result query_result(m_test_procedure_ndb.send_query(mysql_query));
	//iterator_query_result(&query_result);
}

// �򵥼��sqlƴ��
void ntest_ndb_mysql::simple_test_query_stream(void)
{
	if( !m_test_ndb.initialize(NULL, NULL, NULL, NULL,"loong_gmdb") )
		return;

	{// select
		m_query_stream.spell_select("account","id, name, psd");
		m_query_stream.set_where()<<"id<100";

		if( m_test_ndb.send_query(m_query_result, m_query_stream.get_centence()) )
			iterator_query_result();
	}

	{// replace update
		m_query_stream.spell_select("account","id, name, psd");
		m_query_stream.set_where()<<"id=1";
		if( m_test_ndb.send_query(m_query_result, m_query_stream.get_centence()) )
			iterator_query_result();

		m_query_stream.spell_update("account").append_col_str_value("name", "test_spell_update").set_where() << "id = 1";
		if( m_test_ndb.send_query( m_query_result, m_query_stream.get_centence()) )
			iterator_query_result();


		m_query_stream.spell_update("account").append_col_str_value("name", L"test_spell_update").set_where() << "id = 1";
		if( m_test_ndb.send_query( m_query_result, m_query_stream.get_centence()) )
			iterator_query_result();

		m_query_stream.spell_select("account", "id, name");
		m_query_stream.set_where() << "id=1";
		if( m_test_ndb.send_query(m_query_result, m_query_stream.get_centence()) )
			iterator_query_result();
	}

	{//delete

	}
}


void ntest_ndb_mysql::simple_test_thread(void)
	{
	//m_test_ndb.initialize(NULL, NULL, NULL,"loong_gmdb");

	//for( uint32 id=1; id<10000; ++id)
	//{
	//	ndb_query_stream* query_stream = ndb_query_stream::alloc_query_stream();
	//	std::string field = query_stream->filter_format("id = %d, name = '%s_%d'", id, "name", id);
	//	query_stream->spell_insert("account",  field.c_str());
	//	m_test_ndb.excute(query_stream);
	//}
	}

// �ܵļ�⺯��
void ntest_ndb_mysql::test(void)
{
	ntest_ndb_mysql m_test_ndb;

	int32	exit_flag = 0;

	for( ; ; )
	{
		std::cout << "�����" << std::endl;
		std::cout << "1:�������\n2:�򵥲������ݿ��еı�\n3:��select\n4:���query_result��[]������" << std::endl;
		std::cout << "5:��insert\n6:�򵥼���ȡ�н���ӿ�"<<std::endl;
		//std::cout << "7:�򵥴洢����\n8:���query_stream\n9:" << std::endl;
		std::cout << "8:���query_stream" << std::endl;
		std::cout << "10:�򵥲��� ndb_query_result�����������Ʋ��ҽӿ�"<<std::endl;
		std::cout << "11:���ndb_query_stream��<<���������" << std::endl;
		std::cout << "12:���ƴдblob��" << std::endl;
		std::cout << "15:���insert��ͬ������������Ҫʱ��" << std::endl;
		std::cout << "16:���update���ݱ�ʱ��" << std::endl;
		std::cout << "17:���select���ݱ�ʱ��" << std::endl;
		std::cout << "18:�������������Ĳ���" << std::endl;
		
		std::cout << "0:����" <<std::endl;

		std::cout << "�����������ݵ���ţ�   " << std::endl;
		
		std::cin >> exit_flag;
		std::cout << std::endl;

		switch(exit_flag)
		{
		case 0:
			return;
			break;
		case 1:
			m_test_ndb.test_connect_to_db_server();
			break;
		case 2:
			m_test_ndb.simple_select_tables_in_db();
			break;
		case 3:
			m_test_ndb.simple_test_select();
			break;
		case 4:
			m_test_ndb.test_qurey_result_operator();
			break;
		case 5:
			m_test_ndb.simple_test_insert();
			break;
		case 6:
			m_test_ndb.simple_test_get_value_template();
			break;
		case 7:
			//m_test_ndb.simple_test_procedure();
			break;
		case 8:
			m_test_ndb.simple_test_query_stream();
			break;
		case 9:
			//m_test_ndb.simple_test_thread();
			break;
		case 10:
			m_test_ndb.test_qurey_result_operator_name();
			break;
		case 11:
			m_test_ndb.test_query_stream_out_operator();
			break;
		case 12:
			m_test_ndb.test_spell_blob();
			break;
		case 15:
			m_test_ndb.test_insert_level();
			break;
		case 16:
			m_test_ndb.test_update_level();
			break;
		case 17:
			m_test_ndb.test_select_level();
			break;
		case 18:
			m_test_ndb.test_index_select();
			break;
}
	}
}


//������ѯ���ؽ����
void ntest_ndb_mysql::iterator_query_result()
{
	for( ; m_query_result.next_row(); )
		{
		for(uint32 col=0 ; col<m_query_result.get_col_count(); ++col)
		{
			out_to_screen(col);
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

// ���col��Ӧ����ֵ
void ntest_ndb_mysql::out_to_screen(uint32 col)
{
	std::cout << m_query_result.get_col_name(col) << " ";

	switch( m_query_result.get_col_type(col) )
	{
	case EFType_BYTE:
		std::cout << "value : " << m_query_result.get_real_value_byte(col) << std::endl;
		break;
	case EFType_INTEGER:
		std::cout << "value : " << m_query_result.get_real_value<int32>(col) << std::endl;
		break;
	case EFType_FLOAT:
		std::cout << "value = " << m_query_result.get_real_value<float>(col) << std::endl;
		break;
	case EFType_STRING:
		{
			const uint32 len = 1024;
			nchar value[len];
			wmemset(value, L'\0', len);
			if( -1 != m_query_result.get_real_value_str(col, value, len) )
			{
				char* cur_locale = ::setlocale(LC_ALL, "chs");
				::wprintf(L"%s\n", value);
				::setlocale(LC_ALL, cur_locale);
				return;
			}
			std::cout << std::endl;
		}
		break;
	default:
		if( m_query_result.get_col_value(col) )
		std::cout << "value : " << m_query_result.get_col_value(col) << std::endl;
	}
}

std::string ntest_ndb_mysql::filter_sql_format(char* format, ...)
{
	if(!format) return NULL;

	const uint32 QUERY_SQL_LEN = 1024;
	char query_sql[QUERY_SQL_LEN];
	memset(query_sql, '\0', QUERY_SQL_LEN);

	va_list ap;
	va_start(ap, format);
	int res = vsnprintf_s( query_sql, QUERY_SQL_LEN, _TRUNCATE, format, ap );
	va_end(ap);

	return std::string(query_sql);
}


void ntest_ndb_mysql::calc_opration_time(boost::function<void()> sql_test_func)
{
	if(!sql_test_func) return;

	int32 last_time = ::GetTickCount();
	sql_test_func();
	int32 elapse_time = ::GetTickCount() - last_time;

	char	out_char	= '*';	//����ַ�
	int32	inter_time	= 1000;	//���ʱ��

	std::cout << "ʱ��Ϊ�� " << ( ((float)elapse_time)/(float)(inter_time) ) << std::endl;
	return;
	for(  ; elapse_time>=0; elapse_time-=inter_time/2 )
	{
		std::cout << out_char;
	}
	std::cout << std::endl;
}

} //namespace nexus