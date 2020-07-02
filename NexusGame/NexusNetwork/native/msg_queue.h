#pragma once
#include "ncommon.h"
#include "nsafe_mem_pool.h"

namespace nexus
{
	struct client_msg;

	/**
	* ��Ϣ����
	*/
	class msg_queue
	{
	public:
		msg_queue();
		virtual ~msg_queue();

		bool enqueue(const void* data_ptr, uint32 size);
		client_msg* dequeue();
		void clear();

	private:
		nsafe_mem_pool					m_msg_pool;
		nfast_recursive_mutex			m_mutex;				// �̰߳�ȫ��

		uint32							m_max_size;				// ���������ɵ���Ϣ�������
		uint32							m_size;					// ���е�ǰ��Ϣ����

		client_msg*						m_head_ptr;				// ���ڳ���
		client_msg*						m_tail_ptr;				// �������
	};
}
