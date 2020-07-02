/**
 *	nexus ndef_cluster - nplayer_define.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_CLUSTER_NPLAYER_DEFINE_H_
#define _NDEF_CLUSTER_NPLAYER_DEFINE_H_

#include "nobject_define.h"

namespace nexus {

#pragma pack(push,1)

	// �����������
	struct nplayer_info
	{
		uint64			player_id;						// 8 ID
		uint32			name_crc;						// 4 ����CRC
		uint32			account_id;						// 4 �˻�ID
		nchar			name[MAX_NAME_LEN];				// 16 ����
		uint32			map_id;							// 4 ���ڵ�ͼ
		uint64			last_online_time;				// 8 �������ʱ��
		uint16			flag;							// 2 ְҵ���ȼ����Ƿ�����
	};




#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_CLUSTER_NPLAYER_DEFINE_H_
