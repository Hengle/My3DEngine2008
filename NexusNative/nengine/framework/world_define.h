#ifndef _NEXUS_WORLD_DEFINE_H_
#define _NEXUS_WORLD_DEFINE_H_

namespace nexus
{
	/// line_check����
	enum ELineCheckType
	{
		ELCT_Static		= (1<<1),		 //!< ֻ��⾲̬������
		ELCT_Dynamic	= (1<<2),		 //!< ֻ��⶯̬������

		ELCT_Terrain	= (1<<3),		 //!< ֻ������
		ELCT_Navigation	= (1<<4),		 //!< ֻ��⵼��ͼ

		// combinations.
		ELCT_World		= ELCT_Static | ELCT_Terrain,

		ELCT_All = 0x7FFFFFFF
	};
}

#endif //_NEXUS_WORLD_DEFINE_H_