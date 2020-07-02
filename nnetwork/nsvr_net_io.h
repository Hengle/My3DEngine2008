/**
*	Network for server
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#ifndef _NEXUS_SVR_NET_IO_H_
#define _NEXUS_SVR_NET_IO_H_
#include "lib_define.h"
#include <boost/shared_ptr.hpp>
#include "ntcp_svr.h"
#include "ntcp_client.h"

namespace nexus
{
	class nasio_wrap;

	/**
	 *	server��������facade��
	 *	@remarks ���ಢ������ntcp_svr��ntcp_client���������
	*/
	class nNET_API nsvr_net_io
	{
	public:
		nsvr_net_io(void);
		~nsvr_net_io(void);

		/** ��������ʼ��asio service������io�̳߳�	*/
		void startup();

		/** �ر�asio service���ȴ�����io�߳̽��� */
		void shutdown();

		/** ����һ��tcp server���� */
		ntcp_svr::ptr create_tcp_svr();		

		/** ����һ��tcp client���� */
		ntcp_client::ptr create_tcp_client();

	protected:
		boost::shared_ptr<nasio_wrap> m_impl;
	};
}//namespace nexus
#endif //_NEXUS_SVR_NET_IO_H_