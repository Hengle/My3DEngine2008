/**
*	nexus ngame - nevent_processor
*
*	Copyright (C) 2010 NexusDev
*
*	Author: D. Zhao
*	Date:	Jun, 2010
*/

#ifndef _NGAME_NEVENT_PROCESSOR_H_
#define _NGAME_NEVENT_PROCESSOR_H_

#include "ncommon.h"
#include "nevent_base_args.h"
#include "nsafe_mem_pool.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace nexus {

	/**
	 *	�̰߳�ȫ˵��, tsΪ�̰߳�ȫ����
	 *	���÷�ʽ�ǻ���nunit���ʹ����¼�, ȫ���¼�����ts��ʽ����(�����ڵ�ͼ�߳��ڵ���), ȫ���¼�updateͨ�����б�֤�̰߳�ȫ
	 *	note: ע��bind��ĳ�Աʱ, Ҫ��֤�ص���������������, �ͷ�ʱ������¼�����������ģ��
	 */
	class nevent_processor : private nnoncopyable
	{
	public:
		typedef boost::function<void (const nevent_base_args*)> nevent_handler;

	public:
		nevent_processor();
		~nevent_processor();

		/**
		 *	����Ͷ���¼�
		 */
		void update(uint32 elapse);

		/**
		 *	���Ͷ�ݵ��¼�
		 */
		void clear_events();

		/**
		 *	ע���¼�������, ����ģ��������Լ���init��ע���Լ����ĵ��¼���������֤ע���¼��������ʹ�������ͬһ��ģ����
		 *	note:	��session��ע�᲻ͬ, �����¼�������, �����ǲ�ͬ��Ķ���, ����ע������ڲ�ͬ����
		 *			session��������ͬһ����ĺ���, ����ͳһ���������
		 */
		void register_event_handler(uint16 event_id, nevent_handler handler);

		/**
		 *	ֱ�Ӵ����¼�, �����¼��߲������¼�������˭����, ��ô����, ������
		 *	note: ��������, ������֪��Ҫ����ʲô���飬���ķ���ֵ
		 */
		void send_event(const void* event_ptr, uint32 size);

		/**
		 *	Ͷ���¼�, Ͷ���¼��߲������¼�������˭����, ��ô����
		 */
		void post_event(uint32 expire_time, const void* data_ptr, uint32 size);

		/**
		 *	�̰߳�ȫ�ӿ�
		 */
		void send_event_ts(const void* event_ptr, uint32 size);
		void post_event_ts(uint32 expire_time, const void* data_ptr, uint32 size);

	private:
		struct nevent
		{
			uint32			expire_time;
			uint32			size;
			char			buffer[sizeof(uint32)];

			nevent() : expire_time(0), size(0) {}
		};

		typedef std::multimap<uint16, nevent_handler>	nevent_handler_map;
		typedef std::list<nevent*>						nevent_list;

	private:
		nevent_list							m_events;
		nevent_handler_map					m_handers;
		nfast_mutex							m_mutex;
		static nsafe_mem_pool				m_pool;
	};

} // namespace nexus

#endif // _NGAME_NEVENT_PROCESSOR_H_
