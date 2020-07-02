#pragma once
#include "NEngine.h"
#include "NFileSystem.h"
#include "NAttDataHelper.h"
#include "nspell_define.h"
#include "ngameframework.h"
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>

namespace NexusEngine
{
	using namespace NexusEngineExtension;
	using namespace nexus;
	using namespace gameframework;

	/**
	* class NSpellInfo
	* ��nexus::nspell_info��װ
	* ΪCLR�ṩ����Native struct�ӿ�
	*/
	DECLARE_ATT_INFO_CLASS_BEGIN(NSpellInfo, nspell_info, nspell_info_dictionary);
		DECLARE_ATTINFO_PROPERTY(UInt32,Type,NativePtr->type,"����");
		DECLARE_ATTINFO_PROPERTY(UInt32,SubType,NativePtr->sub_type,"������");
		DECLARE_ATTINFO_PROPERTY(UInt32,Level,NativePtr->level,"�ȼ�");

		// �Ƿ���������, (true = ��������, false = ��������)
		DECLARE_ATTINFO_PROPERTY(bool,Active,NativePtr->active,"�Ƿ���������");
		DECLARE_ATTINFO_PROPERTY(bool,CanCastInCombat,NativePtr->can_cast_in_combat,"�Ƿ������ս��״̬�ͷ�");
		DECLARE_ATTINFO_PROPERTY(bool,AutoPathFind,NativePtr->auto_path_find,"�Ƿ��Ѱ·�����ͷ�λ��");
		DECLARE_ATTINFO_PROPERTY(bool,NoLineOfSight,NativePtr->no_line_of_sight,"�Ƿ��������߼��");
		DECLARE_ATTINFO_PROPERTY(bool,CanCastWhileFeared,NativePtr->can_cast_while_feared,"�־�ʱ�Ƿ�����ͷ�");
		DECLARE_ATTINFO_PROPERTY(bool,CanCastWhileSilence,NativePtr->can_cast_while_silence,"��Ĭʱ�Ƿ�����ͷ�");
		DECLARE_ATTINFO_PROPERTY(bool,CanCastWhileRoot,NativePtr->can_cast_while_root,"����ʱ�Ƿ�����ͷ�");

		DECLARE_ATTINFO_PROPERTY(UInt32,WeaponType,NativePtr->weapon_type,"������������");
		DECLARE_ATTINFO_PROPERTY(UInt32,MainHand,NativePtr->main_hand,"����������������");
		DECLARE_ATTINFO_PROPERTY(UInt32,OffHand,NativePtr->off_hand,"����������������");

		DECLARE_ATTINFO_PROPERTY(UInt32,CastTargetType,NativePtr->cast_target_type,"�ͷ�Ŀ������");
		DECLARE_ATTINFO_PROPERTY(float,MinRange,NativePtr->min_range,"ѡ��Ŀ�����С����");
		DECLARE_ATTINFO_PROPERTY(float,MaxRange,NativePtr->max_range,"ѡ��Ŀ���������");

		DECLARE_ATTINFO_PROPERTY(UInt32,EffectTargetType,NativePtr->effect_target_type,"����Ŀ������");
		DECLARE_ATTINFO_PROPERTY(float,Radius,NativePtr->radius,"����뾶");
		DECLARE_ATTINFO_PROPERTY(float,Angle,NativePtr->angle,"�������νǶ�");
		DECLARE_ATTINFO_PROPERTY(float,Weight,NativePtr->weight,"������");
		DECLARE_ATTINFO_PROPERTY(float,Length,NativePtr->length,"���򳤶�");

		DECLARE_ATTINFO_PROPERTY(UInt32,CastType,NativePtr->cast_type,"�ͷŷ�ʽ");
		DECLARE_ATTINFO_PROPERTY(UInt32,InterruptFlags,NativePtr->interrupt_flags,"��Ϸ�ʽ");
		DECLARE_ATTINFO_PROPERTY(bool,CanRepeat,NativePtr->can_repeat,"�Ƿ���Գ����ͷ�");
		DECLARE_ATTINFO_PROPERTY(bool,CanMove,NativePtr->can_move,"�Ƿ�����ƶ�");
		DECLARE_ATTINFO_PROPERTY(bool,CanTurn,NativePtr->can_turn,"�Ƿ����ת��");
		DECLARE_ATTINFO_PROPERTY(float,MinRepeatTime,NativePtr->min_repeat_time,"��̳���ʱ��");
		DECLARE_ATTINFO_PROPERTY(float,MaxRepeatTime,NativePtr->max_repeat_time,"�����ʱ��");
		DECLARE_ATTINFO_PROPERTY(float,SpeedScaleCoef,NativePtr->speed_scale_coef,"�����ͷ�ʱ�ٶ�����ϵ��");
		DECLARE_ATTINFO_PROPERTY(float,EffectScaleCoef,NativePtr->effect_scale_coef,"�����ͷ�ʱЧ��ֵ����ϵ��");
		DECLARE_ATTINFO_PROPERTY(float,CastingTime,NativePtr->casting_time,"����ʱ��");
		DECLARE_ATTINFO_PROPERTY(UInt32,MaxTickCount,NativePtr->max_tick_count,"�����������");
		DECLARE_ATTINFO_PROPERTY(float,PeriodicTime,NativePtr->periodic_time,"��������ʱ��");
		DECLARE_ATTINFO_PROPERTY(float,Speed,NativePtr->speed,"���е��ٶ�");

		DECLARE_ATTINFO_PROPERTY(bool,GlobalCDNotEffect,NativePtr->global_cd_not_effect,"�Ƿ���ȫ��cdӰ��");
		DECLARE_ATTINFO_PROPERTY(float,GlobalCDTime,NativePtr->global_cd_time,"������ȫ��cdʱ��");
		DECLARE_ATTINFO_PROPERTY(float,CDTime,NativePtr->cd_time,"�����ļ���cdʱ��");

		DECLARE_ATTINFO_PROPERTY(UInt32,CostType,NativePtr->cost_type,"�������ķ�ʽ");
		DECLARE_ATTINFO_PROPERTY(float,CostValue,NativePtr->cost_value,"������������ֵ");
		DECLARE_ATTINFO_PROPERTY(float,CostScaleCoef,NativePtr->cost_scale_coef,"�����ͷ�ʱ��������ϵ��");
		DECLARE_ATTINFO_NAME_PROPERTY(ReagentId,NativePtr->reagent_id,"���������ҩ��");


		DECLARE_ATTINFO_NAME_PROPERTY(SpellEffectId_0,NativePtr->spell_effect_id[0],"���ܸ���Ч��0");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellEffectId_1,NativePtr->spell_effect_id[1],"���ܸ���Ч��1");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellEffectId_2,NativePtr->spell_effect_id[2],"���ܸ���Ч��2");
		
		// �ͻ���
		DECLARE_ATTINFO_STRING_PROPERTY(SpellIcon, NativePtr->spell_icon, "����ͼ��");
		DECLARE_ATTINFO_STRING_PROPERTY(ClientEffect, NativePtr->client_effect, "���ܶ�����Ч����");

	DECLARE_ATT_INFO_CLASS_END();

}
