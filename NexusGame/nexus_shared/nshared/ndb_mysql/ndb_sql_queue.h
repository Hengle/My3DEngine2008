/**
 *	nexus nshared - ndb_mysql
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _NSHARED_NDB_SQL_QUEUE_H_
#define _NSHARED_NDB_SQL_QUEUE_H_

#include "nmutex.h"
#include "ndb_utility.h"

namespace nexus{

	class ndb_query_stream;

	/**
	 *	ndb_sql_queue
	 */
	class ndb_sql_queue
	{
	private:
		typedef	std::list<ndb_query_stream*>	QueueList;
		typedef	QueueList::iterator				QueueListIt;

	public:
		ndb_sql_queue(void);
		~ndb_sql_queue(void);

		//sql���������
		void add(ndb_query_stream* sql_stream);

		// �Ӷ�����ȡ��sql���
		ndb_query_stream* get(void);

		// ��ȡ����Ϊ�յ��¼�
		const HANDLE& get_empty_event(void){ return m_empty_event; }

	private:
		// ��ȡ����sql��������
		uint32 length(void){ return static_cast<uint32>(m_queue_one.size() + m_queue_other.size()); }

		bool empty(void){ return (0 == length()); }

	private:
		QueueList	m_queue_one;			//����
		QueueList	m_queue_other;			//����

		nmutex		m_mutex_read;			//�����л����ź���
		QueueList*	m_queue_read_ptr;		//��ǰ������

		nmutex		m_mutex_write;			//д���л����ź���
		QueueList*	m_queue_wirte_ptr;		//��ǰд����

		HANDLE		m_change_queue_event;	//��д���н����¼�
		uint32		m_wait_change_queue;	//�ȴ���д�¼���ʱ��(��λ������)

		HANDLE		m_empty_event;			//����Ϊ���¼�

	};
} // namespace nexus

#endif	_NSHARED_NDB_SQL_QUEUE_H_