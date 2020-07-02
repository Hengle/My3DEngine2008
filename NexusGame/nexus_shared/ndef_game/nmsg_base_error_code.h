/**
 *	nexus ndef_game - nmsg_base_error_code.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_GAME_NMSG_BASE_ERROR_CODE_H_
#define _NDEF_GAME_NMSG_BASE_ERROR_CODE_H_

namespace nexus {

#pragma pack(push,1)

	/**
	 * ����������Ϣ��ͬ�Ĵ�����, ����EError_Success�����д������Ϊ������������Ϣ���еĴ�����ɴ�1��ʼ
	 */
	enum EBaseErrorCode
	{
		EError_Success			= 0,
		EError_SystemError		= -1,

	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_BASE_ERROR_CODE_H_
