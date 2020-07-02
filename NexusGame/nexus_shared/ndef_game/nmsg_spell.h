/**
 *	nexus ndef_game - nmsg_spell.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDEF_GAME_NMSG_SPELL_H_
#define _NDEF_GAME_NMSG_SPELL_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"
#include "nspell_data.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * ����buff�����Ϣ id����[Msg_Client_Spell, Msg_Client_Spell + 50]
	 */
	enum
	{
		S2C_SpellError			= Msg_Client_Spell,
		C2S_CastSpell,
		S2C_CastSpellResult,
		C2S_CancelCast,
		S2C_SpellPrepare,
		S2C_SpellCasting,
		S2C_SpellFailed,
		S2C_SpellHit,

		C2S_CancelAura,
		S2C_AddAura,
		S2C_RemoveAura,
		S2C_UpdateAura,


	};

	// �����ͷŽ��ö��ֵ
	enum ESpellCastResult
	{
		ESCR_Success						= 0,
		ESCR_Failed_NoSpell					= 1,		// �����ͷŵļ���û��ѧ��
		ESCR_Failed_InvalidSpell			= 2,		// �����ͷŵļ��ܷǷ�(��������, ����id������Ч)
		ESCR_Failed_SpellInProgress			= 3,		// �ͷ�������ִ����һ����
		ESCR_Failed_CoolDown				= 4,		// ������ȴδ����
		ESCR_Failed_TooClose				= 5,		// ָ��Ŀ��̫Զ��
		ESCR_Failed_OutofRange				= 6,		// ָ��Ŀ��̫����
		ESCR_Failed_CasterDead				= 7,		// �ͷ����Ѿ�����
		ESCR_Failed_Interrupted				= 8,		// ���ܱ����(����)
		ESCR_Failed_NoMana					= 9,		// ħ��ֵ����
		ESCR_Failed_NoEnergy				= 10,		// ����ֵ����
		ESCR_Failed_LineofSight				= 11,		// �����赲
		ESCR_Failed_InvalidTargets			= 12,		// ָ��Ŀ��Ƿ�
		ESCR_Failed_InCombat				= 13,		// ����ս��״̬
		ESCR_Failed_InFear					= 14,		// ���ڿ־�״̬
		ESCR_Failed_InSilence				= 15,		// ���ڳ�Ĭ״̬
		ESCR_Failed_InRoot					= 16,		// ���ڶ���״̬
		ESCR_Failed_CantRepeat				= 17,		// �ü��ܲ��������ͷ�
		ESCR_Failed_Cancel					= 18,		// ���ܱ����(����ȡ��)
		ESCR_Failed_Move					= 19,		// �ƶ��в����ͷ�

	};

	// �������н��ö��ֵ
	enum ESpellHitResult
	{
		ESHR_Hit							= 0x00,		// ����
		ESHR_Crit							= 0x01,		// ����
		ESHR_Miss							= 0x02,		// δ����
		ESHR_Immune							= 0x04,		// ����
		ESHR_Absorb							= 0x08,		// ����
		ESHR_Reflect						= 0x10,		// ����
		ESHR_Evade							= 0x20,		// ���
	};

	enum ESpellHitType
	{
		ESHT_Spell							= 0,		// ����
		ESHT_Aura							= 1,		// buff
	};

	// �����ͷż���(size = 31)
	CMD_START(C2S_CastSpell)
		uint32			spell_id;			// �����ͷŵļ���id
		uint64			target_id;			// �ͷ�Ŀ��id, û��Ϊ0
		vector3			target_pos;			// �ͷ�Ŀ������, û�в�����
		float			time_stamp;			// ʱ���
		uint8			cast_count;			// 0 �����ͷ�, > 0 ʵ�ʴ���
	CMD_END

	// �ͷż��ܽ��(ֻ���͸�������)(size = 7)
	CMD_START(S2C_CastSpellResult)
		uint32			spell_id;			// �ͷ��ĸ�����
		uint8			result;				// ���
	CMD_END

	// ����ȡ���ͷż���(size = 2)
	CMD_START(C2S_CancelCast)
	CMD_END

	// ����׼��(����, �����������ܵ�����Ϣ)��ʼ�����(size = 26)
	CMD_START(S2C_SpellPrepare)
		uint64			caster_id;			// �ͷ���id
		uint32			spell_id;			// �ͷŵļ���
		float			time_stamp;			// ʱ���
		uint32			max_time;			// ׼�����ʱ��, ��λ1/1000s
		uint32			cur_time;			// ׼��ʣ��ʱ��, ��λ1/1000s

		// Ŀ��û���뵽�м���������ҪĿ��, �õ��Ļ��ڼ�
		//uint64			target_id;		// �ͷ�Ŀ��id
		//vector3			target_pos;		// �ͷ�Ŀ������
	CMD_END

	// �����ͷ�(˲��, ��������������)(size = 47)
	CMD_START(S2C_SpellCasting)
		uint64			caster_id;			// �ͷ���id
		uint32			spell_id;			// �ͷŵļ���
		float			time_stamp;			// ʱ���
		uint32			max_time;			// ˲��ʱΪ0, != 0 Ϊͨ�����ʱ��, ��λ1/1000s
		uint32			cur_time;			// ˲��ʱΪ0, != 0 Ϊͨ��ʣ��ʱ��, ��λ1/1000s
		uint64			target_id;			// �ͷ�Ŀ��id, ����жԶ��Ŀ��ļ���, ������µ���Ϣ, ��������, �Զ��Ŀ������
		vector3			target_pos;			// �ͷ�Ŀ������, ���ݼ���ģ����Ϣ, �ж���ʹ�����껹��target_id
		uint8			cast_index;			// �����ͷŵ�����
	CMD_END

	//int8			target_flag;	// 0��ʾ����ҪĿ����Ϣ, <0��ʾvector3 pos����, >0��ʾtarget_id�ĸ���
	//union
	//{
	//	struct
	//	{
	//		uint64	target_id[10];
	//	} target1;

	//	struct
	//	{
	//		float	x;
	//		float	y;
	//		float	z;
	//	} target2[10];
	//};

	// ����׼�����ͷŹ����д�ϵ�(��Ҫ�㲥)(size = 15)
	CMD_START(S2C_SpellFailed)
		uint64			caster_id;			// �ͷ���id
		uint32			spell_id;			// �ͷŵļ���
		uint8			error;				// ����ԭ��
	CMD_END

	// ����������Ϣ(��Ҫ�㲥)(size = 15)
	CMD_START(S2C_SpellHit)
		uint64			caster_id;			// ����˺������Ƶ����
		uint64			target_id;			// �����˺������Ƶ����
		uint32			info_id;			// �ĸ����ܻ�buff
		float			time_stamp;			// ʱ���
		int32			value;				// HP�˺���������ֵ(<0Ϊ�˺�, >0Ϊ����, =0Ϊδ����, ��ȫ����, ����, ����, ���)
		int32			absorb;				// �˺�������ֵ
		uint8			flag;				// ����, ����, δ����, ����, ����, ����, ���, ��ESpellHitResult
		uint8			type;				// 0=����, 1=buff, ��ESpellHitType
	CMD_END

	// ����ȡ��buff(size = 3)
	CMD_START(C2S_CancelAura)
		uint8			slot;				// buff��Ψһ��ʶ
	CMD_END

	// ���һ��buffʵ��
	CMD_START(S2C_AddAura)
		uint64			unit_id;			// ˭���
		naura_data		aura_data;			// buffʵ��
	CMD_END

	// ɾ��һ��buffʵ��
	CMD_START(S2C_RemoveAura)
		uint64			unit_id;			// ˭ɾ��
		uint8			slot;				// buff��Ψһ��ʶ
	CMD_END

	// buffʵ�����ݸ���
	CMD_START(S2C_UpdateAura)
		uint64			unit_id;			// buffӵ����id
		uint8			slot;				// buff��Ψһ��ʶ
		uint8			stack_count;		// ��ǰ���Ӵ���
		uint32			remain_time;		// ʣ��ʱ��(��λs)
		uint32			max_duration;		// ��ʱ��(��λs)
	CMD_END

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_SPELL_H_
