/**
*	Network for server
*
*	Author: Neil
*	Date:	Feb, 2009
*/
#ifndef _NEXUS_TCP_SVR_H_
#define _NEXUS_TCP_SVR_H_
#include "lib_define.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include "ntcp_connection.h"

namespace nexus
{
	class nasio_wrap;

	typedef boost::function<void (ntcp_connection::ptr)> new_connection_handler;

	/**
	 *	A tcp server
	 *	@remark ��ָ����IP, port������listen��ͨ��call back����֪ͨ�ϲ��½������¼���
	 *			�½��������Ӷ������ϲ����ntcp_svr��������connection�����ã�
	*/
	class nNET_API ntcp_svr
	{
	public:
		typedef boost::shared_ptr<ntcp_svr> ptr;

		explicit ntcp_svr(boost::shared_ptr<nasio_wrap> asio_ptr);
		~ntcp_svr(void);

		/**	����������	*/
		bool startup(const std::string& address, const std::string& port, size_t max_pkt_size);

		/** �رշ����� */
		void shutdown();

		/** �����½�����ʱ��call back����
		 *	@remark �¼���������io worker thread�б�����
		*/
		void set_new_connection_handler(new_connection_handler handler);

	private:
		struct impl;
		boost::shared_ptr<impl>	m_impl;
	};
}//namespace nexus

#endif //_NEXUS_TCP_SVR_H_