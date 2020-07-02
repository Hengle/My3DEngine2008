#pragma once

#include "nspell_define.h"
#include "nspell_data.h"


namespace NexusEngine
{
	// ��ս����ص�Native data��interop��װ
	public enum class SpellConstant : System::UInt32
	{
		MaxSpellNum		= MAX_SPELL_NUM,
		MaxAuraNum		= MAX_AURA_NUM,
		MaxCDNum		= MAX_CD_NUM,
	};

	// ref nspell_define.h nspell_data.h
	// ����������
	public enum class SpellType : System::UInt32
	{
		Null		= ESpell_Null,
		Class		= ESpell_Class,			// ְҵ����, ������ESpellClassSubType
		Profession	= ESpell_Profession,	// רҵ����, ������ESpellProfessionSubType
		Talent		= ESpell_Talent,		// �츳����
		Item		= ESpell_Item,			// ��Ʒ����
		Pet			= ESpell_Pet,			// ���＼��
		Creature	= ESpell_Creature,		// npc����
		Misc		= ESpell_Misc,			// δ���༼��
		Max			= ESpell_Max,
	};

	// ְҵ����
	public enum class SpellClassSubType : System::UInt32
	{
		Null	= ESCST_Null,
		Melee	= ESCST_Melee,			// ��ͨ����
		KungFu	= ESCST_KungFu,			// ���ռ���
		Spell	= ESCST_Spell,			// ��������
		Max		= ESCST_Max,
	};

	// �����, ����������(רҵ����)
	public enum class SpellProfessionSubType : System::UInt32
	{
		Null	= ESPST_Null,
		Mining	= ESPST_Mining,			// �ɿ�
		Max		= ESPST_Max,
	};

	// ���ܶ�������������
	public enum class SpellWeaponType : System::UInt32
	{
		Null		= ESWT_Null,			// ������
		MainHand	= ESWT_MainHand,		// ���ֱ���������Ʒ��������
		OffHand		= ESWT_OffHand,			// ���ֱ���������Ʒ��������
		MainAndOff	= ESWT_MainAndOff,		// �����ֶ�������Ʒ��������
		MainOrOff	= ESWT_MainOrOff,		// ����������һ��������Ʒ��������
		Max			= ESWT_Max,
	};

	// �ͷ�Ŀ������
	public enum class CastTargetType : System::UInt32
	{
		Null		= ECTT_Null,			// ��Ŀ��
		
		Self		= ECTT_Self,			// �Լ�
		SingleEnemy	= ECTT_SingleEnemy,		// ������ķ����Ƶ�λ
		SingleFriend= ECTT_SingleFriend,	// ����������Ƶ�λ
		EnemyCorpse	= ECTT_EnemyCorpse,		// ����������ʬ��
		FriendCorpse= ECTT_FriendCorpse,	// ��������ʬ��
		Point		= ECTT_Point,			// �����
		
		Max			= ECTT_Max,
	};

	// ����Ŀ������
	public enum class EffectTargetType : System::UInt32
	{
		Target					= EETT_Target,					// ѡ���ĵ�������
		AllEnemyInArea1			= EETT_AllEnemyInArea1,			// ѡ�������뾶Ϊradius��Բ�������ڷ����Ƶ�λ
		AllEnemyInArea2			= EETT_AllEnemyInArea2,			// ѡ���������Լ�λ�ñ�ʾ�ĳ���radius��angle����ʾ�Ļ��������ڷ����Ƶ�λ
		AllEnemyInArea3			= EETT_AllEnemyInArea3,			// ѡ���������Լ�λ�ñ�ʾ�ĳ���weight��length����ʾ���Լ������ľ��������ڷ����Ƶ�λ
		AllEnemyInArea4			= EETT_AllEnemyInArea4,			// ѡ�������Ϊ�����Һ��Լ�λ�ñ�ʾ�ĳ���weight��length����ʾ���Լ������ľ��������ڷ����Ƶ�λ
		
		AllEnemyAroundCaster	= EETT_AllEnemyAroundCaster,	// �ͷ�����ΧԲ����������Ƶ�λ
		AllFriendAroundCaster	= EETT_AllFriendAroundCaster,	// �ͷ�����ΧԲ���������Ƶ�λ

		Max						= EETT_Max,
	};

	// �ͷű��ַ�ʽ
	public enum class SpellCastType : System::UInt32
	{
		Null	= ESCast_Null,
		Instant	= ESCast_Instant,			// ˲��(can_repeat��ʾ�Ƿ����)
		Cast	= ESCast_Cast,				// ����
		Channel = ESCast_Channel,			// ����
		Max		= ESCast_Max,
	};

	// ���ܴ�Ϸ�ʽ
	[System::Flags]
	public enum class SpellInterruptFlags : System::UInt32
	{
		Null		= ESIF_Null,		// �����϶����
		Damage		= ESIF_Damage,		// �˺����
		Move		= ESIF_Move,		// �ƶ����
		Turning		= ESIF_Turning,		// ��ת���
		Interrupt	= ESIF_Interrupt,   // ���ܴ��
		Damage2		= ESIF_Damage2,		// �˺���ʧʱ��
		SpellPrepare= ESIF_SpellPrepare,// �����������
		SpellCast	= ESIF_SpellCast,   // �����ͷŴ��
	};

	// ��������
	public enum class SpellCostType : System::UInt32
	{
		Null			= ESCT_Null,			// ��Ч, ������
		EnergyValue		= ESCT_EnergyValue,		// ��ֵ��������
		ManaValue		= ESCT_ManaValue,		// ��ֵ������ʶ
		EnergyPct		= ESCT_EnergyPct,		// �����ֵ�İٷֱ���������
		ManaPct			= ESCT_ManaPct,			// �����ֵ�İٷֱ�������ʶ
		ManaPerLevel	= ESCT_ManaPerLevel,	// ÿһ�����ĵ���ʶֵ
		EnergyPerLevel	= ESCT_EnergyPerLevel,	// ÿһ�����ĵ�����ֵ
		ManaAndItem		= ESCT_ManaAndItem,		// ��ʶֵ����Ʒ
		EnergyAndItem	= ESCT_EnergyAndItem,	// ����ֵ����Ʒ
		Item			= ESCT_Item,			// ��Ʒ
		Max				= ESCT_Max,
	};

	// ����Ч����Ч�¼�(����)
	public enum class SpellEffectTriggerType : System::UInt32
	{
		Null			= ESETT_Null,
		SpellPrepare	= ESETT_SpellPrepare,		// ׼��
		SpellCasting	= ESETT_SpellCasting,		// �ͷ�
		SpellInterrupt	= ESETT_SpellInterrupt,		// �������
		HitTarget		= ESETT_HitTarget,			// ����Ŀ��
		Crit			= ESETT_Crit,				// ����
		EffectDead		= ESETT_EffectDead,			// Ч������
		Max				= ESETT_Max,
	};

	// ����Ч�����ܸ�˭
	public enum class SpellTriggerTargetType : System::UInt32
	{
		Null		= ESTTT_Null,
		Targets		= ESTTT_Targets,		// ����Ŀ�꼯�����г�Ա
		Caster		= ESTTT_Caster,			// �����ͷ���
		CasterParty	= ESTTT_CasterParty,	// �����ͷ��ߵ�С�ӳ�Ա
		
		Max			= ESTTT_Max,
	};

	// ����Ч��ö��
	public enum class SpellEffectType : System::UInt32
	{
		Null			= ESET_Null,			// ��Ч��
		
		TriggerSpell	= ESET_TriggerSpell,	// ��������
		ApplyAura		= ESET_ApplyAura,		// Ӧ��buff��չ����Ч��
		Instantkill		= ESET_Instantkill,		// ��ɱ
		MeleeDamage		= ESET_MeleeDamage,		// ��ͨ�˺�
		KungFuDamage	= ESET_KungFuDamage,	// �����˺�
		SpellDamage		= ESET_SpellDamage,		// �����˺�
		Heal			= ESET_Heal,			// ����
		ModPower		= ESET_ModPower,		// �޸�����ֵ, (����, ��ʶ)
		HealPct			= ESET_HealPct,			// ���ٷֱ�ֱ������
		ModPowerPct		= ESET_ModPowerPct,		// ���ٷֱ��޸�����ֵ, (����, ��ʶ)
		Dispel			= ESET_Dispel,			// ��ɢ
		Resurrect		= ESET_Resurrect,		// ����
		KnockDown		= ESET_KnockDown,		// ����
		KnockBack		= ESET_KnockBack,		// ���˻���
		
		Max				= ESET_Max,
	};

	// �˺�����
	public enum class SpellDMGType : System::UInt32
	{
		Null	= ESDT_Null,
		Physics	= ESDT_Physics,			// ����(����, ����)�����˺�����, ����
		Metal	= ESDT_Metal,			// ��, �����˺�������
		Wood	= ESDT_Wood,			// ľ, �����˺�ľ����
		Water	= ESDT_Water,			// ˮ, �����˺�ˮ����
		Fire	= ESDT_Fire,			// ��, �����˺�������
		Soil	= ESDT_Soil,			// ��, �����˺�������
		Max		= ESDT_Max,
	};

	// buffЧ��ö��
	public enum class AuraEffectType : System::UInt32
	{
		Null				= EAET_Null,				// ��Ч��
		
		ModStatBaseValue	= EAET_ModStatBaseValue,	// base_value��ʽ�޸�����ֵ��stat_index, stat_value
		ModStatBasePct		= EAET_ModStatBasePct,		// base_pct��ʽ�޸�����ֵ��stat_index, stat_value
		ModStatTotalValue	= EAET_ModStatTotalValue,	// total_value��ʽ�޸�����ֵ��stat_index, stat_value
		ModStatTotalPct		= EAET_ModStatTotalPct,		// total_pct��ʽ�޸�����ֵ��stat_index, stat_value
		
		PeriodicDamage		= EAET_PeriodicDamage,		// �����˺�
		PeriodicHeal		= EAET_PeriodicHeal,		// ��������
		Fear				= EAET_Fear,				// �־�
		Silence				= EAET_Silence,				// ��Ĭ
		IncreaseSpeed		= EAET_IncreaseSpeed,		// ����
		DecreaseSpeed		= EAET_DecreaseSpeed,		// ����
		Root				= EAET_Root,				// ����
		
		SpellMod			= EAET_SpellMod,			// �츳�޸ļ���
		TriggerSpell		= EAET_TriggerSpell,		// ��������
		TriggerAura			= EAET_TriggerAura,			// ����buff
		
		Max					= EAET_Max,
	};


	// buff��Ϸ�ʽ(ʱ�����)
	[System::Flags]
	public enum class AuraInterruptFlags : System::UInt32
	{
		Null		= EAIF_Null,			// �����
		Move		= EAIF_Move,			// �ƶ�
		EnterCombat	= EAIF_EnterCombat,		// ����ս��״̬
		Cancel		= EAIF_Cancel,			// �ֶ�ȡ��
		Damage		= EAIF_Damage,			// �ܵ��κ��˺�
		Dead		= EAIF_Dead,			// ����
		ChangeMap	= EAIF_ChangeMap,		// �뿪��ǰ��������
		OffLine		= EAIF_OffLine,			// ��ɫ���߻򷵻ؽ�ɫ�б�
		Dispel		= EAIF_Dispel,			// ��ɢ
		Replace		= EAIF_Replace,			// ��Ч�����ߵ�buff����
		Mounting	= EAIF_Mounting,		// ����
		Damage2		= EAIF_Damage2,			// �ܵ�һ�����˺�
		Max			= EATF_Max,
	};

	// ����ʱ��˥������
	public enum class AuraDiminishingGroup : System::UInt32
	{
		Null	= EADG_Null,			// ��˥��
		Silence	= EADG_Silence,			// ��Ĭ˥��
		Root	= EADG_Root,			// ����˥��
		Fear	= EADG_Fear,			// �־�˥��
		Max		= EADG_Max,
	};

	// buff���ʱ���ж��Ƿ���id��ͬ��buff, id��ͬ��ͬԴ��ͬԴ���ӹ���id����ͬ�߳嶥����
	// �嶥����
	public enum class AuraReplaceType : System::UInt32
	{
		Null	= EART_Null,			// ������������buff�嶥

		Max		= EART_Max,
	};

	// buff���ӷ�ʽ(ͬԴ, ��ͬԴ)
	public enum class AuraStackType : System::UInt32
	{
		Null		= EAST_Null,
		Refresh		= EAST_Refresh,			// ����, (ʱ��, ����)
		AddCount	= EAST_AddCount,		// ���ʣ�����, ����ʱ��
		Add			= EAST_Add,				// ��Ӳ�Ӱ������buff
		CantAdd		= EAST_CantAdd,			// ������, �����
		Max			= EAST_Max,
	};

	// ����buff����ʧ��ʱ��ʽ
	public enum class AuraDeadTimerType : System::UInt32
	{
		Null		= EADTT_Null,			// ��������ʧ��д��ֵ
		Stop		= EADTT_Stop,			// �������ʱֹͣ
		Normal		= EADTT_Normal,			// ����������ֹͣ
		Max			= EADTT_Max,
	};

	// ����buff����ʧ��ʱ��ʽ
	public enum class AuraOfflineTimerType : System::UInt32
	{
		Null	= EAOTT_Null,			// ���ߺ���ʧ��д��ֵ
		Stop	= EAOTT_Stop,			// ���ߺ��ʱֹͣ
		Normal	= EAOTT_Normal,			// ���ߺ�����ֹͣ
		Max		= EAOTT_Max,
	};

	// �츳�ı似��ö��
	public enum class SpellModType : System::UInt32
	{
		Null						= ESMT_Null,						// ��Ч, ������
		SpellCastingTime			= ESMT_SpellCastingTime,			// Ӱ�켼������ʱ��
		SpellRange					= ESMT_SpellRange,					// Ӱ�켼��������
		SpellSpeed					= ESMT_SpellSpeed,					// Ӱ�켼�ܷ����ٶ�
		SpellRadius					= ESMT_SpellRadius,					// Ӱ�켼�ܰ뾶
		SpellAngle					= ESMT_SpellAngle,					// Ӱ�켼�ܰ뾶�Ƕ�
		SpellChainTarget			= ESMT_SpellChainTarget,			// Ӱ������������
		SpellDuration				= ESMT_SpellDuration,				// Ӱ�����ʱ��
		SpellCost					= ESMT_SpellCost,					// Ӱ�켼������ֵ
		SpellCostScale				= ESMT_SpellCostScale,				// Ӱ�켼��������������
		SpellModEffect				= ESMT_SpellModEffect,				// �滻����Ч��
		ModEffectTriggerSpell		= ESMT_ModEffectTriggerSpell,		// �滻��������
		ModEffectApplyAura			= ESMT_ModEffectApplyAura,			// �滻Ӧ�õ�buff
		ModTriggerSpellRate			= ESMT_ModTriggerSpellRate,			// Ӱ�켼��Trigger��������
		SpellEffectValue			= ESMT_SpellEffectValue,			// Ӱ�켼��effect��ֵ
		
		AuraMaxDuration				= ESMT_AuraMaxDuration,				// Ӱ��buff����ʱ��
		AuraTick					= ESMT_AuraTick,					// Ӱ��buff tick��ʽ
		AuraPeriodicTime			= ESMT_AuraPeriodicTime,			// Ӱ��buff tick����
		AuraResistDispelChance		= ESMT_AuraResistDispelChance,		// �ı�Buff��ɢ�ȼ�
		AuraStack					= ESMT_AuraStack,					// Ӱ��buff���Ӵ���
		AuraEffectValue				= ESMT_AuraEffectValue,				// Ӱ��buff effect��ֵ
		
		Max							= ESMT_Max,
	};

	public enum class SpellModSubType : System::UInt32
	{
		Null	= ESMST_Null,
		Value	= ESMST_Value,			// ����ֵ�޸�
		Pct		= ESMST_Pct,			// ���ٷֱ��޸�
		Max		= ESMST_Max,
	};	

	// buff�������¼�����(�������buff������, ����Խ�ɫ)
	public enum class AuraTriggerEventType : System::UInt32
	{
		Null			= EATET_Null,
		OnHited			= EATET_OnHited,			// buff�����߱�����
		OnDead			= EATET_OnDead,				// buff����������
		OnCasting		= EATET_OnCasting,			// buff�������ͷż���
		Ondispeled		= EATET_Ondispeled,			// buff����ɢʱ
		Max				= EATET_Max,
	};

	// buff�������ܸ�˭
	public enum class AuraTriggerTargetType : System::UInt32
	{
		Null		= EATTT_Null,
		Self		= EATTT_Self,			// buff������
		Caster		= EATTT_Caster,			// buff�ͷ���
		Party		= EATTT_Party,			// buff�����ߵ�С�ӳ�Ա
		CasterParty	= EATTT_CasterParty,	// buff�ͷ��ߵ�С�ӳ�Ա
		Attacker	= EATTT_Attacker,		// ������
		
		Max			= EATTT_Max,
	};

	/// 
	public enum class SpellCategoryCDType : System::UInt32
	{
		Null		= ESCCDT_Null,
		GlobalCD	= ESCCDT_GlobalCD,		// ȫ�ֹ���cd
		Max			= ESCCDT_Max,
	};
}