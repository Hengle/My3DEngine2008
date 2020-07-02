#pragma once
#include "nobject_fields.h"
#include "nobject_define.h"

namespace NexusEngine
{
	// �Խ�ɫ����ö�ٶ����Interop��װ

	public ref class NCharacterFieldsHelper
	{
	public:
		// IS_PLAYER
		static bool IsPlayer(System::Int64 guid);

		// IS_CREATURE
		static bool IsCreature(System::Int64 guid);
	};

	/**
	* ��Ϸ�����ж�������ö�٣�64λguid
	*/
	public enum class ObjectType : System::UInt32
	{
		Player		= EOT_PLAYER,
		Creature	= EOT_CREATURE,

		Item		= EOT_ITEM,
	};

	// ����
	public enum class RaceType : System::UInt32
	{
		None	= ERT_None,				// ����֮��
		Human	= ERT_Human,			// ��
		Fairy	= ERT_Fairy,			// ��
		Goblin	= ERT_Goblin,			// ��
		Ghost	= ERT_Ghost,			// ��
		Deity	= ERT_Deity,			// ��
		Demon	= ERT_Demon,			// ħ
		Max		= ERT_Max,
	};

	// �����޸ķ�ʽ
	public enum class UnitModifierType : System::UInt32
	{
		BaseValue	= EUMT_BaseValue,			// ����ֵֵ�ĸı�
		BasePct		= EUMT_BasePct,				// ����ֵ�ٷֱȵĸı�
		TotalValue	= EUMT_TotalValue,			// ��ֵֵ�ĸı�
		TotalPct	= EUMT_TotalPct,			// ��ֵ�ٷֱȵĸı�
		Max			= EUMT_Max,
	};


	// unit��״̬��־λ, ��buff�ܱ�ʾ��״̬
	[System::Flags]
	public enum class UnitStatus : System::UInt32
	{
		Normal		= EUS_Normal,		// �ճ�״̬
		InCombat	= EUS_InCombat,		// ս��״̬
		Dead		= EUS_Dead,			// ����״̬
		CycleCasting= EUS_CycleCasting,	// �����ͷ�״̬
		Trading		= EUS_Trading,		// ����״̬
	};

	// native define ref nobject_fields.h 
	public enum class UnitFields : System::UInt32
	{
		Null		= EUF_UnitNull,				// 1, uint32, ��
		Stamina		= EUF_UnitStamina,			// 1, uint32, ����
		Vitality	= EUF_UnitVitality,			// 1, uint32, ����
		Spirit		= EUF_UnitSpirit,			// 1, uint32, ��ʶ
		GenGu		= EUF_UnitGenGu,			// 1, uint32, ����

		MaxHealth	= EUF_UnitMaxHealth,		// 1, uint32, ��������
		MaxEnergy	= EUF_UnitMaxEnergy,		// 1, uint32, ��������
		MaxMana		= EUF_UnitMaxMana,			// 1, uint32, ��������
		MeleeDamage	= EUF_UnitMeleeDamage,		// 1, float, ����
		KungFuDamage= EUF_UnitKungFuDamage,		// 1, float, ����
		SpellDamage	= EUF_UnitSpellDamage,		// 1, float, ����
		Agility		= EUF_UnitAgility,			// 1, float, ����
		HealthRegeneration	= EUF_UnitHealthRegeneration,	// 1, float, ����
		PowerRegeneration	= EUF_UnitPowerRegeneration,	// 1, float, ����

		Dodge		= EUF_UnitDodge,			// 1, float, ����
		Crit		= EUF_UnitCrit,				// 1, float, ����
		Speed		= EUF_UnitSpeed,			// 1, float, �ƶ��ٶ�
		AttackSpeed	= EUF_UnitAttackSpeed,		// 1, float, �����ٶ�

		TotalWeight = EUF_UnitTotalWeight,		// 1, uint32, ȫ��װ������
		Sharp		= EUF_UnitSharp,			// 1, uint32, ����
		Defense		= EUF_UnitDefense,			// 1, uint32, ����
		Resilience	= EUF_UnitResilience,		// 1, uint32, ����
		MetalResist	= EUF_UnitMetalResist,		// 1, uint32, ������
		WoodResist	= EUF_UnitWoodResist,		// 1, uint32, Ŀ����
		WaterResist	= EUF_UnitWaterResist,		// 1, uint32, ˮ����
		FireResist	= EUF_UnitFireResist,		// 1, uint32, ������
		SoilResist	= EUF_UnitSoilResist,		// 1, uint32, ������

		Health		= EUF_UnitHealth,			// 1, uint32, ��ǰ����
		Energy		= EUF_UnitEnergy,			// 1, uint32, ��ǰ����
		Mana		= EUF_UnitMana,				// 1, uint32, ��ǰ����
		Level		= EUF_UnitLevel,			// 1, uint32, �ȼ�
		Flags		= EUF_UnitFlags,			// 1, uint32, ״̬��־λ
		
		End			= EUF_UnitEnd,				// end
	};

	public enum class PlayerFields : System::UInt16
	{
		BaseStamina		= EPF_PlayerBaseStamina,		// 1, uint32, �Ѽ����ǵ�
		BaseVitality	= EUF_PlayerBaseVitality,		// 1, uint32, �ѼӾ�����
		BaseSpirit		= EUF_PlayerBaseSpirit,			// 1, uint32, �Ѽ���ʶ��
		BaseGenGu		= EUF_PlayerBaseGenGu,			// 1, uint32, �ѼӸ��ǵ�
		ReserveStat		= EPF_PlayerReserveStat,		// 1, uint32, ʣ��������Ե�
		Exp				= EPF_PlayerExp,				// 2, uint64, ����
		Money			= EPF_PlayerMoney,				// 2, uint64, ��Ϸ��
		MoneyBind		= EPF_PlayerMoneyBind,			// 2, uint64, ����Ϸ��
		End				= EPF_PlayerEnd					// end
	};

	public enum class FieldsSeparator : System::UInt32
	{
		StatStart	= EUS_StatStart,
		StatEnd		= EUS_StatEnd,
		PowerStart	= EUS_PowerStart,
		PowerEnd	= EUS_PowerEnd,
		EquipStart	= EUS_EquipStart,
		EquipEnd	= EUS_EquipEnd,
	};
	

}
