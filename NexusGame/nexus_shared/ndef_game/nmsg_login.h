/**
 *	nexus ndef_game - nmsg_login.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NMSG_LOGIN_H_
#define _NDEF_GAME_NMSG_LOGIN_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"
#include "nlogin_define.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * �˺���֤�����Ϣ id����[Msg_Client_Login, Msg_Client_Login + 9]
	 */
	enum
	{
		C2S_LoginProof			= Msg_Client_Login,
		S2C_LoginProofResult,
		C2S_MiBao,
		S2C_MiBao,
	};

	// ����ö��ֵ
	enum ELoginError
	{
		ELoginError_UNKNOWN,


		//��ɫ��¼�˺���֤(LoginProof)
		ELoginProof_No_Match,			//���ʺŷǷ�
		ELoginProof_In_Use,				//���ʺŵ�ǰ�Ѿ���ʹ��
		ELoginProof_Freezed,			//���ʺű�ͣ��
		ELoginProof_Wrong_Version,		//�汾�Ų���
		ELoginProof_Disabled,			//���ʺ��Ѿ�����ͣ
		ELoginProof_Queue,				//��ʼ�Ŷ�	
		ELoginProof_Psd_Error,			//�������
		ELoginProof_Mibao_Error,		//�ܱ�����
		ELoginProof_Wrong_Proof_Type,	//�Ƿ���֤����
		ELoginProof_Proofing,			//������֤���Ѿ���֤���
		ELoginProof_WorldName_Wrong,	//�Ƿ�����Ϸ������
		ELoginProof_World_No_Use,		//��Ϸ���粻����
		ELoginProof_SUCCESS,			//��֤�ɹ�


	};

	// �����½��֤
	CMD_START(C2S_LoginProof)
		uint32	cur_version;			// ��ǰ�汾ID
		uint32	account_id;				// �û�������crcֵ
		char	psd[MAX_MD5_SIZE];		// ����
		uint32	ip;						// ��½IP
		uint8	mac[8];					// mac��ַ
	CMD_END

	// ��½��֤���
	CMD_START(S2C_LoginProofResult)
		uint16			error;
	CMD_END

	// �����������ܱ�
	CMD_START(S2C_MiBao)
		nchar	MiBao[MAX_MIBAO_LEN];
	CMD_END

	// �ͻ�����д�ܱ���֤
	CMD_START(C2S_MiBao)
		uint32	mibao_crc;
	CMD_END

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_LOGIN_H_
