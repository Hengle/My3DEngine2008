/**
 *	nexus nlogin - nworld
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author:
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NWORLD_H_
#define _NLOGIN_NWORLD_H_

#include "ncommon.h"
#include "nlogin_utility.h"

namespace nexus {

	class nlogin_player;

	class nworld : private nnoncopyable
	{
	public:
		nworld();
		~nworld();

		bool init(uint32 world_session_id, uint32 world_id, const tstring& world_name);
		void destroy();
		void update(uint32 elapse);

		void set_gateway_session_id(uint32 gateway_session_id)	{ m_gateway_session_id = gateway_session_id; }

		uint32 get_world_id() const					{ return m_world_id; }
		uint32 get_world_name_crc() const			{ return m_world_name_crc; }
		const tstring& get_world_name() const		{ return m_world_name; }

		bool send_world_msg(void* msg_ptr, uint32 size);
		bool send_gateway_msg(void* msg_ptr, uint32 size);

		// ��ǰ����������Ƿ���������
		bool is_well(void);

		// ����ȴ���֤���
		void add_proof_player(nlogin_player* player_ptr);

		// �Ƴ���֤�б��е����
		void remove_from_proof(nlogin_player* player_ptr);

		// ��ҽ����Ŷ�
		void add_queue_player(nlogin_player* player_ptr);

		// ���ŶӶ������Ƴ�
		void remove_from_queue(nlogin_player* player_ptr);

		// ����˳�
		void login_out(uint32 account_id);

		// �ҳ�����֤�����е����
		nlogin_player* get_proofing_player(uint64 client_id);

	private:

		void update_queue(uint32 elapse);

		// ��ǰ���������Ƿ�ﵽ����
		bool is_upper(void);

	private:
		uint32			m_world_session_id;

		uint32			m_gateway_session_id;
		uint32			m_world_id;
		uint32			m_world_name_crc;
		tstring			m_world_name;

		PlayerList		m_proofing_playeres;	//��֤������б�
		PlayerList		m_queuing_playeres;		//�Ŷӵ�����б�

		uint32			m_max_player_num;		//������������
		uint32			m_cur_player_num;		//��ǰ��������
		uint32			m_per_permit_num;		//ÿһ��tick�������world������
	};

} // namespace nexus

#endif // _NLOGIN_NWORLD_H_
