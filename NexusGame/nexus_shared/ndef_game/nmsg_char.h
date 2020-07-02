/**
 *	nexus ndef_game - nmsg_char.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NMSG_CHAR_H_
#define _NDEF_GAME_NMSG_CHAR_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * char ��ɫ���������Ϣ id����[Msg_Client_Char, Msg_Client_Char + 19]
	 */
	enum
	{
		S2C_CharError			= Msg_Client_Char,
		C2S_CharEnum,
		S2C_CharEnum,
		C2S_CharCreate,
		S2C_CharCreate,
		C2S_CharDelete,
		S2C_CharDelete,

		C2S_CharEnterGame,
		S2C_CharEnterGame,

		C2S_CharLeaveGame,
	};

	// ����ö��ֵ
	enum ECharError
	{

	};

	// ��ɫ����ͳһ���󷵻���Ϣ����ȷ�Ļ�������Ӧ���ݽ��
	CMD_START(S2C_CharError)
		int16			error;
	CMD_END

	// �����ɫ�б�
	CMD_START(C2S_CharEnum)
	CMD_END

	// ��ɫ�б���
	CMD_START(S2C_CharEnum)
	CMD_END

	// ���󴴽���ɫ
	CMD_START(C2S_CharCreate)
	CMD_END

	// ������ɫ���
	CMD_START(S2C_CharCreate)
	CMD_END

	// ����ɾ����ɫ
	CMD_START(C2S_CharDelete)
	CMD_END

	// ɾ����ɫ���
	CMD_START(S2C_CharDelete)
	CMD_END

	// ������ȫ��
	// ���ð�ȫ��
	// ȡ����ȫ��

	CMD_START(C2S_CharEnterGame)
		uint64			player_id;
	CMD_END

	CMD_START(S2C_CharEnterGame)
		uint64			player_id;				// ��ɫid
		uint32			map_id;					// ��ͼ����id
		vector3			pos;					// λ�ã����������ģ��������ʱ�����ٶ�Ϊ0, flagΪ��ֹ
		float			yaw;					// ����
	CMD_END

	CMD_START(C2S_CharLeaveGame)
	CMD_END

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_CHAR_H_
