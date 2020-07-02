/**
 *	nexus ndef_game - nitem_define
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NITEM_DEFINE_H_
#define _NDEF_GAME_NITEM_DEFINE_H_

#include "ntype_def.h"

namespace nexus {

#pragma pack(push,1)

	// ��װ������Ʒʵ��(size = 18)
	struct nitem_data
	{
		uint64					guid;			// 8 ��ƷΨһid
		uint32					info_id;		// 4 ģ��id
		uint16					count;			// 2 ��ǰ�ѵ���Ŀ
		uint16					slot;			// 2 �����е�λ��
		uint8					bag;			// 1 ��������
		uint8					flag;			// 1 ��־λ��Ϣ(�󶨣�����������(�ǽ������������))
	};

	// װ������Ʒʵ��(size = 18 + 74)
	struct nequip_data : public nitem_data
	{
		uint8					quality;		// 1 Ʒ����ǿ���ȼ�����һ��?
		uint8					level;			// 1 ǿ���ȼ�
		uint32					durability;		// 4 �;�
		uint16					weight;			// 2 ����
		uint16					sharp;			// 2 ����
		uint16					solid;			// 2 ��ʵ
		uint16					tenacity;		// 2 ǿ��

		uint8					stat_type[8];	// 3 * 8 = 24 ��������
		uint16					stat_value[8];
		uint16					gem[6];			// 2 * 6 = 12 ��Ƕ�ı�ʯ, ���ٴ������Ʒʵ����id�ķ�Χ����,��0xFFFF���洢ʱPAIR32_LOPART(spell_id), ʹ��ʱMAKE_PAIR32(gem[12], 0)

		uint16					spell[4];		// 2 * 4 = 8 ���Ӽ���, �洢ʱPAIR32_LOPART(spell_id), ʹ��ʱMAKE_PAIR32(spell[0], 0)
		uint16					spell_rate[3];	// 2 * 3 = 6 ���ӱ������ܴ�������
		uint16					enchantment;	// 2 ��ħid, �洢ʱPAIR32_LOPART(spell_id), ʹ��ʱMAKE_PAIR32(enchantment, 0)
		uint64					creator;		// 8 ������id
	};

	// cd ʱ����־�ʱ�䵥���洢
	struct nitem_duration
	{
		uint32					guid;			// ��Ʒid
		uint64					destroy_time;	// ����ʱ��time_t
	};

	// �����������������40 + 4 * 25 = 140��������ޣ���ʹ���Ͼ��������Կ�ף����ң���ս��������Ϊ����Ʒ�־ô洢
	// �ֿ��������������60 + 8 * 25 = 260���������

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NITEM_DEFINE_H_
