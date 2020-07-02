/**
 *	nexus ndef_game - nmsg_map.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NMSG_MAP_H_
#define _NDEF_GAME_NMSG_MAP_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * map ��ͼ�����Ϣ id����[Msg_Client_Map, Msg_Client_Map + 9]
	 */
	enum
	{
		S2C_MapError			= Msg_Client_Map,
		C2S_MapEnter,
		S2C_MapEnter,
		S2C_TeleportTo,
		C2S_TeleportAck,
	};

	// ����ö��ֵ
	enum EMapError
	{
		EMR_MapInvalid					= 1,					// ��ͼʵ��������

	};

	// ��ͼͳһ���󷵻���Ϣ����ȷ�Ļ�������Ӧ���ݽ��
	CMD_START(S2C_MapError)
		int16			error;
	CMD_END

	// ��������ͼ
	CMD_START(C2S_MapEnter)
	CMD_END

	// �����ͼ���
	CMD_START(S2C_MapEnter)
		int16			error;
	CMD_END

	// ��������ͼ
	CMD_START(S2C_TeleportTo)
		uint32			map_id;
		vector3			pos;
		float			yaw;
	CMD_END

	// �ͻ��˼�������Դ��ȷ�ϴ���
	CMD_START(C2S_TeleportAck)
	CMD_END

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_MAP_H_
