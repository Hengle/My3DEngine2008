#ifndef _NEXUS_SIMPLE_MESH_H_
#define _NEXUS_SIMPLE_MESH_H_
#include "ncore.h"

namespace nexus
{
	/*	һ��static mesh�ļ�ģ��
	 *	@remarks ����������ײ��⡢occlusion cull��shadow��
	*/
	struct nsimple_mesh
	{
		typedef boost::shared_ptr<nsimple_mesh> ptr;

		std::vector<vector3>		vert_buffer;
		std::vector<unsigned short>	index_buffer;
	};
}//namespace nexus

#endif //_NEXUS_SIMPLE_MESH_H_