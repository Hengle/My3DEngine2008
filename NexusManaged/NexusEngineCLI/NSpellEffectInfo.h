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
	* class NSpellEffectInfo
	* ��nexus::nspell_effect_info��װ
	* ΪCLR�ṩ����Native struct�ӿ�
	*/
	DECLARE_ATT_INFO_CLASS_BEGIN(NSpellEffectInfo, nspell_effect_info, nspell_effect_info_dictionary);
	
		DECLARE_ATTINFO_PROPERTY(UInt32,effect_trigger_type,NativePtr->effect_trigger_type,"��Ч��ʽ");
		DECLARE_ATTINFO_PROPERTY(UInt32,trigger_target_type,NativePtr->trigger_target_type,"��˭");
		DECLARE_ATTINFO_PROPERTY(float,rate,NativePtr->rate,"��Ч����");

		DECLARE_ATTINFO_PROPERTY(UInt32,effect_type,NativePtr->effect_type,"����Ч������");

		DECLARE_ATTINFO_PROPERTY(UInt32,damage_type,NativePtr->damage_type,"�˺�����");
		DECLARE_ATTINFO_PROPERTY(float,value1,NativePtr->value1,"��ֵ1");
		DECLARE_ATTINFO_PROPERTY(float,value2,NativePtr->value2,"��ֵ2");

		DECLARE_ATTINFO_NAME_PROPERTY(dst_teleport_id,NativePtr->dst_teleport_id,"����Ŀ�ĵ�");
		DECLARE_ATTINFO_NAME_PROPERTY(summon_id,NativePtr->summon_id,"�ٻ���");

		DECLARE_ATTINFO_PROPERTY(Int32,threat,NativePtr->threat,"���ֵ");

		DECLARE_ATTINFO_NAME_PROPERTY(trigger_spell_id,NativePtr->trigger_spell_id,"��������");
		DECLARE_ATTINFO_NAME_PROPERTY(apply_aura_id,NativePtr->apply_aura_id,"��չ����Ч��");

	DECLARE_ATT_INFO_CLASS_END();

}
