#ifndef _NEXUS_RENDER_DYNAMIC_MESH_H_
#define _NEXUS_RENDER_DYNAMIC_MESH_H_
#include "nrender_mesh.h"

namespace nexus
{

	class nAPI nrender_dynamic_mesh
		: public nrender_mesh
	{
	public:
		nrender_dynamic_mesh(void):m_draw_limit(-1)
		{	}
		virtual ~nrender_dynamic_mesh(void)	{	}

		virtual void create(enum EPrimitiveType prim_type,
			const nmesh_vertex_data* vert_data, int mtl_id) = 0;

		virtual void update_vertices(const nmesh_vertex_data* vert_data) = 0;		

		virtual void set_draw_limit(int primitive_count)
		{
			m_draw_limit = primitive_count;
		}
	protected:
		int	m_draw_limit;	// vertex buffer�п��ܱ����˺ܶ�primitive�����ǻ���ʱ�����ֻ������ô�ָࣻ��Ϊ-1��ʾ�ж��ٻ����٣�

		nDECLARE_VIRTUAL_CLASS(nrender_dynamic_mesh)
	};
}//namespace nexus

#endif //_NEXUS_RENDER_DYNAMIC_MESH_H_