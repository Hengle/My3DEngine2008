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
	* class NSpellTalentTable
	* ��nexus::nspell_talent_table��װ
	* ΪCLR�ṩ����Native struct�ӿ�
	*/
	DECLARE_ATT_INFO_CLASS_BEGIN(NSpellTalentTable, nspell_talent_table, nspell_talent_table_dictionary);
		// ְҵ
		DECLARE_ATTINFO_PROPERTY(UInt32,Class,NativePtr->class_,"ְҵ");

		// ����ȼ�
		DECLARE_ATTINFO_PROPERTY(UInt32,Level,NativePtr->level,"����ȼ�");

		// �ѼӼ��ܵ�
		DECLARE_ATTINFO_PROPERTY(UInt32,Point,NativePtr->point,"�ѼӼ��ܵ�");

		// ���ĵ���Ϸ��
		DECLARE_ATTINFO_PROPERTY(UInt32,Money,NativePtr->money,"������Ϸ��");

		// ��������
		DECLARE_ATTINFO_PROPERTY(UInt32,ReputationFaction,NativePtr->reputation_faction,"��������");

		// ���������ȼ�
		DECLARE_ATTINFO_PROPERTY(UInt32,ReputationRank,NativePtr->reputation_rank,"���������ȼ�");
		
		// ���輼�����ڵ�
		DECLARE_ATTINFO_NAME_PROPERTY(DependsOn,NativePtr->depends_on,"���輼�����ڵ�");
		
		// ����ӵ�ȼ�
		DECLARE_ATTINFO_PROPERTY(UInt32,DependsOnRank,NativePtr->depends_on_rank,"����ӵ�ȼ�");
		
		// ���輼�����ڵ�
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_0,NativePtr->spell_id[0],"����0");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_1,NativePtr->spell_id[1],"����1");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_2,NativePtr->spell_id[2],"����2");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_3,NativePtr->spell_id[3],"����3");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_4,NativePtr->spell_id[4],"����4");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_5,NativePtr->spell_id[5],"����5");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_6,NativePtr->spell_id[6],"����6");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_7,NativePtr->spell_id[7],"����7");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_8,NativePtr->spell_id[8],"����8");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_9,NativePtr->spell_id[9],"����9");

	DECLARE_ATT_INFO_CLASS_END();

}
