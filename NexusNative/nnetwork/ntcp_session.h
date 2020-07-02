/**
 *	nexus network - ntcp_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NTCP_SESSION_H_
#define _NNETWORK_NTCP_SESSION_H_

#include "ncommon.h"
#include "nnetwork_def.h"

namespace nexus {

	class ntcp_server;
	class nlzo_wrap;

	/**
	 *	ntcp_session���紫��Ự����һ���ͻ���
	 */
	class nNET_API ntcp_session : private nnoncopyable
	{
	private:
		friend class ntcp_server;

	public:
		ntcp_session();
		virtual ~ntcp_session();

		/** 
		 * ��ָ�����ӷ������ݣ��ڲ��������ü����ķ�ʽ��������
		 * ���ͬһnmessage��send���Ҫ�������е����ô����ٵ���send, server�ڲ����ͷ�nmessage�ڴ�����
		 * @param session_id	����id
		 * @param msg_ptr		������Ϣ�ĵ�ַ
		 * @param size			�������ݵ�ʵ�ʳ���
		 */
		bool send(uint32 session_id, nmessage* msg_ptr, uint32 size);

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
		// ���ӳ�ʼ�������ò�����������Դ
		bool _init(uint16 index, ntcp_server* server_ptr, uint32 max_recv_size, uint32 max_write_size, uint32 send_buffers, bool minilzo);
		// �ͷ�����������ڴ���Դ��������pool������Դ��pool��Դ��closeʱ�黹
		void _free_memory();
		// ������Ͷ��acceptexǰ���ݳ�ʼ��
		void _pre_accept();
		// ���Ӵ�ʱ����ʼ��socket���ã���iocp��Ͷ���첽������
		void _open(HANDLE completion_port, nsafe_mem_pool* pool_ptr, DWORD address, DWORD port);
		// ���ݴ��
		DWORD _packeted_buffer(char* temp_ptr, DWORD off_size);
		// �黹����pool��Դ
		void _release();
		// �ر�ָ������
		void _close();

		// ����session״̬
		NINLINE void _set_status(TCPSessionStatus status) { m_status = status; }
		// ���session״̬
		NINLINE TCPSessionStatus _get_status() const { return m_status; }

		// �����ݻص�
		void _handle_read(DWORD last_error, DWORD bytes_transferred, nsafe_mem_pool* pool_ptr);
		// д���ݻص�
		void _handle_write(DWORD last_error, DWORD bytes_transferred, char* temp_ptr, nlzo_wrap* minilzo_ptr);

	private:
		// session close �����࣬��Ҫ��������������Ƿ��ͷ�session pool��Դ
		class nclose_helper
		{
		private:
			friend class ntcp_session;

		public:
			nclose_helper(ntcp_session* session_ptr) 
				: m_session_ptr(session_ptr) {}
			~nclose_helper();

		private:
			ntcp_session*	m_session_ptr;
		};

	private:
		uint32				m_session_id;					// session_idһ������| ��2�ֽ� ��ʾindex | ��2�ֽڱ�ʾguid |��0XFFFFFFFFΪ�Ƿ���client_id
		ntcp_server*		m_server_ptr;					// �ϲ�serverָ�룬���ڻص���Ϣ
		SOCKET				m_socket;						// session��socket��Դ

		noverlappedex2		m_accept;						// ����acceptex���ص��ṹ�壬��ɼ�Ϊ0��ͨ����ַƫ�Ƶõ�sessionָ��
		noverlappedex		m_read;							// ����wsarecv���ص��ṹ�壬��ɼ�Ϊsessionָ��
		noverlappedex		m_write;						// ����wsasend���ص��ṹ�壬��ɼ�Ϊsessionָ��

		nfast_mutex			m_mutex;						// ����send��������session״̬�ı�

		TCPSessionStatus	m_status;						// session״̬

		nmessage*			m_recv_msg_ptr;					// ������Ϣ
		DWORD				m_read_rd;						// �������ݵĵ�ַƫ��ָ��
		DWORD				m_read_wr;						// �������ݵĵ�ַƫ��ָ��

		char*				m_write_buffer_ptr;				// Ͷ��wsasend�����ݻ�����
		DWORD				m_write_rd;						// �������ݵĵ�ַƫ��ָ��
		DWORD				m_write_wr;						// �������ݵĵ�ַƫ��ָ��

		volatile LONG		m_wsa_send;						// �첽send��־��1��ʾ���ڴ���send���̣�0��ʾ�ȴ��ⲿ����send����������
		volatile LONG		m_wsa_recv;						// �첽recv��־��1��ʾ���ڴ���recv���̣�0��ʾrecv���̶Ͽ�

		nsend_buffer*		m_send_buffers_ptr;				// ���͵Ļ�����

		nsend_buffer*		m_send_head_ptr;				// �����ݵķ��ͻ�����ͷ
		nsend_buffer*		m_send_tail_ptr;				// �����ݵķ��ͻ�����β
		nsend_buffer*		m_empty_head_ptr;				// �����ݵķ��ͻ�����ͷ

		DWORD				m_max_recv_size;				// ������Ϣ����󳤶�
		DWORD				m_max_write_size;				// һ���첽wsasend����󳤶�
		DWORD				m_send_buffers;					// �ȴ����͵������Ϣ����
		bool				m_minilzo;						// �Ƿ�ʹ��lzoѹ���㷨��������
	};

} // namespace nexus

#endif _NNETWORK_NTCP_SESSION_H_
