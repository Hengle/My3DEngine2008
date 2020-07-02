/**
 *	nexus ndef_game - nobject_fields
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NOBJECT_FIELDS_H_
#define _NDEF_GAME_NOBJECT_FIELDS_H_

namespace nexus {

	// ��ҪΪս������
	enum EUnitFields
	{
		EUF_UnitNull							= 0,	// 1, uint32, ��
		EUF_UnitStamina							= 1,	// 1, uint32, ����
		EUF_UnitVitality						= 2,	// 1, uint32, ����
		EUF_UnitSpirit							= 3,	// 1, uint32, ��ʶ
		EUF_UnitGenGu							= 4,	// 1, uint32, ����

		EUF_UnitMaxHealth						= 5,	// 1, uint32, ��������
		EUF_UnitMaxEnergy						= 6,	// 1, uint32, ��������
		EUF_UnitMaxMana							= 7,	// 1, uint32, ��������
		EUF_UnitMeleeDamage 					= 8,	// 1, float, ����
		EUF_UnitKungFuDamage 					= 9,	// 1, float, ����
		EUF_UnitSpellDamage						= 10,	// 1, float, ����
		EUF_UnitAgility							= 11,	// 1, float, ����
		EUF_UnitHealthRegeneration				= 12,	// 1, float, ����
		EUF_UnitPowerRegeneration				= 13,	// 1, float, ����

		EUF_UnitDodge							= 14,	// 1, float, ����
		EUF_UnitCrit							= 15,	// 1, float, ����
		EUF_UnitSpeed							= 16,	// 1, float, �ƶ��ٶ�
		EUF_UnitAttackSpeed						= 17,	// 1, float, �����ٶ�

		EUF_UnitTotalWeight						= 18,	// 1, uint32, ȫ��װ������
		EUF_UnitSharp							= 19,	// 1, uint32, ����
		EUF_UnitDefense							= 20,	// 1, uint32, ����
		EUF_UnitResilience						= 21,	// 1, uint32, ����
		EUF_UnitMetalResist						= 22,	// 1, uint32, ������
		EUF_UnitWoodResist						= 23,	// 1, uint32, Ŀ����
		EUF_UnitWaterResist						= 24,	// 1, uint32, ˮ����
		EUF_UnitFireResist						= 25,	// 1, uint32, ������
		EUF_UnitSoilResist						= 26,	// 1, uint32, ������

		EUF_UnitSeparator						= 27,	// �����ķָ���

		EUF_UnitHealth							= 0 + EUF_UnitSeparator,	// 1, uint32, ��ǰ����
		EUF_UnitEnergy							= 1 + EUF_UnitSeparator,	// 1, uint32, ��ǰ����
		EUF_UnitMana							= 2 + EUF_UnitSeparator,	// 1, uint32, ��ǰ����
		EUF_UnitLevel							= 3 + EUF_UnitSeparator,	// 1, uint32, �ȼ�
		EUF_UnitFlags							= 4 + EUF_UnitSeparator,	// 1, uint32, ״̬��־λ
		EUF_UnitEnd								= 5 + EUF_UnitSeparator,	// end
	};

	enum EPlayerFields
	{
		EPF_PlayerBaseStamina					= 0 + EUF_UnitEnd,		// 1, uint32, �Ѽ����ǵ�
		EUF_PlayerBaseVitality					= 1 + EUF_UnitEnd,		// 1, uint32, �ѼӾ�����
		EUF_PlayerBaseSpirit					= 2 + EUF_UnitEnd,		// 1, uint32, �Ѽ���ʶ��
		EUF_PlayerBaseGenGu						= 3 + EUF_UnitEnd,		// 1, uint32, �ѼӸ��ǵ�
		EPF_PlayerReserveStat					= 4 + EUF_UnitEnd,		// 1, uint32, ʣ��������Ե�
		EPF_PlayerExp							= 5 + EUF_UnitEnd,		// 2, uint64, ����
		EPF_PlayerMoney							= 7 + EUF_UnitEnd,		// 2, uint64, ��Ϸ��
		EPF_PlayerMoneyBind						= 9 + EUF_UnitEnd,		// 2, uint64, ����Ϸ��
		EPF_PlayerEnd							= 11 + EUF_UnitEnd
	};

	enum EUnitSeparator
	{
		EUS_StatStart							= EUF_UnitStamina,
		EUS_StatEnd								= EUF_UnitGenGu + 1,
		EUS_PowerStart							= EUF_UnitMaxHealth,
		EUS_PowerEnd							= EUF_UnitMaxMana + 1,
		EUS_EquipStart							= EUF_UnitSharp,
		EUS_EquipEnd							= EUF_UnitSoilResist + 1,
	};


} // namespace nexus

#endif // _NDEF_GAME_NOBJECT_FIELDS_H_
