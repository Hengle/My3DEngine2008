
#include <boost/program_options.hpp>


#include "nlog_mt.h"
#include "ncore.h"

#include "ndb_query_stream.h"
#include "ndb_sql_queue.h"
#include "ndb_query_result.h"
#include "ndb_mysql.h"

namespace po = boost::program_options;

namespace nexus{

void output_debug_info(wchar_t* format, ...)
{
#if defined(_DEBUG)
	::OutputDebugStringW(L"\n");

	const uint32 QUERY_SQL_LEN = 1024;
	wchar_t info[QUERY_SQL_LEN];
	wmemset(info, L'\0', QUERY_SQL_LEN);

	int32 res = 0;
	va_list ap;
	va_start(ap, format);
	res = _vsnwprintf_s( info, QUERY_SQL_LEN, _TRUNCATE, format, ap );
	va_end(ap);
	info[res+2] = L'\0';
	::OutputDebugStringW(info);

	::OutputDebugStringW(L"\n");
#endif
}


void output_debug_info(char* format, ...)
{
#if defined(_DEBUG)
	::OutputDebugStringA("\n");

	const uint32 QUERY_SQL_LEN = 1024;
	char info[QUERY_SQL_LEN];
	memset(info, '\0', QUERY_SQL_LEN);

	int32 res = 0;
	va_list ap;
	va_start(ap, format);
	res = _vsnprintf_s( info, QUERY_SQL_LEN, _TRUNCATE, format, ap );
	va_end(ap);
	info[res+1] = '\0';
	::OutputDebugStringA(info);

	::OutputDebugStringA("\n");
#endif
}


ndb_mysql::ndb_mysql(void)
{
	init();

	m_sql_queue = new ndb_sql_queue();
	assert(NULL != m_sql_queue);

	mysql_library_init(-1, NULL, NULL);
}


ndb_mysql::~ndb_mysql(void)
{
	if( m_sql_queue )
	{
		delete m_sql_queue;
		m_sql_queue = NULL;
	}

	shut_down();

	mysql_library_end();
}


// Զ�����ݿ�����
bool ndb_mysql::initialize(const char* host, const char* user, const char* port, const char* passwd, const char* db_name)
{
	set_cfg(host,user,port,passwd,db_name);

	return _initialize();
}


// �����������ݿ�
bool ndb_mysql::reconnect(void)
{
	shut_down();
	return _initialize();
}


// �ر����ݿ�
void ndb_mysql::shut_down(void)
{
	if(m_connectiones)
	{
		for(int32 index=0; index<m_connect_num; ++index)
		{
			mysql_close(m_connectiones[index].m_mysql);
		}
		delete [] m_connectiones;
		m_connectiones = NULL;
	}
}


/**
*	��Զ�����ݿⷢ��sql���
*	@param query	:	sql�������
*	@remark	query	:	query������Ϊ�����Ʋ�ѯ��䣬ֻΪ��'\0'��β���ַ���
*/
bool ndb_mysql::send_query(ndb_query_result& query_result, const char* query)
{
	bool result = false;
	// ������һ������
	ndb_connection* con = get_free_connection();
	if( con && _excute_query(con->m_mysql, query) )
	{
		MYSQL_RES*	mysql_res	= mysql_store_result(con->m_mysql);
		uint32		rows		= static_cast<uint32>( mysql_affected_rows(con->m_mysql) );
		uint32		cols		= static_cast<uint32>( mysql_field_count(con->m_mysql) );

		query_result.connect(mysql_res, rows, cols);

		if( mysql_more_results(con->m_mysql) )
		{//queryΪ�洢���̣����ؽ����Ϊ���У�ȥ�������Ĵ洢����״̬��
			for( ; !mysql_next_result(con->m_mysql); )
			{
				MYSQL_RES* other_res = mysql_store_result(con->m_mysql);
				mysql_free_result(other_res);
			}
		}
		result = true;
	}
	return_connection(con);

	return result;
	}


/**
*	���ݿ�������
*	@param opration : sql��ز������(����insert, delete, update, replace)
*	@remark			: ��send_query����������ִ��sql����Ҫ���ؽ����
*/
bool ndb_mysql::excute(const char* query)
	{
	bool result = false;

	// ������һ������
	ndb_connection* con = get_free_connection();
	if( con )
{
		result = _excute_query(con->m_mysql, query);
}
	return_connection(con);

	return result;
}


/**
 *	ת��һ��blob�ַ���
 *	@param out_len : ����ת�����out_blob�ĳ���
 *	@remark out_len : ��Ҫ����out_len��ֵ out_len >= (2*len + 1)
 */
bool ndb_mysql::spell_blob_str(const char* blob, uint32 len, char* out_blob, uint32& out_len)
{
	if( !len || !out_blob || out_len<(len*2+1) ) return false;

	ndb_connection* con = get_free_connection();
	if(!con) return false;

	memset( out_blob, '0', len*2+1);	
	out_len = mysql_real_escape_string(con->m_mysql, out_blob, blob, len);
	out_blob[out_len] = '\0';

	return_connection(con);
	return true;
}


// �õ�һ����������
ndb_connection* ndb_mysql::get_free_connection(void)
	{
	for(int32 index=0;  ; ++index)
{
		int	cur_index = (index) % m_connect_num;

		ndb_connection* con = m_connectiones + cur_index;
		if( !con )
		{
			_error_prompt("û���κ�mysql���ӣ�����������ݿ��Ƿ��н�������");
			return NULL;
		}

		if( con->m_mutex.try_lock() )
			return con;

		if( (m_connect_num-1)== cur_index )
			::Sleep(20);
}

	return NULL;
}


// ʹ������ͷ�һ������
void ndb_mysql::return_connection(ndb_connection* con)
{
	if( con ) con->m_mutex.unlock();
}


void ndb_mysql::set_cfg(const char* host, const char* user, const char* port, const char* passwd, const char* db_name)
{
		//���ò���
	if( NULL != host )		m_host		= std::string(host);
	if( NULL != user )		m_user		= std::string(user);
	if( NULL != port )		m_port		= boost::lexical_cast<uint32>(std::string(port));
	if( NULL != db_name )	m_db_name	= std::string(db_name);
	if( NULL != passwd )	m_passwd	= std::string(passwd);
}


// Զ�����ݿ�����
bool ndb_mysql::_initialize(void)
{
	if(m_connect_num<1 ) return false;

	m_connectiones = new ndb_connection[m_connect_num];

	for(int32 index=0; index<m_connect_num; ++index)
	{
	MYSQL* mysql_ptr = mysql_init(NULL);
	if(!mysql_ptr)
	{
		_error_sql_prompt(mysql_ptr, "��ʼ��mysql��ʧ�ܣ�");
			return false;
	}

	//������mysql_client���õ��ַ���
	mysql_options(mysql_ptr, MYSQL_SET_CHARSET_NAME, m_client_character_set.c_str());

		// ����mysql�Ͽ�����ִ��query֮ǰ�Զ�������
		bool is_reconnect = true;
		mysql_options(mysql_ptr, MYSQL_OPT_RECONNECT, &is_reconnect);

	mysql_ptr = mysql_real_connect( mysql_ptr, m_host.c_str(), m_user.c_str(), m_passwd.c_str(), m_db_name.c_str(), m_port, NULL, m_client_flag);
	if( mysql_ptr )
	{
		if( mysql_autocommit(mysql_ptr, 1) )
		{
			NLOG_ERROR(_T("autocommit not set to 1"));
		}

		mysql_query(mysql_ptr, m_connect_character_sql.c_str());
			m_connectiones[index].m_mysql = mysql_ptr;
		
			NLOG_INFO(_T("success connect to %d database"), m_db_name.c_str());
	}
	else
	{
			_error_sql_prompt(mysql_ptr, "unsuccess to connect to mysql: ");
	mysql_close(mysql_ptr);
			return false;
}
	}
	return true;
}


// ����sql���ǰ�Ļ����ж�
bool ndb_mysql::pre_send_query(MYSQL* mysql_ptr, const char* query)
{
	if( (NULL==query) || ('\0'==query[0]) )
	{
		_error_prompt("ָ����sql query���Ϊ��");
		return false;
	}

	if(!mysql_ptr)
	{
		_error_prompt("δ����Զ�����ݿ������");
		return false;
	}

	//if( !_is_connect(mysql_ptr) )
	//{//��Ƶ�����ʱ�ή�ͷ�������Ч�ʣ�ȥ��
	//	_error_prompt("�Ͽ���Զ�����ݿ������");
	//	return false;
	//}

	return true;
}


// ִ��sql���
bool ndb_mysql::_excute_query(MYSQL* mysql_ptr, const char* query)
{
	if( !pre_send_query(mysql_ptr, query) ) return false;

	uint32	result = mysql_query(mysql_ptr, query);
	if( result )
	{//TODO(LeoChen): ���ڷ��Ͳ�ѯ���Ĵ�����
		//CR_COMMANDS_OUT_OF_SYNC:	�Բ�ǡ����˳��ִ��������
		//CR_SERVER_GONE_ERROR:		MySQL������������
		//CR_SERVER_LOST:			�ڲ�ѯ�����У�������������Ӷ�ʧ
		//CR_UNKNOWN_ERROR:			����δ֪����

		_error_sql_prompt(mysql_ptr, "sql����ѯִ�д��� ");
		if(CR_SERVER_LOST == result)
		{//�ٴ�ִ��һ�����ӣ�������sql���
			_is_connect(mysql_ptr);
			result = mysql_query(mysql_ptr, query);
		}
	}

	return(0 == result);
}

bool ndb_mysql::_is_connect(MYSQL* mysql_ptr)
{
	if( !mysql_ptr) return false;

	output_debug_info( "%s", mysql_get_client_info() );
	output_debug_info( "%s", mysql_get_server_info(mysql_ptr) );

	return (0 == mysql_ping(mysql_ptr));
}


void ndb_mysql::init(void)
{
	m_connect_num	= get_cpu_num() * 2 + 2;
	m_connectiones	= NULL;

	m_port			= 3306;
	m_client_flag	= CLIENT_MULTI_RESULTS;//�����ض�����,�����Ҫ��Դ洢����

	m_client_character_set	= std::string("utf8");
	m_connect_character_sql	= std::string("SET NAMES utf8;");
}


void ndb_mysql::_error_sql_prompt(MYSQL* mysql_ptr, char* error_info)
{
	if(!mysql_ptr) return;

	char* default_error_pre = "mysql wrong info: ";
	if(!error_info)
	{
		error_info = default_error_pre;
	}

	std::string error_msg = mysql_error(mysql_ptr);
	NLOG_ERROR(_T("%s %s"), error_info, error_msg.c_str());
}


void ndb_mysql::_error_prompt(char* error_info)
{
	if(!error_info) return;

	NLOG_ERROR(_T("%s"), error_info);
}


//��ȡ��ǰpc��CPU����
uint32 ndb_mysql::get_cpu_num(void)
{
	SYSTEM_INFO sys_info;
	::GetSystemInfo(&sys_info);
	return static_cast<uint32>(sys_info.dwNumberOfProcessors);
}

} //namespace nexus