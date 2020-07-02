#pragma once

#include "nnetwork.h"

/**
 *	�ͻ������ݴ�����
 */
class client : public nexus::ntcp_client
{
public:
	client();
	virtual ~client();

	static client* instance();

	virtual void on_connect(uint32 error_code);
	virtual void on_datarecv(const void* msg_ptr, uint32 size);
	virtual void on_disconnect();
};
