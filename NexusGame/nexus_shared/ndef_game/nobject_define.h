/**
 *	nexus ndef_game - nobject_define
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NOBJECT_DEFINE_H_
#define _NDEF_GAME_NOBJECT_DEFINE_H_

#include "ntype_def.h"

namespace nexus {

	/**
	 * ��Ϸ�����ж�������ö�٣�64λguid
	 */
	enum EObjectType
	{
		EOT_PLAYER				= 0x01,
		EOT_CREATURE			= 0x02,


		EOT_ITEM				= 0x80,
	};

#define MAKE_PAIR64(l, h)				uint64(uint32(l) | (uint64(h) << 32))
#define PAIR64_HIPART(x)				(uint32)((uint64(x) >> 32) & 0x00000000FFFFFFFFui64)
#define PAIR64_LOPART(x)				(uint32)(uint64(x)         & 0x00000000FFFFFFFFui64)

#define MAKE_PAIR32(l, h)				uint32(uint16(l) | (uint32(h) << 16))
#define PAIR32_HIPART(x)				(uint16)((uint32(x) >> 16) & 0x0000FFFF)
#define PAIR32_LOPART(x)				(uint16)(uint32(x)         & 0x0000FFFF)

// g - 32 guid
// r - 24 realm ����ս������Ϣ
// t - 8  type  ������Ϣ
#define MAKE_OBJECT_GUID(g, r, t)		uint64(uint64(g) | (uint64(r) << 32) | (uint64(t) << 56))
#define GET_TYPE(guid)					(uint8)(guid >> 56)
#define	IS_PLAYER(guid)					((GET_TYPE(guid) == EOT_PLAYER))
#define	IS_CREATURE(guid)				((GET_TYPE(guid) == EOT_CREATURE))

	const uint32 MAX_NAME_LEN = 8;			// ������ֳ���
	const uint32 REGEN_TIME			= 2000;				// �ָ�����ʱ����1/1000s

	// ����
	enum ERaceType
	{
		ERT_None				= 0,			// ����֮��
		ERT_Human				= 1,			// ��
		ERT_Fairy				= 2,			// ��
		ERT_Goblin				= 3,			// ��
		ERT_Ghost				= 4,			// ��
		ERT_Deity				= 5,			// ��
		ERT_Demon				= 6,			// ħ
		ERT_Max					= 7,
	};

	// ְҵ
	enum EClassType
	{
		ECT_None				= 0,			// 
		ECT_XiaShi				= 1,			// ��ʿ
		ECT_JianXian			= 2,			// ����
		ECT_LingYao				= 3,			// ����
		ECT_ZheXian				= 4,			// ����
		ECT_Max					= 5,
	};

	// �����޸ķ�ʽ
	enum EUnitModifierType
	{
		EUMT_BaseValue			= 0,			// ����ֵֵ�ĸı�
		EUMT_BasePct			= 1,			// ����ֵ�ٷֱȵĸı�
		EUMT_TotalValue			= 2,			// ��ֵֵ�ĸı�
		EUMT_TotalPct			= 3,			// ��ֵ�ٷֱȵĸı�
		EUMT_Max				= 4,
	};

	// unit��״̬��־λ, ��buff�ܱ�ʾ��״̬
	enum EUnitStatus
	{
		EUS_Normal				= 0x00000000,	// �ճ�״̬
		EUS_InCombat			= 0x00000001,	// ս��״̬
		EUS_Dead				= 0x00000002,	// ����״̬
		EUS_CycleCasting		= 0x00000004,	// �����ͷ�״̬
		EUS_Trading				= 0x00000008,	// ����״̬

	};

} // namespace nexus

#endif // _NDEF_GAME_NOBJECT_DEFINE_H_
