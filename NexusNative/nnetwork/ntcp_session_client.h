/**
 *	nexus network - ntcp_session_client
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NTCP_SESSION_CLIENT_H_
#define _NNETWORK_NTCP_SESSION_CLIENT_H_

#include "ncommon.h"
#include "nnetwork_def.h"

#include <string>

namespace nexus {

	class ntcp_client_iocp;
	class nlzo_wrap;

	/**
	 *	ntcp_session_client���紫��Ự����һ������
	 */
	class nNET_API ntcp_session_client : private nnoncopyable
	{
	private:
		friend class ntcp_client_iocp;

	public:
		ntcp_session_client();
		virtual ~ntcp_session_client();

		/** 
		 * �첽�����ӷ�����
		 * @param ip_addresss	Զ��ip��ַ
		 * @param port			Զ�̶˿�
		 * @return				����id,0XFFFFFFFF��ʾ��������Ͷ��ʧ��
		 */
		uint32 connect_to(const std::string& ip_addresss, uint16 port);

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
		bool _init(uint16 index, ntcp_client_iocp* client_ptr, uint32 max_recv_size, uint32 max_send_msg, uint32 max_send_msg_size, uint32 time_out_sec, bool nagle);
		// �ͷ�����������ڴ���Դ��������pool������Դ��pool��Դ��closeʱ�黹
		void _free_memory();
		// ������Ͷ��ConnectExǰ���ݳ�ʼ��
		void _pre_connect();
		// �黹����pool��Դ
		void _release();
		// �ر�ָ������
		void _close();
		// ����Ϊ��С��Ϣ��λ���ص�����
		bool parse_msg(char* buffer_ptr, uint32 size, nsafe_mem_pool* pool_ptr);

		// ����session״̬
		NINLINE void _set_status(TCPSessionStatus status) { m_status = status; }
		// ���session״̬
		NINLINE TCPSessionStatus _get_status() const { return m_status; }

		// �������ӻص�
		void _handle_connect(DWORD last_error);
		// �����ݻص�
		void _handle_read(DWORD last_error, DWORD bytes_transferred, nsafe_mem_pool* pool_ptr, char* temp_ptr, nlzo_wrap* minilzo_ptr);
		// д���ݻص�
		void _handle_write(DWORD last_error, DWORD bytes_transferred);

	private:
		// session close �����࣬��Ҫ��������������Ƿ��ͷ�session pool��Դ
		class nclose_helper
		{
		private:
			friend class ntcp_session_client;

		public:
			nclose_helper(ntcp_session_client* session_ptr) 
				: m_session_ptr(session_ptr) {}
			~nclose_helper();

		private:
			ntcp_session_client*	m_session_ptr;
		};

	private:
		uint32				m_session_id;					// session_idһ������| ��2�ֽ� ��ʾindex | ��2�ֽڱ�ʾguid |��0XFFFFFFFFΪ�Ƿ���client_id
		ntcp_client_iocp*	m_client_ptr;					// �ϲ�clientָ�룬���ڻص���Ϣ
		SOCKET				m_socket;						// session��socket��Դ

		noverlappedex		m_read;							// ����wsarecv��connectex���ص��ṹ�壬��ɼ�Ϊsessionָ��
		noverlappedex		m_write;						// ����wsasend���ص��ṹ�壬��ɼ�Ϊsessionָ��

		nfast_mutex			m_mutex;						// ����send��������session״̬�ı�

		TCPSessionStatus	m_status;						// session״̬

		DWORD				m_msg_head;						// msg head ��������Ϣͷ4�ֽ�
		bool				m_lzo;							// msg body �Ƿ�ѹ������
		char*				m_recv_buffer_ptr;				// ������Ϣ������
		DWORD				m_read_rd;						// �������ݵĵ�ַƫ��ָ��
		DWORD				m_read_wr;						// �������ݵĵ�ַƫ��ָ��

		DWORD				m_write_rd;						// �������ݵĵ�ַƫ��ָ��
		DWORD				m_write_wr;						// �������ݵĵ�ַƫ��ָ��

		volatile LONG		m_wsa_send;						// �첽send��־��1��ʾ���ڴ���send���̣�0��ʾ�ȴ��ⲿ����send����������
		volatile LONG		m_wsa_recv;						// �첽recv��־��1��ʾ���ڴ���recv���̣�0��ʾrecv���̶Ͽ�

		nmessage*			m_send_msg_ptr;					// ��ǰ������Ϣ

		nmessage*			m_send_head_ptr;				// �����ݵķ��ͻ�����ͷ
		nmessage*			m_send_tail_ptr;				// �����ݵķ��ͻ�����β

		DWORD				m_cur_send_msg;					// ��ǰ�ȴ����͵���Ϣ��Ŀ

		DWORD				m_max_recv_size;				// ������Ϣ����󳤶�
		DWORD				m_max_send_msg_size;			// һ���첽wsasend����󳤶�
		DWORD				m_max_send_msg;					// �ȴ����͵������Ϣ����
		bool				m_nagle;

		std::string			m_ip_addresss;					// session���ӷ�������ip��ַ
		uint16				m_port;							// session���ӷ������Ķ˿�
		uint32				m_time_out_sec;					// �������ӳ�ʱʱ��s
	};

} // namespace nexus

#endif _NNETWORK_NTCP_SESSION_CLIENT_H_
