/**
 *	nexus network - ntype_def.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NCORE_NTYPE_DEF_H_
#define _NCORE_NTYPE_DEF_H_

#include <tchar.h>

#pragma pack(push,1)

namespace nexus {

	/**
	 *	����Э���л�������
	 */
	typedef signed __int64		int64;
	typedef signed __int32		int32;
	typedef signed __int16		int16;
	typedef signed __int8		int8;

	typedef unsigned __int64	uint64;
	typedef unsigned __int32	uint32;
	typedef unsigned __int16	uint16;
	typedef unsigned __int8		uint8;

	// bool����					bool
	// 32λ��������			float
	// 64λ��������			double
	// �ַ�����					char			// һ�����ڶ��建����
	typedef TCHAR				nchar;			// һ�����ڶ�������

} // namespace nexus

#pragma pack(pop)

#endif // _NNETWORK_NTYPE_DEF_H_
