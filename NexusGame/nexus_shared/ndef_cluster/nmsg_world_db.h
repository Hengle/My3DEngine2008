/**
 *	nexus ndef_cluster - nmsg_world_db.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_CLUSTER_NMSG_WORLD_DB_H_
#define _NDEF_CLUSTER_NMSG_WORLD_DB_H_

#include "nmsg_base_id.h"
#include "nprotocol_base.h"
#include "nmsg_base_error_code.h"
#include "nobject_fields.h"
#include "nlogin_define.h"
#include "nspell_data.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * msg idΪuint16����, ����msg id������2000����, 1001 - 1600
	 */
	enum
	{
		Msg_World_DB_Begin		= Msg_World_DB + 1,

		// character
		Msg_W2D_CharEnum,
		Msg_D2W_CharEnum,
		Msg_W2D_CharCreate,
		Msg_D2W_CharCreate,
		Msg_W2D_CharDelete,
		Msg_D2W_CharDelete,

		// loading, û����Ч����ҲҪ������Ӧ��Ϣ
		Msg_W2D_LoadPlayerData,
		Msg_D2W_LoadPlayerStat,
		Msg_D2W_LoadPlayerItem,
		Msg_D2W_LoadPlayerSpell,
		Msg_D2W_LoadPlayerAura,
		Msg_D2W_LoadPlayerCoolDown,
		Msg_D2W_LoadPlayerRoleInfo,
		Msg_D2W_LoadPlayerSkill,

		// saving, û����Ч����ҲҪ������Ӧ��Ϣ, ���һ��save��Ϣ�����˳�
		Msg_W2D_SavePlayerStat,
		Msg_W2D_SavePlayerItem,
		Msg_W2D_SavePlayerSpell,
		Msg_W2D_SavePlayerAura,
		Msg_W2D_SavePlayerCoolDown,


		Msg_W2D_UnloadPlayer,
		Msg_D2W_LoadPlayerAtt,

		Msg_W2D_DeleteCharacter,
		Msg_D2W_DeleteCharacter,
		Msg_W2D_Operate_Skill,
		Msg_D2W_Operate_Skill,
		Msg_World_DB_End		= 1600,
	};

	// ����ö��ֵ
	enum EDBError
	{
		EDBError_None,
		EDBError_CreateCharacterSucess,
		EDBError_DeleteCharacterSucess,
		EDBError_OperateSkillSuccess,
		EDBError_NoExistCharacter,
		EDBError_NoRoleOffLine,
		EDBError_NoExistSkill,
		EDBError_Max,
	};

	// �����ɫ�б�
	CMD_START(Msg_W2D_CharEnum)
		uint32			account_id;
	CMD_END

	// ���ؽ�ɫ�б�
	CMD_START(Msg_D2W_CharEnum)

	CMD_END

	// ���󴴽���ɫ
	CMD_START(Msg_W2D_CharCreate)
		uint32	account_id;
		nchar	name[MAX_ACCOUNT_LEN];
		uint8	sex;
		uint8	race;
		uint8	class_type;
	CMD_END

	// ���ؽ�ɫ�������
	CMD_START(Msg_D2W_CharCreate)
		uint16			error;
		uint64			player_id;
		// ...
	CMD_END

	// ����ɾ����ɫ
	CMD_START(Msg_W2D_CharDelete)
		uint64	player_id;
	CMD_END

	// ���ؽ�ɫɾ�����
	CMD_START(Msg_D2W_CharDelete)
		uint16			error;
		uint64			player_id;
	CMD_END

	// ������ؽ�ɫ�������
	CMD_START(Msg_W2D_LoadPlayerData)
		uint64			player_id;
	CMD_END

	// ���ؽ�ɫ������Ϣ
	CMD_START(Msg_D2W_LoadPlayerStat)
		uint32			stat_data[EPF_PlayerEnd - EUF_UnitSeparator];	// ����, (�仯����)
	CMD_END

	// ���ؽ�ɫ��Ʒ��Ϣ
	CMD_START(Msg_D2W_LoadPlayerItem)

	CMD_END

	// ���ؽ�ɫ������Ϣ
	CMD_START(Msg_D2W_LoadPlayerSpell)
		uint16			num;
		uint32			spell_data[MAX_SPELL_NUM];
	CMD_END

	// ���ؽ�ɫ״̬��Ϣ
	CMD_START(Msg_D2W_LoadPlayerAura)
		uint16			num;
		naura_data_db	aura_data[MAX_AURA_NUM];
	CMD_END

	// ���ؽ�ɫcd��Ϣ
	CMD_START(Msg_D2W_LoadPlayerCoolDown)
		uint16				num;
		nspell_cool_dowm	cd_data[MAX_CD_NUM];
	CMD_END


	// �����ɫ������Ϣ
	CMD_START(Msg_W2D_SavePlayerStat)

	CMD_END


	// �����ɫcd��Ϣ
	CMD_START(Msg_W2D_SavePlayerItem)

	CMD_END


	// �����ɫ������Ϣ
	CMD_START(Msg_W2D_SavePlayerSpell)

	CMD_END


	// �����ɫ״̬��Ϣ
	CMD_START(Msg_W2D_SavePlayerAura)

	CMD_END


	// �����ɫcd��Ϣ
	CMD_START(Msg_W2D_SavePlayerCoolDown)

	CMD_END

	// ��ɫ����
	CMD_START(Msg_W2D_UnloadPlayer)
		uint32	role_id;
	CMD_END

	// ������ؽ�ɫ�������
	CMD_START(Msg_D2W_LoadPlayerAtt)
		uint16			error;
	CMD_END

	// ���ؽ�ɫ��Ϣ
	CMD_START(Msg_D2W_LoadPlayerRoleInfo)
		uint32	account_id;
		uint64	role_id;
		nchar	name[MAX_ACCOUNT_LEN];
		uint8	sex;
		uint8	race;
		uint8	class_type;
		uint16	map_id;
		uint32	last_time;
	CMD_END

	const uint32 MAX_SKILL_LEN = 12;
	// ���ؽ�ɫSkill
	CMD_START(Msg_D2W_LoadPlayerSkill)
		uint64	role_id;
		uint32	num;
		uint32	skilles[MAX_SKILL_LEN];
	CMD_END



	//// ɾ����ɫ
	//CMD_START(Msg_W2D_DeleteCharacter)
	//	uint32	role_id;
	//CMD_END

	//// ɾ����ɫ
	//CMD_START(Msg_D2W_DeleteCharacter)
	//	uint32	role_id;
	//	uint8	result;
	//CMD_END

	enum EOperateSkill_Type
	{
		EOperateSkill_None,
		EOperateSkill_Add,		//���Ӽ���
		EOperateSkill_Delete,	//�Ƴ�����
	};
	// �Լ�����ز���(���ӡ�ɾ��)
	CMD_START(Msg_W2D_Operate_Skill)
		uint32	role_id;
		uint8	op_type;
		uint32	skill_id;
	CMD_END
	CMD_START(Msg_D2W_Operate_Skill)
		uint32	role_id;
		uint8	op_type;
		uint8	result;
	CMD_END

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_CLUSTER_NMSG_WORLD_DB_H_
