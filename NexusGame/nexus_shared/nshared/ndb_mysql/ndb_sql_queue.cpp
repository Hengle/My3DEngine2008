#include "ndb_query_stream.h"
#include "ndb_sql_queue.h"

namespace nexus{

ndb_sql_queue::ndb_sql_queue(void)
{
	m_queue_one.clear();
	m_queue_other.clear();
	m_queue_read_ptr	= &m_queue_one;
	m_queue_wirte_ptr	= &m_queue_other;
	m_wait_change_queue	= 120;

	//m_change_queue_event:�ֶ���λ��λ����ʼ��ʱ���ڼ���״̬
	m_change_queue_event= ::CreateEvent(NULL, TRUE, TRUE, NULL);

	//m_empty_event:�ֶ���λ��λ����ʼ��ʱ���ڼ���״̬
	m_empty_event		= ::CreateEvent(NULL, TRUE, TRUE, NULL);
}


ndb_sql_queue::~ndb_sql_queue(void)
{
	// �رն����е�handle
	::CloseHandle(m_empty_event);
	::CloseHandle(m_change_queue_event);
}


void ndb_sql_queue::add(ndb_query_stream* sql_stream)
{
	if(!sql_stream) return;

	m_mutex_write.lock();

	// ��д���н���ʱ���ȴ�
	::WaitForSingleObject(m_change_queue_event, 120);

	m_queue_wirte_ptr->push_back(sql_stream);

	//������в�Ϊ�յ��¼�
	if( !empty() ) ::SetEvent(m_empty_event);

	m_mutex_write.unlock();
}


ndb_query_stream* ndb_sql_queue::get(void)
{
	ndb_query_stream* sql_stream = NULL;

	m_mutex_read.lock();
	if(!m_queue_read_ptr->empty())
	{
		sql_stream = m_queue_read_ptr->front();
		m_queue_read_ptr->pop_front();
	}
	else
	{//�����п�ʱm_queue_one��m_queue_other����
			::ResetEvent(m_change_queue_event);
		QueueList* temp = m_queue_read_ptr;
		m_queue_read_ptr = m_queue_wirte_ptr;
		m_queue_wirte_ptr= temp;
		m_queue_wirte_ptr->clear();

		if( empty() )
		{//���г���Ϊ��
			::ResetEvent(m_empty_event);
		}

			::SetEvent(m_change_queue_event);
		}
	m_mutex_read.unlock();

	return sql_stream;
}


} //namespace nexus