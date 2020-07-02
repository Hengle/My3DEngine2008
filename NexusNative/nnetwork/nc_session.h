/**
 *	nexus network - nc_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NNETWORK_NC_SESSION_H_
#define _NNETWORK_NC_SESSION_H_

#include "ncommon.h"
#include "nnetwork_def.h"

namespace nexus {

	class nc_server;
	class nc_client;

	/**
	 *	nc_session ���紫��Ự����һ���ͻ���
	 */
	class nNET_API nc_session : private nnoncopyable
	{
	private:
		friend class nc_server;
		friend class nc_client;

	public:
		nc_session();
		virtual ~nc_session();

		/** 
		 * �첽�����ӷ�����
		 * @param ip_addresss	Զ��ip��ַ
		 * @param port			Զ�̶˿�
		 * @return				����id,0XFFFFFFFF��ʾ��������Ͷ��ʧ��
		 */
		uint32 connect_to(const std::string& ip_addresss, uint16 port);

		/** 
		 * ��ָ�����ӷ������ݣ��ڲ��������ü����ķ�ʽ��������, 
		 * ���ͬһnmessage��send���Ҫ�������е����ô����ٵ���send, server�ڲ����ͷ�nmessage�ڴ�����
		 * note: gatewayʹ�ýӿ�
		 * @param session_id	����id
		 * @param msg_ptr		������Ϣ�ĵ�ַ
		 * @param size			�������ݵ�ʵ�ʳ���
		 */
		bool send(uint32 session_id, nmessage* msg_ptr, uint32 size);

		/** 
		 * ��ָ�����ӷ������ݣ��ڲ��Ὠ��һ�ݿ������˺������غ�msgָ����ڴ���԰�ȫ�ͷ�
		 * note: ��Ϸ�߼�������ʹ�ýӿ�
		 * @param session_id	����id
		 * @param msg_ptr		������Ϣ�ĵ�ַ
		 * @param size			�������ݵ�ʵ�ʳ���
		 */
		bool send_msg(uint32 session_id, const void* msg_ptr, uint32 size);

		/** 
		 * �ر�ָ������
		 * @param session_id	����id
		 */
		NINLINE void close(uint32 session_id)
		{
			if (session_id != m_session_id)
			{
				return;
			}

			_close();
		}

	private:
		// ���ӳ�ʼ�������ò�����������Դ, �������Ϳͻ��˹��ø�sessionͨ�����ݵĲ�������server_ptr��client_ptr����ֻ��һ���ǿ�
		bool _init(uint16 index, nc_server* server_ptr, nc_client* client_ptr, uint32 buffer_size, bool nagle);
		// �ͷ�����������ڴ���Դ��������pool������Դ��pool��Դ��closeʱ�黹
		void _free_memory();
		// ������Ͷ��acceptex��connectexǰ���ݳ�ʼ��
		void _pre_accept_or_connect();
		// ���Ӵ�ʱ����ʼ��socket���ã���iocp��Ͷ���첽������
		void _open(HANDLE completion_port, DWORD address, DWORD port);
		// �黹����pool��Դ
		void _release();
		// �ر�ָ������
		void _close();
		// ����Ϊ��С����Ϣ��λ
		bool _parse_msg(char* buffer_ptr, uint32 size, nsafe_mem_pool* pool_ptr);

		// ����session״̬
		NINLINE void _set_status(TCPSessionStatus status) { m_status = status; }
		// ���session״̬
		NINLINE TCPSessionStatus _get_status() const { return m_status; }

		// �������ӻص�
		void _handle_connect(DWORD last_error);
		// �����ݻص�
		void _handle_read(DWORD last_error, DWORD bytes_transferred, nsafe_mem_pool* pool_ptr);
		// д���ݻص�
		void _handle_write(DWORD last_error, DWORD bytes_transferred);

	private:
		// session close �����࣬��Ҫ��������������Ƿ��ͷ�session pool��Դ
		class nclose_helper
		{
		private:
			friend class nc_session;

		public:
			nclose_helper(nc_session* session_ptr) 
				: m_session_ptr(session_ptr) {}
			~nclose_helper();

		private:
			nc_session*	m_session_ptr;
		};

	private:
		uint32				m_session_id;					// session_idһ������| ��2�ֽ� ��ʾindex | ��2�ֽڱ�ʾguid |��0XFFFFFFFFΪ�Ƿ���client_id
		nc_server*			m_server_ptr;					// �ϲ�serverָ�룬���ڻص���Ϣ
		nc_client*			m_client_ptr;					// �ϲ�clientָ�룬���ڻص���Ϣ
		SOCKET				m_socket;						// session��socket��Դ

		noverlappedex2		m_operator;						// ����acceptex��connectex���ص��ṹ�壬��ɼ�Ϊ0��ͨ����ַƫ�Ƶõ�sessionָ��
		noverlappedex		m_read;							// ����wsarecv���ص��ṹ�壬��ɼ�Ϊsessionָ��
		noverlappedex		m_write;						// ����wsasend���ص��ṹ�壬��ɼ�Ϊsessionָ��

		nfast_mutex			m_mutex;						// ����send��������session״̬�ı�

		TCPSessionStatus	m_status;						// session״̬

		DWORD				m_msg_head;						// ��Ϣͷ
		char*				m_recv_msg_ptr;					// ������Ϣ
		DWORD				m_read_rd;						// �������ݵĵ�ַƫ��ָ��
		DWORD				m_read_wr;						// �������ݵĵ�ַƫ��ָ��

		char*				m_write_buffer_ptr[2];			// Ͷ��wsasend�����ݻ�����
		DWORD				m_write_rd[2];					// �������ݵĵ�ַƫ��ָ��
		DWORD				m_write_wr[2];					// �������ݵĵ�ַƫ��ָ��
		DWORD				m_write_index;					// ���ͻ���������

		volatile LONG		m_wsa_send;						// �첽send��־��1��ʾ���ڴ���send���̣�0��ʾ�ȴ��ⲿ����send����������
		volatile LONG		m_wsa_recv;						// �첽recv��־��1��ʾ���ڴ���recv���̣�0��ʾrecv���̶Ͽ�

		DWORD				m_max_buffer_size;				// ������Ϣ����󳤶�
		bool				m_nagle;						// �Ƿ���nagle�㷨
	};

} // namespace nexus

#endif // _NNETWORK_NC_SESSION_H_
