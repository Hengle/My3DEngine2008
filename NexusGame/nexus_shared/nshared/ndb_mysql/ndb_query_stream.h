/**
 *	nexus nshared - ndb_query_result
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _NSHARED_NDB_QUERY_STREAM_H_
#define _NSHARED_NDB_QUERY_STREAM_H_

#include <assert.h>
#include "ndb_utility.h"

namespace nexus{
	/**
	 *	ndb_query_stream
	 */
	class ndb_query_stream
	{
	public:
		// ����һ��ndb_query_stream����
		static	ndb_query_stream* alloc_query_stream(void);

		// ����һ��ndb_query_stream����
		static	void free_query_stream(ndb_query_stream* qurey_stream);

		static std::string filter_format(char* format, ...);

	public:
		ndb_query_stream(void);
		~ndb_query_stream(void);

		// ��ȡsql���
		const char* get_centence(void);

		// ������ص�sql���
		ndb_query_stream& spell_select(const char* table, const char* seclect);
		ndb_query_stream& spell_update(const char* table);
		ndb_query_stream& spell_insert(const char* table);
		ndb_query_stream& set_replace(const char* table);
		ndb_query_stream& spell_delete(const char* table);

		ndb_query_stream& set_where();

		ndb_query_stream& spell_proce_start(const char* proce);
		ndb_query_stream& spell_proce_end(void);

		/**
		 *	@remark : �����ر�ǿ���ڽ��� ö�١�int8��uint8����ֵʱ��������ת��Ϊi32����
		 *	@example
		 *		int8 param = 8;
		 *		ndb_query_stream << ( (int32)(param) );
		 *		��Ҫԭ��boost::lexical_cast<std::string>(param)��ת��ʱ��param����charת�������������ֵ����
		 */
		template<class T>
		ndb_query_stream& operator << (const T param)
		{
			std::string sql_append = boost::lexical_cast<std::string>(param);
			m_sql_centence.append(sql_append.begin(), sql_append.end());
			return *this;
		}

		ndb_query_stream& append_nchar(const nchar* info, uint32 len);


		// ����sql����е���ֵ�� col = value; ��� spell_update��spell_insert��spell_replaceʹ��
		template<class T>
		ndb_query_stream& append_col_value(const char* col, T value)
		{
			assert( !is_empty_str(col) );

			if( !m_first_append )
			{
				m_first_append = true;
			(*this) << col << "=" << value;
			}
			else
			{
				(*this) << "," << col << "=" << value;
			}

			return *this;
		}

		//����sql����е���ֵ�� col = value(Ϊ�ַ�������); ��� spell_update��spell_insert��spell_replaceʹ��
		ndb_query_stream& append_col_str_value(const char* col, char* value);
		ndb_query_stream& append_col_str_value(const char* col, wchar_t* value);

		/**
		 *	����select���ش�ָ��λ�ÿ�ʼ��ָ�������Ľ����
		 *	@remark			: �ýӿ����spell_select
		 *	@param	offset:	: ָ���Ŀ�ʼλ��
		 *	@param	num		: ָ��������
		 */ 
		ndb_query_stream& set_limit(int32 offset, int32 num);

		void clear(void);

	private:
		bool is_empty_str(const char* str_ptr){ return ( (NULL==str_ptr) || ('\0'==str_ptr[0]) ); }
		bool is_empty_str(const wchar_t* str_ptr){ return ( (NULL==str_ptr) || (L'\0'==str_ptr[0]) ); }

	private:

		bool		m_first_append;

		std::string	m_sql_centence;
	};

} // namespace nexus

#endif	_NSHARED_NDB_QUERY_STREAM_H_