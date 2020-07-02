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
	* class NSpellAuraInfo
	* ��nexus::nspell_aura_info��װ
	* ΪCLR�ṩ����Native struct�ӿ�
	*/
	DECLARE_ATT_INFO_CLASS_BEGIN(NSpellAuraInfo, nspell_aura_info, nspell_aura_info_dictionary);

		DECLARE_ATTINFO_PROPERTY(UInt32,effect_type,NativePtr->effect_type,"Ч������");
		DECLARE_ATTINFO_PROPERTY(bool,positive,NativePtr->positive,"�Ƿ�����");
		DECLARE_ATTINFO_PROPERTY(bool,to_client,NativePtr->to_client,"�Ƿ�ͬ�����ͻ���");
		DECLARE_ATTINFO_PROPERTY(UInt32,interrupt_flags,NativePtr->interrupt_flags,"��ϱ�־");

		DECLARE_ATTINFO_PROPERTY(UInt32,diminishing_group,NativePtr->diminishing_group,"����ʱ��˥������");
		DECLARE_ATTINFO_PROPERTY(float,max_duration,NativePtr->max_duration,"������ʱ��");
		DECLARE_ATTINFO_PROPERTY(float,periodic_time,NativePtr->periodic_time,"����ʱ��");

		DECLARE_ATTINFO_PROPERTY(UInt32,dispel_level,NativePtr->dispel_level,"��ɢ�ȼ�");

		DECLARE_ATTINFO_PROPERTY(UInt32,replace_type,NativePtr->replace_type,"�滻����");
		DECLARE_ATTINFO_PROPERTY(UInt32,level,NativePtr->level,"�ȼ�");
		DECLARE_ATTINFO_PROPERTY(UInt32,ssrc_stack_type,NativePtr->ssrc_stack_type,"ͬԴ���ӷ�ʽ");
		DECLARE_ATTINFO_PROPERTY(UInt32,dsrc_stack_type,NativePtr->dsrc_stack_type,"��Դ���ӷ�ʽ");
		DECLARE_ATTINFO_PROPERTY(UInt32,max_stack_count,NativePtr->max_stack_count,"��������");

		DECLARE_ATTINFO_PROPERTY(UInt32,death,NativePtr->death,"��������ʧ��ʱ��ʽ");
		DECLARE_ATTINFO_PROPERTY(UInt32,offline,NativePtr->offline,"���߲���ʧ��ʱ��ʽ");

		DECLARE_ATTINFO_PROPERTY(UInt32,stat_index_0,NativePtr->stat_index[0],"Ӱ������0");
		DECLARE_ATTINFO_PROPERTY(UInt32,stat_index_1,NativePtr->stat_index[1],"Ӱ������1");
		DECLARE_ATTINFO_PROPERTY(UInt32,stat_index_2,NativePtr->stat_index[2],"Ӱ������2");
		DECLARE_ATTINFO_PROPERTY(UInt32,stat_index_3,NativePtr->stat_index[3],"Ӱ������3");

		DECLARE_ATTINFO_PROPERTY(float,stat_value_0,NativePtr->stat_value[0],"Ӱ������ֵ0");
		DECLARE_ATTINFO_PROPERTY(float,stat_value_1,NativePtr->stat_value[1],"Ӱ������ֵ1");
		DECLARE_ATTINFO_PROPERTY(float,stat_value_2,NativePtr->stat_value[2],"Ӱ������ֵ2");
		DECLARE_ATTINFO_PROPERTY(float,stat_value_3,NativePtr->stat_value[3],"Ӱ������ֵ3");

		DECLARE_ATTINFO_PROPERTY(UInt32,damage_type,NativePtr->damage_type,"�˺�����");
		DECLARE_ATTINFO_PROPERTY(float,value1,NativePtr->value1,"��ֵ1");
		DECLARE_ATTINFO_PROPERTY(float,value2,NativePtr->value2,"��ֵ2");

		DECLARE_ATTINFO_PROPERTY(Int32,threat,NativePtr->threat,"���ֵ");

		DECLARE_ATTINFO_PROPERTY(UInt32,spell_mod_type,NativePtr->spell_mod_type,"�츳Ч������");
		DECLARE_ATTINFO_PROPERTY(UInt32,spell_mod_sub_type,NativePtr->spell_mod_sub_type,"�츳�޸ķ�ʽ");
		DECLARE_ATTINFO_NAME_PROPERTY(spell_mod_spell_id,NativePtr->spell_mod_spell_id,"�츳���ĸ�������Ӱ��");
		DECLARE_ATTINFO_PROPERTY(float,spell_mod_value1,NativePtr->spell_mod_value1,"�츳����ֵ");
		DECLARE_ATTINFO_NAME_PROPERTY(spell_mod_value2,NativePtr->spell_mod_value2,"�츳�滻idֵ");

		DECLARE_ATTINFO_PROPERTY(UInt32,trigger_event_type,NativePtr->trigger_event_type,"������ʽ");
		DECLARE_ATTINFO_NAME_PROPERTY(cast_spell_id,NativePtr->cast_spell_id,"ʹ�ü���id");
		DECLARE_ATTINFO_PROPERTY(float,rate,NativePtr->rate,"��������");
		DECLARE_ATTINFO_PROPERTY(UInt32,target_type,NativePtr->target_type,"������˭");
		DECLARE_ATTINFO_PROPERTY(UInt32,max_chain_target,NativePtr->max_chain_target,"Ŀ��������Ŀ");
		DECLARE_ATTINFO_NAME_PROPERTY(trigger_spell_id,NativePtr->trigger_spell_id,"��������id");
		DECLARE_ATTINFO_NAME_PROPERTY(trigger_aura_id,NativePtr->trigger_aura_id,"����buff id");

		// �ͻ���
		DECLARE_ATTINFO_STRING_PROPERTY(aura_icon, NativePtr->aura_icon, "ͼ��");
		DECLARE_ATTINFO_STRING_PROPERTY(add_effect, NativePtr->add_effect, "�����Ч");
		DECLARE_ATTINFO_STRING_PROPERTY(duration_effect, NativePtr->duration_effect, "������Ч");
		DECLARE_ATTINFO_STRING_PROPERTY(remove_effect, NativePtr->remove_effect, "��ʧ��Ч");

	DECLARE_ATT_INFO_CLASS_END();

}
