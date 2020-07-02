/**
 *	nexus ngateway - nwan_server
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGATEWAY_NWAN_SERVER_H_
#define _NGATEWAY_NWAN_SERVER_H_

#include "nnetwork.h"

namespace nexus {

	class nwan_session;

	/**
	 *	����WAN������
	 */
	class nwan_server : public ntcp_server
	{
	public:
		nwan_server();
		virtual ~nwan_server();

		bool init(uint32 max_session);
		void destroy();
		void update(uint32 elapse);

		// �����ص�����
		virtual void on_connect(uint32 session_id, uint32 address, uint32 port);
		virtual void on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size);
		virtual void on_disconnect(uint32 session_id);

		nwan_session* get_session(uint32 session_id);
		void broadcast(nmessage* msg_ptr, uint32 size);

	private:
		nwan_session*				m_session_ptrs;						// sessionָ������
		uint32						m_max_session;						// ���ɵ����session��Ŀ
	};

	extern nwan_server g_wan_server;

} // namespace nexus

#endif // _NGATEWAY_NWAN_SERVER_H_
