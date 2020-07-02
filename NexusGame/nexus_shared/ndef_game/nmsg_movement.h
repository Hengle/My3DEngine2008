/**
 *	nexus ndef_game - nmsg_movement.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_GAME_NMSG_MOVEMENT_H_
#define _NDEF_GAME_NMSG_MOVEMENT_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * �ƶ���Ϣ[Msg_Client_Movement, Msg_Client_Movement + 19]
	 */
	enum
	{
		S2C_MoveError			= Msg_Client_Movement,
		S2C_PlayerEnterAoi,
		S2C_CreatureEnterAoi,
		S2C_GameObjectEnterAoi,
		S2C_ObjectLeaveAoi,
		C2S_Move,
		S2C_Move,
		C2S_CheckTime,
		S2C_CheckTime,

	};

	enum EMoveError
	{
		EMRC_Success					= 0,		// �ƶ��ɹ�
		EMRC_CantArrive					= 1,		// ���ܵ���
		EMRC_Invalid					= 2,		// �Ƿ�����
		EMRC_Fear						= 3,		// �־�״̬
		EMRC_Root						= 4,		// ����״̬
		EMRC_Dead						= 5,		// ����״̬
		EMRC_Casting					= 6,		// �ͷż���
	};

	// �����ƶ�ʧ����Ϣ��ֻ���ظ������ƶ��Ŀͻ���
	CMD_START(S2C_MoveError)
		uint64			object_id;
		uint16			error_code;
		vector3			position;
		float			orient;
	CMD_END

	// ��ҽ�����Ұ
	CMD_START(S2C_PlayerEnterAoi)
		uint64			player_id;
		vector3			position;
		float			yaw;
		vector3			acceleration;
		float			time_stamp;
		uint8			flag;
	CMD_END

	// npc������Ұ
	CMD_START(S2C_CreatureEnterAoi)
		uint64			creature_id;
		vector3			position;
		float			yaw;
		vector3			acceleration;
		float			time_stamp;
		uint8			flag;
	CMD_END

	// ��Ϸ�о�ֹ�Ķ��������Ұ
	CMD_START(S2C_GameObjectEnterAoi)
		uint64			go_id;
		vector3			position;
		float			yaw;
	CMD_END

	// �����뿪��Ұ
	CMD_START(S2C_ObjectLeaveAoi)
		uint64			object_id;
	CMD_END

	// �����ƶ�
	CMD_START(C2S_Move)
		vector3			position;
		float			yaw;
		vector3			acceleration;
		float			time_stamp;
		uint8			flag;
	CMD_END

	// ͬ���ƶ�
	CMD_START(S2C_Move)
		uint64			unit_id;
		vector3			position;
		float			yaw;
		vector3			acceleration;
		float			time_stamp;
		uint8			flag;
	CMD_END

	// ����У��ʱ��
	CMD_START(C2S_CheckTime)
		float			client_time_stamp;
	CMD_END

	// У��ʱ����
	CMD_START(S2C_CheckTime)
		float			client_time_stamp;
		float			server_time_stamp;
	CMD_END

	// ���ɻ������⴦���������⣬�˵ķ���ͳ���һ�����ٶ����⣬���ɵ��ٶȻ�����ٶ�����

	struct EnterAoi
	{
		union
		{
			char data1[sizeof(tagS2C_PlayerEnterAoi)];
			char data2[sizeof(tagS2C_CreatureEnterAoi)];
			char data3[sizeof(tagS2C_GameObjectEnterAoi)];
		};
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_MOVEMENT_H_
