/**
 *	nexus network - nwinsock_init
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NWINSOCK_INIT_H_
#define _NNETWORK_NWINSOCK_INIT_H_

namespace nexus {

	/**
	 *	nwinsock_init��ʼ�������
	 *	@remark nwinsock_init ͨ����̬��������֤��ʼ���ҳ�ʼ��һ��
	 */
	class nwinsock_init
	{
	public:
		nwinsock_init()
		{
			WSADATA wsa_data;
			if (::WSAStartup(MAKEWORD(2, 2), &wsa_data) || ::WSAStartup(MAKEWORD(2, 1), &wsa_data)
				|| ::WSAStartup(MAKEWORD(2, 0), &wsa_data) || ::WSAStartup(MAKEWORD(1, 1), &wsa_data))
			{}
		}

		~nwinsock_init()
		{
			::WSACleanup();
		}
	};

	static nwinsock_init swinsock_init;

} // namespace nexus

#endif _NNETWORK_NWINSOCK_INIT_H_
