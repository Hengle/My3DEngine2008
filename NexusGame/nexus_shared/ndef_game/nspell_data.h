/**
 *	nexus ngame_define - nspell_data
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDEF_GAME_NSPELL_DATA_H_
#define _NDEF_GAME_NSPELL_DATA_H_

#include "ntype_def.h"

namespace nexus {

#pragma pack(push,1)

	const uint32 MAX_SPELL_NUM			= 1024;		// ���ܵ������Ŀ
	const uint32 MAX_AURA_NUM			= 256;		// buff�������Ŀ
	const uint32 MAX_CD_NUM				= 50;		// cd����������Ŀ, �Ƿ�洢cd, ��ǰcd����500ms

	enum ESpellCategoryCDType
	{
		ESCCDT_Null						= 0,
		ESCCDT_GlobalCD					= 1,		// ȫ�ֹ���cd
		ESCCDT_Max,
	};

	struct nspell_cool_dowm
	{
		uint32					spell_id;			// ����ģ��id
		uint32					category;			// cd��������
		uint64					cd_time;			// ����ʱ��time_t
	};

	// buffʵ������(size = 10)
	struct naura_data
	{
		uint32					info_id;			// buffģ��id
		uint8					slot;				// buff��λ, guid
		uint8					stack_count;		// ��ǰ���Ӵ���
		uint32					remain_time;		// ʣ��ʱ��(��λ��1/1000s)
		uint32					max_duration;		// ��ʱ��(��λ��1/1000s)
	};

	struct naura_data_db : public naura_data
	{
		uint64					caster_id;			// �ͷ���id
		int32					amount;				// ����ͳ��, ���ô���, ���յ��˺��ȵ�
		uint32					spell_id;			// ������buff�ļ���id, û��Ϊ0
		uint8					index;				// ����Ч������
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NSPELL_DATA_H_
