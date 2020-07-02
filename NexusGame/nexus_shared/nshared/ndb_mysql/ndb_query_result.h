/**
 *	nexus nshared - ndb_query_result
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _NSHARED_NDB_QUERY_RESULT_H_
#define _NSHARED_NDB_QUERY_RESULT_H_

#include <assert.h>

#include "ndb_utility.h"

namespace nexus{

	//�е���������ö��
	enum EFieldType
	{
		EFType_UNKNOWN	= 0,			// δ֪
		EFType_BYTE		= 1,			// �ֽ���(BYTE)
		EFType_INTEGER	= 2,			// ����
		EFType_BIGINT	= 3,			// 64������
		EFType_STRING	= 4,			// �ַ�����
		EFType_FLOAT	= 5,			// ������
		EFType_BLOB		= 6				// ����������
	};

	/**
	 *	ndb_query_result
	 */
	class ndb_query_result
	{
	public:
		ndb_query_result(void);
		~ndb_query_result(void);

		// ��ȡ�����
		void connect(MYSQL_RES* mysql_res, uint32 rows, uint32 fields);

		bool next_row(void);

		uint32	get_row_count(void){ return m_rows; };
		uint32	get_col_count(void){ return m_cols; };


		// col: ʵ�ʵ���ֵ(0,1,2,3,...N-1)
		char*	get_col_name(uint32 col);

		// col: ʵ�ʵ���ֵ(0,1,2,3,...N-1)
		char*	get_col_value(uint32 col);
		char*	get_col_value(const char* col_name);

		// col: ʵ�ʵ���ֵ(0,1,2,3,...N-1)
		char*	operator[](uint32 col) const;
		char*	operator[](const char* col_name) const;

		// col: ʵ�ʵ���ֵ(0,1,2,3,...N-1)
		EFieldType get_col_type(uint32 col);
		EFieldType get_col_type(const char* col_name);

		/**
		 *	��ȡʵ�ʸ��е���ֵ��ֵ
		 *	@param col: ʵ�ʵ���ֵ(0,1,2,3,...N-1)
		 *	@remark ע���get_real_value_byte()��get_real_vlaue_str������
		 *	@remark Ч�ʵ�
		 */
		template<class T> T get_real_value(uint32 col)
		{
			assert( !is_type(col, EFType_BYTE) );

			if( get_col_value(col) )
			return boost::lexical_cast<T>(std::string(get_col_value(col)));

			return (T)(0);
		}

		/**
		 *	@remark Ч�ʵ�
		 */
		template<class T> T get_real_value(const char* col_name)
		{
			int32 col = get_col_by_name(col_name);

			assert( !is_type(col, EFType_BYTE) );

			if( get_col_value(col) )
			return boost::lexical_cast<T>(std::string(get_col_value(col)));

			return (T)(0);
		}

		/**
		 *	��ȡָ���е��ַ���Ϣ
		 *	@return : ʵ�ʵĳ���(-1:��Ч����)
		 *	@param out value : ������
		 *	@param len : ���Ŀ��ĳ���
		 */
		int32 get_real_value_str(uint32 col, nchar* value, uint32 len);
		int32 get_real_value_str(const char* col_name, nchar* value, uint32 len);

		/**
		 *	��ȡʵ�ʸ��е�ֵ
		 *	@param col: ʵ�ʵ���ֵ(0,1,2,3,...N-1)
		 *	@remark ���е���ʵֵ����Ϊbyte
		 *	@remark Ч�ʵ�
		 */
		int8 get_real_value_byte(uint32 col)
		{
			assert( is_type(col, EFType_BYTE) );
			return  static_cast<int8> ( boost::lexical_cast<int32>(std::string(get_col_value(col))) );
		}

		/**
		 *	@remark Ч�ʵ�
		 */
		int8 get_real_value_byte(const char* col_name)
		{
			int32 col = get_col_by_name(col_name);
			return get_real_value_byte(col);
		}

		bool get_real_value_blob(uint32 col, char* value, uint32 len);
		bool get_real_value_blob(const char* col_name, char* value, uint32 len);


		bool is_type(uint32 col, EFieldType type)
		{
			return ( type==get_col_type(col) );
		}

		inline const char* get_string(uint32 col) const;

		inline bool get_bool(uint32 col);
		inline uint32 get_uint32(uint32 col);
		inline int32 get_int32(uint32 col);
		inline uint64 get_uint64(uint32 col);
		inline int8 get_int8(uint32 col);
		inline uint8 get_uint8(uint32 col);
		inline int16 get_int16(uint32 col);
		inline int16 get_uint16(uint32 col);
		inline float get_float(uint32 col);

		inline bool get_bool(const char* col_name);
		inline uint32 get_uint32(const char* col_name);
		inline int32 get_int32(const char* col_name);
		inline uint64 get_uint64(const char* col_name);
		inline int8 get_int8(const char* col_name);
		inline uint8 get_uint8(const char* col_name);
		inline int16 get_int16(const char* col_name);
		inline int16 get_uint16(const char* col_name);
		inline float get_float(const char* col_name);

	private:
		bool is_valid_col(uint32 col);

		int32 get_col_by_name(const char* name);

		void _init(MYSQL_RES* mysql_res, uint32 rows, uint32 cols);

		void _un_connect(void);

	private:
		MYSQL_RES*			m_mysql_res;//mysql�����
		MYSQL_ROW			m_cur_row;	//��ǰ��
		uint32				m_rows;		//�������
		uint32				m_cols;		//�������

		MYSQL_FIELD*		m_fieldes;	//�����ֶε�������Ϣ
	};

	#include "_ndb_query_result.h"

} // namespace nexus

#endif	_NSHARED_NDB_QUERY_RESULT_H_