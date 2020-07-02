/**
 *	nexus ndef_game - nmsg_gm_command.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NMSG_GM_COMMAND_H_
#define _NDEF_GAME_NMSG_GM_COMMAND_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * gm ������Ϣ
	 */
	enum
	{
		C2S_GMCommand		= Msg_Client_GM,
		S2C_GMCommandResult,

	};

	enum EGMErrorCode
	{
		EError_LowPrivilege		= 1,


	};

	const uint32 MAX_GM_COMMAND_LEN = 128;

	CMD_START(C2S_GMCommand)					// ͨ��GM����
		nchar	command[MAX_GM_COMMAND_LEN];
	CMD_END;

	CMD_START(S2C_GMCommandResult)				// ͨ��GM�����
		uint16	error_code;
	CMD_END;

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_GM_COMMAND_H_
