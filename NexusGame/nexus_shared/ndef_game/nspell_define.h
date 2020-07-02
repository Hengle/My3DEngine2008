/**
 *	nexus ngame_define - nspell_define
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDEF_GAME_NSPELL_DEFINE_H_
#define _NDEF_GAME_NSPELL_DEFINE_H_

#include "ntype_def.h"
#include "object/nname.h"
#include "ndef_base.h"

namespace nexus {

#pragma pack(push,1)

	const uint32 MAX_EFFECT_NUM						= 3;			// ���ܸ���Ч���������Ŀ
	const uint32 MAX_AURA_STAT_NUM					= 4;			// buffӰ����������������Ŀ
	const uint32 MAX_SPELL_TALENT_RANK				= 10;			// �����츳��ߵȼ�

	// ����������
	enum ESpellType
	{
		ESpell_Null									= 0,
		ESpell_Class								= 1,			// ְҵ����, ������ESpellClassSubType
		ESpell_Profession							= 2,			// רҵ����, ������ESpellProfessionSubType
		ESpell_Talent								= 3,			// �츳����
		ESpell_Item									= 4,			// ��Ʒ����
		ESpell_Pet									= 5,			// ���＼��
		ESpell_Creature								= 6,			// npc����
		ESpell_Misc									= 7,			// δ���༼��
		ESpell_Max,
	};

	// ְҵ����
	enum ESpellClassSubType
	{
		ESCST_Null									= 0,
		ESCST_Melee									= 1,			// ��ͨ����
		ESCST_KungFu								= 2,			// ���ռ���
		ESCST_Spell									= 3,			// ��������
		ESCST_Max,
	};

	// �����, ����������(רҵ����)
	enum ESpellProfessionSubType
	{
		ESPST_Null									= 0,
		ESPST_Mining								= 1,			// �ɿ�
		ESPST_Max,
	};

	// ���ܶ�������������
	enum ESpellWeaponType
	{
		ESWT_Null									= 0,			// ������
		ESWT_MainHand								= 1,			// ���ֱ���������Ʒ��������
		ESWT_OffHand								= 2,			// ���ֱ���������Ʒ��������
		ESWT_MainAndOff								= 3,			// �����ֶ�������Ʒ��������
		ESWT_MainOrOff								= 4,			// ����������һ��������Ʒ��������
		ESWT_Max,
	};

	// �ͷ�Ŀ������
	enum ECastTargetType
	{
		ECTT_Null									= 0,			// ��Ŀ��

		ECTT_Self									= 1,			// �Լ�
		ECTT_SingleEnemy							= 2,			// ������ķ����Ƶ�λ
		ECTT_SingleFriend							= 3,			// ����������Ƶ�λ
		ECTT_EnemyCorpse							= 4,			// ����������ʬ��
		ECTT_FriendCorpse							= 5,			// ��������ʬ��
		ECTT_Point									= 6,			// �����

		ECTT_Max,
	};

	// ����Ŀ������
	enum EEffectTargetType
	{
		EETT_Target									= 0,			// ѡ���ĵ�������
		EETT_AllEnemyInArea1						= 1,			// ѡ�������뾶Ϊradius��Բ�������ڷ����Ƶ�λ
		EETT_AllEnemyInArea2						= 2,			// ѡ���������Լ�λ�ñ�ʾ�ĳ���radius��angle����ʾ�Ļ��������ڷ����Ƶ�λ
		EETT_AllEnemyInArea3						= 3,			// ѡ���������Լ�λ�ñ�ʾ�ĳ���weight��length����ʾ���Լ������ľ��������ڷ����Ƶ�λ
		EETT_AllEnemyInArea4						= 4,			// ѡ�������Ϊ�����Һ��Լ�λ�ñ�ʾ�ĳ���weight��length����ʾ���Լ������ľ��������ڷ����Ƶ�λ

		EETT_AllEnemyAroundCaster					= 5,			// �ͷ�����ΧԲ����������Ƶ�λ
		EETT_AllFriendAroundCaster					= 6,			// �ͷ�����ΧԲ���������Ƶ�λ

		EETT_Max,
	};

	// �ͷű��ַ�ʽ
	enum ESpellCastType
	{
		ESCast_Null									= 0,
		ESCast_Instant								= 1,			// ˲��(can_repeat��ʾ�Ƿ����)
		ESCast_Cast									= 2,			// ����
		ESCast_Channel								= 3,			// ����
		ESCast_Max,
	};

	// ���ܴ�Ϸ�ʽ
	enum ESpellInterruptFlags
	{
		ESIF_Null									= 0x00000000,	// �����϶����
		ESIF_Damage									= 0x00000001,   // �˺����
		ESIF_Move									= 0x00000002,   // �ƶ����
		ESIF_Turning								= 0x00000004,   // ��ת���
		ESIF_Interrupt								= 0x00000008,   // ���ܴ��
		ESIF_Damage2								= 0x00000010,   // �˺���ʧʱ��
		ESIF_SpellPrepare							= 0x00000020,   // �����������
		ESIF_SpellCast								= 0x00000040,   // �����ͷŴ��
	};

	// ��������
	enum ESpellCostType
	{
		ESCT_Null									= 0,			// ��Ч, ������
		ESCT_EnergyValue							= 1,			// ��ֵ��������
		ESCT_ManaValue								= 2,			// ��ֵ������ʶ
		ESCT_EnergyPct								= 3,			// �����ֵ�İٷֱ���������
		ESCT_ManaPct								= 4,			// �����ֵ�İٷֱ�������ʶ
		ESCT_ManaPerLevel							= 5,			// ÿһ�����ĵ���ʶֵ
		ESCT_EnergyPerLevel							= 6,			// ÿһ�����ĵ�����ֵ
		ESCT_ManaAndItem							= 7,			// ��ʶֵ����Ʒ
		ESCT_EnergyAndItem							= 8,			// ����ֵ����Ʒ
		ESCT_Item									= 9,			// ��Ʒ
		ESCT_Max,
	};

	// ����Ч����Ч�¼�(����)
	enum ESpellEffectTriggerType
	{
		ESETT_Null									= 0,
		ESETT_SpellPrepare							= 1,			// ׼��
		ESETT_SpellCasting							= 2,			// �ͷ�
		ESETT_SpellInterrupt						= 3,			// �������
		ESETT_HitTarget								= 4,			// ����Ŀ��
		ESETT_Crit									= 5,			// ����
		ESETT_EffectDead							= 6,			// Ч������
		ESETT_Max,
	};

	// ����Ч�����ܸ�˭
	enum ESpellTriggerTargetType
	{
		ESTTT_Null									= 0,
		ESTTT_Targets								= 1,			// ����Ŀ�꼯�����г�Ա
		ESTTT_Caster								= 2,			// �����ͷ���
		ESTTT_CasterParty							= 3,			// �����ͷ��ߵ�С�ӳ�Ա

		ESTTT_Max,
	};

	// ����Ч��ö��
	enum ESpellEffectType
	{
		ESET_Null									= 0,			// ��Ч��

		ESET_TriggerSpell							= 1,			// ��������
		ESET_ApplyAura								= 2,			// Ӧ��buff��չ����Ч��
		ESET_Instantkill							= 3,			// ��ɱ
		ESET_MeleeDamage							= 4,			// ��ͨ�˺�
		ESET_KungFuDamage							= 5,			// �����˺�
		ESET_SpellDamage							= 6,			// �����˺�
		ESET_Heal									= 7,			// ����
		ESET_ModPower								= 8,			// �޸�����ֵ, (����, ��ʶ)
		ESET_HealPct								= 9,			// ���ٷֱ�ֱ������
		ESET_ModPowerPct							= 10,			// ���ٷֱ��޸�����ֵ, (����, ��ʶ)
		ESET_Dispel									= 11,			// ��ɢ
		ESET_Resurrect								= 12,			// ����
		ESET_KnockDown								= 13,			// ����
		ESET_KnockBack								= 14,			// ���˻���

		ESET_Max,
	};

	// �˺�����
	enum ESpellDMGType
	{
		ESDT_Null									= 0,
		ESDT_Physics								= 1,			// ����(����, ����)�����˺�����, ����
		ESDT_Metal									= 2,			// ��, �����˺�������
		ESDT_Wood									= 3,			// ľ, �����˺�ľ����
		ESDT_Water									= 4,			// ˮ, �����˺�ˮ����
		ESDT_Fire									= 5,			// ��, �����˺�������
		ESDT_Soil									= 6,			// ��, �����˺�������
		ESDT_Max,
	};

	// buffЧ��ö��
	enum EAuraEffectType
	{
		EAET_Null									= 0,			// ��Ч��

		EAET_ModStatBaseValue						= 1,			// base_value��ʽ�޸�����ֵ��stat_index, stat_value
		EAET_ModStatBasePct							= 2,			// base_pct��ʽ�޸�����ֵ��stat_index, stat_value
		EAET_ModStatTotalValue						= 3,			// total_value��ʽ�޸�����ֵ��stat_index, stat_value
		EAET_ModStatTotalPct						= 4,			// total_pct��ʽ�޸�����ֵ��stat_index, stat_value

		EAET_PeriodicDamage							= 5,			// �����˺�
		EAET_PeriodicHeal							= 6,			// ��������
		EAET_Fear									= 7,			// �־�
		EAET_Silence								= 8,			// ��Ĭ
		EAET_IncreaseSpeed							= 9,			// ����
		EAET_DecreaseSpeed							= 10,			// ����
		EAET_Root									= 11,			// ����

		EAET_SpellMod								= 12,			// �츳�޸ļ���
		EAET_TriggerSpell							= 13,			// ��������
		EAET_TriggerAura							= 14,			// ����buff

		EAET_Max,
	};

	// buff��Ϸ�ʽ(ʱ�����)
	enum EAuraInterruptFlags
	{
		EAIF_Null									= 0x00000000,	// �����
		EAIF_Move									= 0x00000001,	// �ƶ�
		EAIF_EnterCombat							= 0x00000002,	// ����ս��״̬
		EAIF_Cancel									= 0x00000004,	// �ֶ�ȡ��
		EAIF_Damage									= 0x00000008,	// �ܵ��κ��˺�
		EAIF_Dead									= 0x00000010,	// ����
		EAIF_ChangeMap								= 0x00000020,	// �뿪��ǰ��������
		EAIF_OffLine								= 0x00000040,	// ��ɫ���߻򷵻ؽ�ɫ�б�
		EAIF_Dispel									= 0x00000080,	// ��ɢ
		EAIF_Replace								= 0x00000100,	// ��Ч�����ߵ�buff����
		EAIF_Mounting								= 0x00000200,	// ����
		EAIF_Damage2								= 0x00000400,	// �ܵ�һ�����˺�
		EATF_Max,
	};

	// ����ʱ��˥������
	enum EAuraDiminishingGroup
	{
		EADG_Null									= 0,			// ��˥��
		EADG_Silence								= 1,			// ��Ĭ˥��
		EADG_Root									= 2,			// ����˥��
		EADG_Fear									= 3,			// �־�˥��
		EADG_Max,
	};

	// buff���ʱ���ж��Ƿ���id��ͬ��buff, id��ͬ��ͬԴ��ͬԴ���ӹ���id����ͬ�߳嶥����
	// �嶥����
	enum EAuraReplaceType
	{
		EART_Null									= 0,			// ������������buff�嶥

		EART_Max,
	};

	// buff���ӷ�ʽ(ͬԴ, ��ͬԴ)
	enum EAuraStackType
	{
		EAST_Null									= 0,
		EAST_Refresh								= 1,			// ����, (ʱ��, ����)
		EAST_AddCount								= 2,			// ���ʣ�����, ����ʱ��
		EAST_Add									= 3,			// ��Ӳ�Ӱ������buff
		EAST_CantAdd								= 4,			// ������, �����
		EAST_Max,
	};

	// ����buff����ʧ��ʱ��ʽ
	enum EAuraDeadTimerType
	{
		EADTT_Null									= 0,			// ��������ʧ��д��ֵ
		EADTT_Stop									= 1,			// �������ʱֹͣ
		EADTT_Normal								= 2,			// ����������ֹͣ
		EADTT_Max,
	};

	// ����buff����ʧ��ʱ��ʽ
	enum EAuraOfflineTimerType
	{
		EAOTT_Null									= 0,			// ���ߺ���ʧ��д��ֵ
		EAOTT_Stop									= 1,			// ���ߺ��ʱֹͣ
		EAOTT_Normal								= 2,			// ���ߺ�����ֹͣ
		EAOTT_Max,
	};

	// �츳�ı似��ö��
	enum ESpellModType
	{
		ESMT_Null									= 0,			// ��Ч, ������
		ESMT_SpellCastingTime						= 1,			// Ӱ�켼������ʱ��
		ESMT_SpellRange								= 2,			// Ӱ�켼��������
		ESMT_SpellSpeed								= 3,			// Ӱ�켼�ܷ����ٶ�
		ESMT_SpellRadius							= 4,			// Ӱ�켼�ܰ뾶
		ESMT_SpellAngle								= 5,			// Ӱ�켼�ܰ뾶�Ƕ�
		ESMT_SpellChainTarget						= 6,			// Ӱ������������
		ESMT_SpellDuration							= 7,			// Ӱ�����ʱ��
		ESMT_SpellCost								= 8,			// Ӱ�켼������ֵ
		ESMT_SpellCostScale							= 9,			// Ӱ�켼��������������
		ESMT_SpellModEffect							= 10,			// �滻����Ч��
		ESMT_ModEffectTriggerSpell					= 11,			// �滻��������
		ESMT_ModEffectApplyAura						= 12,			// �滻Ӧ�õ�buff
		ESMT_ModTriggerSpellRate					= 13,			// Ӱ�켼��Trigger��������
		ESMT_SpellEffectValue						= 14,			// Ӱ�켼��effect��ֵ

		ESMT_AuraMaxDuration						= 15,			// Ӱ��buff����ʱ��
		ESMT_AuraTick								= 16,			// Ӱ��buff tick��ʽ
		ESMT_AuraPeriodicTime						= 17,			// Ӱ��buff tick����
		ESMT_AuraResistDispelChance					= 18,			// �ı�Buff��ɢ�ȼ�
		ESMT_AuraStack								= 19,			// Ӱ��buff���Ӵ���
		ESMT_AuraEffectValue						= 20,			// Ӱ��buff effect��ֵ

		ESMT_Max,
	};

	enum ESpellModSubType
	{
		ESMST_Null									= 0,
		ESMST_Value									= 1,			// ����ֵ�޸�
		ESMST_Pct									= 2,			// ���ٷֱ��޸�
		ESMST_Replace								= 3,			// �滻
		ESMST_Max,
	};

	// buff�������¼�����(�������buff������, ����Խ�ɫ)
	enum EAuraTriggerEventType
	{
		EATET_Null									= 0,
		EATET_OnHited								= 1,			// buff�����߱�����
		EATET_OnDead								= 2,			// buff����������
		EATET_OnCasting								= 3,			// buff�������ͷż���
		EATET_Ondispeled							= 4,			// buff����ɢʱ
		EATET_Max,
	};

	// buff�������ܸ�˭
	enum EAuraTriggerTargetType
	{
		EATTT_Null									= 0,
		EATTT_Self									= 1,			// buff������
		EATTT_Caster								= 2,			// buff�ͷ���
		EATTT_Party									= 3,			// buff�����ߵ�С�ӳ�Ա
		EATTT_CasterParty							= 4,			// buff�ͷ��ߵ�С�ӳ�Ա
		EATTT_Attacker								= 5,			// ������

		EATTT_Max,
	};

	// ������ģ���, ����ѧϰ
	struct nspell_talent_table : public att_info_base
	{
		uint32					class_;								// ְҵ
		uint32					level;								// ����ȼ�
		uint32					point;								// �ѼӼ��ܵ�
		uint32					money;								// ������Ϸ��
		uint32					reputation_faction;					// ��������
		uint32					reputation_rank;					// ���������ȼ� 
		nname					depends_on;							// ���輼�����ڵ�id nspell_talent_table
		uint32					depends_on_rank;					// ����ӵ�ȼ�
		nname					spell_id[MAX_SPELL_TALENT_RANK];	// ���ȼ���Ӧ�ļ���id
		//nname					icon_effect[MAX_SPELL_TALENT_RANK + 1];	// 0���͸��ȼ���Ӧ��ͼ��
	
		nspell_talent_table()
		{
		}
		explicit nspell_talent_table(const nstring& id_str) 
			: att_info_base(id_str)
		{
		}

	protected:
		virtual void custom_serialize(narchive& ar)
		{
			nSERIALIZE(ar, class_);
			nSERIALIZE(ar, level);
			nSERIALIZE(ar, point);
			nSERIALIZE(ar, money);
			nSERIALIZE(ar, reputation_faction);
			nSERIALIZE(ar, reputation_rank);
			nSERIALIZE(ar, depends_on);
			nSERIALIZE(ar, depends_on_rank);
			nSERIALIZE(ar, spell_id[0]);
			nSERIALIZE(ar, spell_id[1]);
			nSERIALIZE(ar, spell_id[2]);
			nSERIALIZE(ar, spell_id[3]);
			nSERIALIZE(ar, spell_id[4]);
			nSERIALIZE(ar, spell_id[5]);
			nSERIALIZE(ar, spell_id[6]);
			nSERIALIZE(ar, spell_id[7]);
			nSERIALIZE(ar, spell_id[8]);
			nSERIALIZE(ar, spell_id[9]);
		}
	};

	// ����ģ���
	struct nspell_info : public att_info_base
	{
		uint32					type;								// ����, ��ESpellType
		uint32					sub_type;							// ������, ����type������ȡ��ͬö��ֵ
		uint32					level;								// �ȼ�

		bool					active;								// �Ƿ���������, (true = ��������, false = ��������)
		bool					can_cast_in_combat;					// �Ƿ������ս��״̬�ͷ�, (true = ����, false = ������)
		bool					auto_path_find;						// �Ƿ��Ѱ·�����ͷ�λ��, (true = ��Ѱ·, false = ����Ѱ·)
		bool					auto_orient_target;					// �Ƿ���Ҫ�Զ�����Ŀ��, (true = ��, false = ��)
		bool					no_line_of_sight;					// �Ƿ��������߼��, (true = �������߼��, false = ���������߼��)
		bool					can_cast_while_feared;				// �־�ʱ�Ƿ�����ͷ�, (true = ����, false = ������)
		bool					can_cast_while_silence;				// ��Ĭʱ�Ƿ�����ͷ�, (true = ����, false = ������)
		bool					can_cast_while_root;				// ����ʱ�Ƿ�����ͷ�, (true = ����, false = ������)
		bool					can_cast_while_move;				// �ƶ���ת��ʱ�Ƿ�����ͷ�, (true = ����, false = ������)

		uint32					weapon_type;						// ������������, ��ESpellWeaponType
		uint32					main_hand;							// ����������������, ��Ӧ��������Ʒ������
		uint32					off_hand;							// ����������������, ��Ӧ��������Ʒ������

		uint32					cast_target_type;					// �ͷ�Ŀ������, ��ECastTargetType
		float					min_range;							// ѡ��Ŀ�����С����
		float					max_range;							// ѡ��Ŀ���������

		uint32					effect_target_type;					// ����Ŀ�����ͣ���EEffectTargetType
		float					radius;								// ����뾶
		float					angle;								// �������νǶ�
		float					weight;								// ������
		float					length;								// ���򳤶�

		uint32					cast_type;							// �ͷŷ�ʽ, ��ESpellCastType
		uint32					interrupt_flags;					// ��Ϸ�ʽ (����, ����, ����)��ESpellInterruptFlags
		bool					can_repeat;							// �Ƿ���Գ����ͷ�,(true == �ɳ���, false == �����Գ���)
		bool					can_move;							// �Ƿ�����ƶ�, (true == ����, false == �������ƶ�)
		bool					can_turn;							// �Ƿ����ת��, �����˲����ʾ�����������Ƿ����ת��,  (true == ����, false == ������ת��)
		float					min_repeat_time;					// ��̳���ʱ��, ��λs
		float					max_repeat_time;					// �����ʱ��, ��λs
		float					speed_scale_coef;					// �����ͷ�ʱ�ٶ�����ϵ��speed * (1 + speed_scale_coef * count)
		float					effect_scale_coef;					// �����ͷ�ʱЧ��ֵ����ϵ��effect * (1 + effect_scale_coef * count)
		float					casting_time;						// ����ʱ��, ��λs
		uint32					max_tick_count;						// �����������
		float					periodic_time;						// ��������ʱ��, ��λs
		float					speed;								// ���е��ٶ�

		bool					global_cd_not_effect;				// �Ƿ���ȫ��cdӰ��, (true == ���ܹ���cdӰ��, false == �ܹ���cdӰ��)
		float					global_cd_time;						// ������ȫ��cdʱ��, ��λs
		float					cd_time;							// �����ļ���cdʱ��, ��λs

		uint32					cost_type;							// �������ķ�ʽ, ��ESpellCostType
		float					cost_value;							// ������������ֵ
		float					cost_scale_coef;					// �����ͷ�ʱ��������ϵ��, ������(-1.0f) cost_value * (1 + cost_scale_coef * count)
		nname					reagent_id;							// ���������ҩ��, ���ü������Ĳ�����Ʒ��

		nname					spell_effect_id[MAX_EFFECT_NUM];	// ���ܸ���Ч������

		// �ͻ���ר����Ϣ
		nstring					spell_icon;							// ����ͼ��
		nstring					client_effect;						// ���ܶ�����Ч����
		//nname					inactive_icon;						// ��Ч״̬ͼ��
		//nname					prepare_action;						// ׼������
		//nname					casting_action;						// �ͷŶ���
		//nname					casting_effect;						// �ͷ���Ч
		//nname					fly_effect;							// ������Ч
		//nname					hit_effect;							// ������Ч
	
		nspell_info()
		{
		}
		explicit nspell_info(const nstring& id_str) 
			: att_info_base(id_str)
		{
		}

	protected:
		virtual void custom_serialize(narchive& ar)
		{
			nserialize(ar,type,						_T("type"));
			nserialize(ar,sub_type,					_T("sub_type"));
			nserialize(ar,level,					_T("level"));

			nserialize(ar,active,					_T("active"));
			nserialize(ar,can_cast_in_combat,		_T("can_cast_in_combat"));
			nserialize(ar,auto_path_find,			_T("auto_path_find"));
			nserialize(ar,auto_orient_target,		_T("auto_orient_target"));
			nserialize(ar,no_line_of_sight,			_T("no_line_of_sight"));
			nserialize(ar,can_cast_while_feared,	_T("can_cast_while_feared"));
			nserialize(ar,can_cast_while_silence,	_T("can_cast_while_silence"));
			nserialize(ar,can_cast_while_root,		_T("can_cast_while_root"));
			nserialize(ar,can_cast_while_move,		_T("can_cast_while_move"));

			nserialize(ar,weapon_type,				_T("weapon_type"));
			nserialize(ar,main_hand,				_T("main_hand"));
			nserialize(ar,off_hand,					_T("off_hand"));

			nserialize(ar,cast_target_type,			_T("cast_target_type"));
			nserialize(ar,min_range,				_T("min_range"));
			nserialize(ar,max_range,				_T("max_range"));

			nserialize(ar,effect_target_type,		_T("effect_target_type"));
			nserialize(ar,radius,					_T("radius"));
			nserialize(ar,angle,					_T("angle"));
			nserialize(ar,weight,					_T("weight"));
			nserialize(ar,length,					_T("length"));

			nserialize(ar,cast_type,				_T("cast_type"));
			nserialize(ar,interrupt_flags,			_T("interrupt_flags"));
			nserialize(ar,can_repeat,				_T("can_repeat"));
			nserialize(ar,can_move,					_T("can_move"));
			nserialize(ar,can_turn,					_T("can_turn"));
			nserialize(ar,min_repeat_time,			_T("min_repeat_time"));
			nserialize(ar,max_repeat_time,			_T("max_repeat_time"));
			nserialize(ar,speed_scale_coef,			_T("speed_scale_coef"));
			nserialize(ar,effect_scale_coef,		_T("effect_scale_coef"));
			nserialize(ar,casting_time,				_T("casting_time"));
			nserialize(ar,max_tick_count,			_T("max_tick_count"));
			nserialize(ar,periodic_time,			_T("periodic_time"));
			nserialize(ar,speed,					_T("speed"));

			nserialize(ar,global_cd_not_effect,		_T("global_cd_not_effect"));
			nserialize(ar,global_cd_time,			_T("global_cd_time"));
			nserialize(ar,cd_time,					_T("cd_time"));

			nserialize(ar,cost_type,				_T("cost_type"));
			nserialize(ar,cost_value,				_T("cost_value"));
			nserialize(ar,cost_scale_coef,			_T("cost_scale_coef"));
			nserialize(ar,reagent_id,				_T("reagent_id"));

			nserialize(ar,spell_effect_id[0],		_T("spell_effect_id[0]"));
			nserialize(ar,spell_effect_id[1],		_T("spell_effect_id[1]"));
			nserialize(ar,spell_effect_id[2],		_T("spell_effect_id[2]"));

			nserialize(ar,spell_icon,				_T("spell_icon"));
			nserialize(ar,client_effect,			_T("client_effect"));
		}
	};

	// ����Ч��ģ���
	struct nspell_effect_info : public att_info_base
	{
		uint32					effect_trigger_type;				// ��Ч��ʽ, ��ESpellEffectTriggerType
		uint32					trigger_target_type;				// ��˭, ��ESpellTriggerTargetType
		float					rate;								// ��Ч����

		uint32					effect_type;						// ����Ч������, ��ESpellEffectType

		uint32					damage_type;						// �˺�����, ��ESpellDMGType
		float					value1;								// ��ֵ1
		float					value2;								// ��ֵ2

		nname					dst_teleport_id;					// ����Ŀ�ĵ�id������������
		nname					summon_id;							// �ٻ���id������������

		int32					threat;								// ��вֵ, ���ֵ

		nname					trigger_spell_id;					// ��������id
		nname					apply_aura_id;						// Ӧ��aura��չ����Ч��

		nspell_effect_info()
		{
		}
		explicit nspell_effect_info(const nstring& id_str) 
			: att_info_base(id_str)
		{
		}

	protected:
		virtual void custom_serialize(narchive& ar)
		{
			nserialize(ar, effect_trigger_type,		_T("effect_trigger_type"));
			nserialize(ar, trigger_target_type,		_T("trigger_target_type"));
			nserialize(ar, rate,					_T("rate"));

			nserialize(ar, effect_type,				_T("effect_type"));

			nserialize(ar, damage_type,				_T("damage_type"));
			nserialize(ar, value1,					_T("value1"));
			nserialize(ar, value2,					_T("value2"));

			nserialize(ar, dst_teleport_id,			_T("dst_teleport_id"));
			nserialize(ar, summon_id,				_T("summon_id"));

			nserialize(ar, threat,					_T("threat"));

			nserialize(ar, trigger_spell_id,		_T("trigger_spell_id"));
			nserialize(ar, apply_aura_id,			_T("apply_aura_id"));
		}
	};

	// buffЧ��ģ���
	struct nspell_aura_info : public att_info_base
	{
		uint32					effect_type;						// Ч������, ��EAuraEffectType
		bool					positive;							// �Ƿ�����, (true == ����, false == ����)
		bool					to_client;							// �Ƿ�ͬ�����ͻ���, (true == ͬ��, false == ��ͬ��)
		uint32					interrupt_flags;					// ���(aura remove)��־, ��EAuraInterruptFlags

		uint32					diminishing_group;					// ����ʱ��˥������, ��EAuraDiminishingGroup
		float					max_duration;						// ������ʱ��, ��λs
		float					periodic_time;						// ����ʱ��, ��λs

		uint32					dispel_level;						// ��ɢ�ȼ�

		uint32					replace_type;						// �滻����, ��ͬ���͵Ĳ�ͬ�ȼ�buff�໥�滻, ��EAuraReplaceType
		uint32					level;								// buff�ȼ�, ����ͬ�滻����buff�嶥
		uint32					ssrc_stack_type;					// ͬԴ���ӷ�ʽ, ��EAuraStackType
		uint32					dsrc_stack_type;					// ��Դ���ӷ�ʽ, ��EAuraStackType
		uint32					max_stack_count;					// ��������

		uint32					death;								// ��������ʧ��ʱ��ʽ, ��EAuraDeadTimerType
		uint32					offline;							// ���߲���ʧ��ʱ��ʽ, ��EAuraOfflineTimerType

		uint32					stat_index[MAX_AURA_STAT_NUM];		// Ӱ������, ��EUnitFields(EUF_UnitNull -- EUF_UnitSeparator)
		float					stat_value[MAX_AURA_STAT_NUM];		// Ӱ������ֵ

		uint32					damage_type;						// �˺�����, ��ESpellDMGType
		float					value1;								// ��ֵ1
		float					value2;								// ��ֵ2

		int32					threat;								// ��вֵ, ���ֵ

		uint32					spell_mod_type;						// �츳Ч������, ��ESpellModType
		uint32					spell_mod_sub_type;					// �츳�޸ķ�ʽ, ��ESpellModSubType
		nname					spell_mod_spell_id;					// �츳���ĸ�������Ӱ��
		float					spell_mod_value1;					// �츳����ֵ
		nname					spell_mod_value2;					// �츳�滻idֵ

		uint32					trigger_event_type;					// ������ʽ, ��EAuraTriggerEventType
		nname					cast_spell_id;						// ʹ�ü���id
		float					rate;								// ��������
		uint32					target_type;						// ������˭, ��EAuraTriggerTargetType
		uint32					max_chain_target;					// Ŀ��������Ŀ
		nname					trigger_spell_id;					// ��������id
		nname					trigger_aura_id;					// ����buff id

		// �ͻ���ר����Ϣ
		nstring					aura_icon;							// buffͼ��, ��Чֵ����ʾͼ��
		nstring					add_effect;							// buff�����Ч
		nstring					duration_effect;					// buff������Ч
		nstring					remove_effect;						// buff��ʧ��Ч
	
		nspell_aura_info()
		{
		}
		explicit nspell_aura_info(const nstring& id_str) 
			: att_info_base(id_str)
		{
		}

	protected:
		virtual void custom_serialize(narchive& ar)
		{
			nserialize(ar, effect_type,				_T("effect_type"));
			nserialize(ar, positive,				_T("positive"));
			nserialize(ar, to_client,				_T("to_client"));
			nserialize(ar, interrupt_flags,			_T("interrupt_flags"));

			nserialize(ar, diminishing_group,		_T("diminishing_group"));
			nserialize(ar, max_duration,			_T("max_duration"));
			nserialize(ar, periodic_time,			_T("periodic_time"));

			nserialize(ar, dispel_level,			_T("dispel_level"));

			nserialize(ar, replace_type,			_T("replace_type"));
			nserialize(ar, level,					_T("level"));
			nserialize(ar, ssrc_stack_type,			_T("ssrc_stack_type"));
			nserialize(ar, dsrc_stack_type,			_T("dsrc_stack_type"));
			nserialize(ar, max_stack_count,			_T("max_stack_count"));

			nserialize(ar, death,					_T("death"));
			nserialize(ar, offline,					_T("offline"));

			nserialize(ar, stat_index[0],			_T("stat_index[0]"));
			nserialize(ar, stat_index[1],			_T("stat_index[1]"));
			nserialize(ar, stat_index[2],			_T("stat_index[2]"));
			nserialize(ar, stat_index[3],			_T("stat_index[3]"));
			nserialize(ar, stat_value[0],			_T("stat_value[0]"));
			nserialize(ar, stat_value[1],			_T("stat_value[1]"));
			nserialize(ar, stat_value[2],			_T("stat_value[2]"));
			nserialize(ar, stat_value[3],			_T("stat_value[3]"));

			nserialize(ar, damage_type,				_T("damage_type"));
			nserialize(ar, value1,					_T("value1"));
			nserialize(ar, value2,					_T("value2"));

			nserialize(ar, threat,					_T("threat"));

			nserialize(ar, spell_mod_type,			_T("spell_mod_type"));
			nserialize(ar, spell_mod_sub_type,		_T("spell_mod_sub_type"));
			nserialize(ar, spell_mod_spell_id,		_T("spell_mod_spell_id"));
			nserialize(ar, spell_mod_value1,		_T("spell_mod_value1"));
			nserialize(ar, spell_mod_value2,		_T("spell_mod_value2"));

			nserialize(ar, trigger_event_type,		_T("trigger_event_type"));
			nserialize(ar, cast_spell_id,			_T("cast_spell_id"));
			nserialize(ar, rate,					_T("rate"));
			nserialize(ar, target_type,				_T("target_type"));
			nserialize(ar, max_chain_target,		_T("max_chain_target"));
			nserialize(ar, trigger_spell_id,		_T("trigger_spell_id"));
			nserialize(ar, trigger_aura_id,			_T("trigger_aura_id"));

			nserialize(ar, aura_icon,				_T("aura_icon"));
			nserialize(ar, add_effect,				_T("add_effect"));
			nserialize(ar, duration_effect,			_T("duration_effect"));
			nserialize(ar, remove_effect,			_T("remove_effect"));
		}
	};


	//-- �������л�ģ�庯��
	SERIALIZE_ATT_INFO(nspell_talent_table);
	SERIALIZE_ATT_INFO(nspell_info);
	SERIALIZE_ATT_INFO(nspell_effect_info);
	SERIALIZE_ATT_INFO(nspell_aura_info);


#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NSPELL_DEFINE_H_
