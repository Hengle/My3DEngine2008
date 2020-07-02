/**
 *	nexus ndef_game - nmsg_chat.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NMSG_CHAT_H_
#define _NDEF_GAME_NMSG_CHAT_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * map ��ͼ�����Ϣ id����[Msg_Client_Chat, Msg_Client_Chat + 9]
	 */
	enum
	{
		S2C_ChatError			= Msg_Client_Chat,
		C2S_ChatMessage,
		S2C_ChatMessage,

	};

	// ������Ϣ���ݵ���󳤶�
	const uint8	MAX_MSG_LEN	= 200;

	enum EChatType
	{
		ECT_System					= 0,		// ϵͳ
		ECT_Say						= 1,		// ��ǰ
		ECT_Party					= 2,		// ����
		ECT_Guild					= 3,		// ����
		ECT_Whisper					= 4,		// ˽��
		ECT_Yell					= 5,		// ��
		ECT_Emote					= 6,		// ����
		ECT_MonsterSay				= 7,		// npc
		ECT_Channel					= 8,		// �Զ���Ƶ��
		ECT_PartyLeader				= 9,		// С�Ӷӳ�
		ECT_BattleGround			= 10,		// ս��
		ECT_BattleGroundLeader		= 11,		// ս������
		ECT_BossEmote				= 12,		// boss����
		ECT_BossWhisper				= 13,		// boss
		ECT_BG_SystemNeutral		= 14,		// ս��ϵͳ
		ECT_BG_SystemRed			= 15,		// ս��ϵͳ�췽
		ECT_BG_SystemBlue			= 16,		// ս��ϵͳ����
		ECT_Map						= 17,		// ��ǰ��ͼ

		ECT_Client					= 18,		// �ͻ���������Ƶ����ʾ��
		ECT_Loot					= ECT_Client + 0,
		ECT_Money					= ECT_Client + 1,
		ECT_Skill					= ECT_Client + 2,
	};

	// ����ö��ֵ
	enum EChatError
	{

	};

	struct tagTimeStamp
	{
		uint32		time_stamp1;
		uint32		time_stamp2;
		uint32		time_stamp3;
	};

	struct tagC2S_ChatMessage : public nmsg_base
	{
		uint8				type;
		uint8				msg_size;
		union
		{
			struct
			{
				nchar		msg[MAX_MSG_LEN + 1];
			} msg1;

			struct
			{
				uint64		to_player_id;
				nchar		msg[MAX_MSG_LEN + 1];
			} msg2;

			struct
			{
				uint32		channel_id;
				nchar		msg[MAX_MSG_LEN + 1];
			} msg3;
		};

		tagC2S_ChatMessage()
		{
			memset(this, 0, sizeof(*this));
			msg_id = C2S_ChatMessage;
		}

		uint32 get_packet_size() const 
		{
			if (ECT_Channel == type)
			{
				return sizeof(nmsg_base) + sizeof(type) + sizeof(msg_size) + sizeof(msg3.channel_id) + msg_size;
			}
			else if (ECT_Whisper == type)
			{
				return sizeof(nmsg_base) + sizeof(type) + sizeof(msg_size) + sizeof(msg2.to_player_id) + msg_size;
			}
			else
			{
				return sizeof(nmsg_base) + sizeof(type) + sizeof(msg_size) + msg_size;
			}
		}
	};

	struct tagS2C_ChatMessage : public nmsg_base
	{
		uint64				player_id;
		uint8				type;
		uint8				flag;
		uint8				msg_size;
		union
		{
			struct
			{
				nchar		msg[MAX_MSG_LEN + 1];
			} msg1;

			struct
			{
				uint32		channel_id;
				nchar		msg[MAX_MSG_LEN + 1];
			} msg2;
		};

		tagS2C_ChatMessage()
		{
			memset(this, 0, sizeof(*this));
			msg_id = S2C_ChatMessage;
		}

		uint32 get_packet_size() const 
		{
			if (ECT_Channel == type)
			{
				return sizeof(nmsg_base) + sizeof(player_id) + sizeof(type) + sizeof(flag) + sizeof(msg_size) + sizeof(msg2.channel_id) + msg_size;
			}
			else
			{
				return sizeof(nmsg_base) + sizeof(player_id) + sizeof(type) + sizeof(flag) + sizeof(msg_size) + msg_size;
			}
		}
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_CHAT_H_
