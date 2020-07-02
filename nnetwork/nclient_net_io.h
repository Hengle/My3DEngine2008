/**
*	Network for client
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#ifndef _NEXUS_CLIENT_NET_IO_H_
#define _NEXUS_CLIENT_NET_IO_H_
#include <string>
#include "conn_define.h"
#include "lib_define.h"
#include "net_packet.h"

namespace nexus
{
	
	/**
	 *	����Ϸ�ͻ����õ�����ģ��
	 *	@remark ʹ�õ��߳�ģʽ�����нӿ���Ҫ��ͬһ�߳�(ͨ������Ϸ�ͻ������߳�)�е��ã�
	*/
	class nNET_API nclient_net_io
	{
	public:
		nclient_net_io(void);
		virtual ~nclient_net_io(void);

		/** �첽�����ӷ����� */
		void connect_to(const std::string& host_name, const std::string& port, int time_out_sec);
		
		/** �����ڲ�IO�¼�����Ҫÿ֡����*/
		void tick();

		/** ��������Ͽ����� */
		void disconnect();

		/** ��ǰ��send packet�����һ��message 
		 *	@param msg ��Ҫ���͵���Ϣ���ڲ��Ὠ��һ�ݿ������˺������غ�msgָ����ڴ���԰�ȫ�ͷ�
		*/
		void send_push_msg(nnet_msg_header* msg);

		/** �첽�Ľ���������ͳ�ȥ*/
		void send_packet();

		/** �Ƿ��н��յ�����Ϣ */
		bool recv_empty() const;

		/** ���ն���ͷ������Ϣ
		 *	@return ������Ϣָ�����NULL���ⲿ��Ӧ�ó��д�ָ�룬���ڴ���nclient_net_io�ڲ�����
		*/
		nnet_msg_header* recv_front_msg() const;

		/**	������Ϣ����ͷ�ϵ���Ϣ
		*/
		void recv_pop_msg();

		/** ����һ������״̬�ı��call back����
		*/
		void set_state_call_back(conn_state_call_back on_state_change_func);

		/** �õ�����״̬ */
		EConnectionState get_state() const;
	private:
		struct impl;
		struct impl*	m_impl;
	};
}//namespace nexus
#endif //_NEXUS_CLIENT_NET_IO_H_