#include "nclient.h"
#include "echo_test.h"

#include "../ncluster_server/nperfmon.h"

#include <iostream>

namespace nexus {

	bool g_connected = false;

	nclient::nclient()
	{
	}

	nclient::~nclient()
	{
	}

	void nclient::on_connect(uint32 error_code)
	{
		// �ص����������close ��Ҫ�ǲ���shutdown�Ƿ��ܹ��˳�
		// close();

		switch (error_code)
		{
		case ECS_Connected:
			{
				g_connected = true;
				std::cout << "it is successful to connect to the server!" << std::endl;
			}
			break;
		case ECS_ConnectTimeout:
			{
				std::cout << "the client is timeout when connecting to the server!" << std::endl;
			}
			break;
		case ECS_ConnectAborted:
			{
				std::cout << "the connect operator is aborted!" << std::endl;
			}
			break;
		case ECS_Error:
			{
				std::cout << "the error occur!" << std::endl;
			}
			break;
		default:
			{
				std::cout << "unknown error occur!" << std::endl;
			}
			break;
		}
	}

	void nclient::on_datarecv(const void* data_ptr, uint32 size)
	{
		// ������������ݿ�������Ϣ���У�Ŀǰ����ֻ�Ǵ�ӡ�յ�����Ϣ
		
		const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(data_ptr);

		switch (packet->msg_id)
		{
		case NS_Chat:
			{
				if (size != sizeof(tagNS_Chat))
				{
					std::cout << "msg size error!" << std::endl;
				}

				const tagNS_Chat* msg_data_ptr = static_cast<const tagNS_Chat*>(packet);
				char buff[300] = {0};
				memcpy(buff, msg_data_ptr->msg, msg_data_ptr->len);
				std::cout << "recv server msg:" << buff << std::endl;
			}
			break;
		case NS_MoveTest:
			{
				if (size != sizeof(tagNS_MoveTest))
				{
					std::cout << "msg size error!" << std::endl;
				}

				g_nperfmon.recv(size);

				const tagNS_MoveTest* msg_data_ptr = static_cast<const tagNS_MoveTest*>(packet);

				uint32 lag_time = ::GetTickCount() - msg_data_ptr->time_stamp;
				// uint32 lag_time = ::timeGetTime() - msg_data_ptr->time_stamp;
				g_nperfmon.lag(lag_time);

				if (msg_data_ptr->flag2 != 2000)
				{
					g_nperfmon.msg_error();
				}
	
				//static uint32 i = 0;

				//if (i != msg_data_ptr->flag)
				//{
				//	g_nperfmon.msg_error();
				//}

				//++i;
				// У����Ϣ�����ݺ��Ƿ�ʧ�߼���
			}
			break;
		default:
			{
				std::cout << "error msg id" << std::endl;
			}
			break;
		}

		// on_datarecv close
		// close();
	}

	void nclient::on_disconnect()
	{
		g_connected = false;
		std::cout << "disconect" << std::endl;
		// connect_to("127.0.0.1", 6000); �ú�����ntcp_clientû��shutdownʱ�ܵ��óɹ�����������ʧ��
	}

	nclient g_nclient;

} // namespace nexus
