#ifndef _NEXUS_RESOURCE_SCENE_MESH_H_
#define _NEXUS_RESOURCE_SCENE_MESH_H_
#include "nresource.h"

namespace nexus
{
	/**
	*	�ڲ�ʹ�ÿռ�ָ�������̬ģ�ͣ��������������������ĳ���������ģ��
	*/
	class nresource_scene_mesh :
		public nresource
	{
	public:
		nresource_scene_mesh(const nstring& name_str);
		virtual ~nresource_scene_mesh(void);
	};
}//namespace nexus
#endif //_NEXUS_RESOURCE_SCENE_MESH_H_