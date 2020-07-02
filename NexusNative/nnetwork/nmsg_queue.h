/**
 *	nexus network - nmsg_queue
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NMSG_QUEUE_H_
#define _NNETWORK_NMSG_QUEUE_H_

#include "nmutex.h"

namespace nexus {

	struct nmessage;

	/**
	 *	nmsg_queue�����߼���Ϣ
	 *	@remark nmsg_queue ����ö��е���Ϣ������TODO: D. Zhao �ḻȡ��Ϣ�Ľӿ�
	 */
	class nNET_API nmsg_queue : private nnoncopyable
	{
	public:
		nmsg_queue() : m_max_size(0), m_size(0), m_head_ptr(NULL), m_tail_ptr(NULL) {}
		explicit nmsg_queue(uint32 max_size) : m_max_size(max_size), m_size(0), m_head_ptr(NULL), m_tail_ptr(NULL) {}
		virtual ~nmsg_queue();

		bool enqueue(nmessage* msg_ptr);
		nmessage* dequeue();
		void clear();

		NINLINE uint32 get_msg_num() const { return m_size; }

	private:
		nfast_recursive_mutex	m_mutex;				// �̰߳�ȫ��

		uint32					m_max_size;				// ���������ɵ���Ϣ�������
		uint32					m_size;					// ���е�ǰ��Ϣ����

		nmessage*				m_head_ptr;				// ���ڳ���
		nmessage*				m_tail_ptr;				// �������
	};

} // namespace nexus

#endif _NNETWORK_NMSG_QUEUE_H_
