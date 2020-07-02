/**
*	Network for server
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#ifndef _NEXUS_TCP_CLIENT_H_
#define _NEXUS_TCP_CLIENT_H_
#include "lib_define.h"
#include <boost/shared_ptr.hpp>
#include "ntcp_client_connection.h"

namespace nexus
{
	class nasio_wrap;

	/**
	 *	���������ڲ�ʹ�õ�tcp client
	 *	@remarks ��������tcp client connection���������
	*/
	class nNET_API ntcp_client
	{
	public:
		typedef boost::shared_ptr<ntcp_client> ptr;

		ntcp_client(boost::shared_ptr<nasio_wrap> asio_ptr);
		~ntcp_client(void);

		/** �½�һ�����Ӷ����ⲿ����ʹ����ȥ���ӷ����� */
		ntcp_client_connection::ptr create_connection();

	private:
		struct impl;
		boost::shared_ptr<impl>	m_impl;
	};
}//namespace nexus

#endif //_NEXUS_TCP_CLIENT_H_