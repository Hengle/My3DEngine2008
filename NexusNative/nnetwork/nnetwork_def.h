/**
 *	nexus network - nnetwork_def.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NNETWORK_DEF_H_
#define _NNETWORK_NNETWORK_DEF_H_

#include "ncommon.h"
#include "nsafe_mem_pool.h"

namespace nexus {

	class ntcp_session;

#pragma pack(push,1)

	/**
	 *	IOCP��ɰ�����
	 */
	enum IOCompletionStatus
	{
		IOCS_ACCEPT				= 0,		// ����������ɻ������������
		IOCS_READ				= 1,		// ���������
		IOCS_WRITE				= 2,		// д�������
		IOCS_MAX				= 3,
	};

	/**
	 *	����ײ�session״̬, ntcp_session, ncluster_session, ntcp_session_client
	 */
	enum TCPSessionStatus
	{
		TSS_NONE				= 0,
		TSS_INIT				= 1,
		TSS_ACCEPTEX			= 2,
		TSS_OPEN				= 3,
		TSS_SHUT				= 4,
		TSS_CLOSE				= 5,
		TSS_MAX					= 6,
	};

	/**
	 *	����ײ�״̬, ntcp_server, ncluster_server, ncluster_client, ntcp_client_iocp
	 */
	enum TCPServerStatus
	{
		TSVRS_NONE				= 0,
		TSVRS_INIT				= 1,
		TSVRS_OPEN				= 3,
		TSVRS_SHUT				= 4,
		TSVRS_CLOSE				= 5,
		TSVRS_MAX				= 6,
	};

	/**
	 *	on_connect�ص���������ö��ֵ, ncluster_client, ntcp_client_iocp
	 */
	enum EConnectCode
	{
		ECC_Connected			= 0,		// �Ѿ����Ӻ�
		ECC_ConnectTimeout		= 1,		// ���ӳ�ʱ
		ECC_ConnectAborted		= 2,		// ��������ȡ��
		ECC_Error				= 3,		// ����ʱ��������
		ECC_Max,
	};

	struct noverlappedex
	{
		OVERLAPPED			ov;
		IOCompletionStatus	status;

		noverlappedex() : status(IOCS_MAX)
		{
			ov.Internal		= 0;
			ov.InternalHigh	= 0;
			ov.Offset		= 0;
			ov.OffsetHigh	= 0;
			ov.hEvent		= 0;
		}
	};

	struct noverlappedex2 : public noverlappedex
	{
		void*		session_ptr;

		noverlappedex2() : session_ptr(NULL) {}
	};

	struct ntcp_server_config
	{
		uint16			port;						// �˿�, �����е�ַ
		uint32			max_session;				// �������������������
		uint32			max_send_size;				// ���η��͵�����ֽ���(ѹ��ǰ),��Ҫ�����ͻ��˵Ļ�������С48 * 1024!!!
		uint32			max_send_msg;				// WSASend���������Ϣ��
		uint32			max_recv_size;				// �����ܰ�����
		bool			reuse_port;					// �Ƿ����ö˿�
		bool			nagle;						// �Ƿ�ʹ��Nagle�㷨
		bool			minilzo;					// �Ƿ�ʹ��minilzo�㷨

		ntcp_server_config() : port(6000), max_session(4000), max_send_size(48 * 1024), max_send_msg(200), max_recv_size(512), 
			reuse_port(false), nagle(false), minilzo(true) {}
	};

	struct nsend_buffer
	{
		void*			buffer_ptr;
		nsend_buffer*	next_ptr;
		uint32			size;

		nsend_buffer() : buffer_ptr(NULL), next_ptr(NULL), size(0) {}
	};

	// �ýṹ�����ڹ���ע����Щ�ǲ���ģ���Щ��Ϊ�˹�����Ա��
	struct nmessage
	{
		nsafe_mem_pool*	pool_ptr;					// ��¼���ĸ�mem_pool����
		nmessage*		next_ptr;					// ָ����һ����Ϣ, ���ڹ����߼�������Ϣ����
		volatile LONG	reference;					// ��¼�����ü���
		uint32			size;						// ���յ���Ч���ݳ���, ���������׵�ַ&buffer, ���������׵�ַ&session_id
		uint32			session_id;					// ��������Ϣ�׵�ַ&session_id
		uint32			server_id;					// ngateway������id����nworld����
		char			buffer[sizeof(uint32)];		// ���������׵�ַ&buffer

		nmessage() : pool_ptr(NULL), next_ptr(NULL), reference(0), size(0), session_id(0), server_id(0) {}

		// �����ͷ�sendʱ��������Դ, �߼���Ϣ�Ĵ����ֱ���ͷţ����poolΪnull crush
		void dec_reference()
		{
			if (0 == ::InterlockedDecrement(&reference))
			{
				pool_ptr->free(this);
			}
		}

		// �黹�����ڴ�أ����poolΪnull crush
		void free()
		{
			pool_ptr->free(this);
		}
	};

	#define RECV_SIZE(size) (sizeof(nmessage) + (size))

	#define	NSMSG_OFFSET		(sizeof(nsafe_mem_pool*) + sizeof(LONG) + sizeof(nmessage*) + sizeof(uint32))
	#define	NCMSG_OFFSET		(NSMSG_OFFSET + sizeof(uint32) + sizeof(uint32))
	#define NC2MSG(base_ptr)	(reinterpret_cast<nmessage*>(reinterpret_cast<char*>(base_ptr) - NCMSG_OFFSET))
	#define NS2MSG(base_ptr)	(reinterpret_cast<nmessage*>(reinterpret_cast<char*>(base_ptr) - NSMSG_OFFSET))

#pragma pack(pop)

} // namespace nexus

#endif _NNETWORK_NNETWORK_DEF_H_
