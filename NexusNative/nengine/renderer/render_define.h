/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _RENDER_DEFINE_H_
#define _RENDER_DEFINE_H_

namespace nexus
{
	typedef unsigned int hit_id;

	enum ERenderMode
	{
		ERM_Wireframe,
		ERM_Unlit,
		ERM_Lit,
		ERM_LightingOnly,
	};

	enum EDepthGroup
	{
		EDG_Background = 0,	// ������
		EDG_World,			// ���������
		EDG_Foreground,		// ǰ����
		EDG_Max,
	};

	enum ETransparentType
	{
		ETrans_Opaque= 0,		// ��ȫ��͸��
		ETrans_AlphaMasked,		// ��͸����������Ҫalpha mask
		ETrans_Mixed,			// �����а�͸���Ĳ���, Ҳ�в�͸���Ĳ���		
		ETrans_UnlitTrans,		// ��ȫ��͸����Ҳ����Ҫ����
		ETrans_Max
	};

	struct render_config
	{
		void*	window_handle;
		size_t	width,
				height;
		size_t	color_bits;		
		bool	bWindowed;
		bool	bEnableHDR;
	};

	enum EPrimitiveType
	{
		EDraw_None = 0,
		EDraw_PointList,
		EDraw_LineList,
		EDraw_LineStrip,
		EDraw_TriangleList,
		EDraw_TriangleStrip,
		EDraw_TriangleFan,
	};

}//namespace nexus

#endif //_RENDER_DEFINE_H_