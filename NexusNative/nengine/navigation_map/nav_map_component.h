/**
*	Nexus Engine
*
*	Author: zewoo
*	Date:	2010-03-27
*/

#ifndef _NEXUS_ENGINE_NAV_MAP_COMPONENT_H_
#define _NEXUS_ENGINE_NAV_MAP_COMPONENT_H_
#include "../actor/nmesh_component.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "../material/nmtl_static.h"

namespace nexus
{	
	class nav_map_actor;
	class nav_map_vertex_data;

	/**
	* ����ͼ���������ڵ��εķֿ���Ⱦ��ʽ�����������˵���ͼ�е�����
	*/
	class nAPI nav_map_component :
		public nmesh_component
	{
	public:
		typedef boost::shared_ptr<nav_map_component> ptr;

		nav_map_component(const nstring& name_str);
		virtual ~nav_map_component(void);

		// ��ʼ��������VertBuffer & IndexBuffer����
		void create(nav_map_actor* nav, int x, int y, int chunk_size, float cull_dist);
		// �߶�ͼ�����仯�����¶�������
		void post_heightmap_change(const nrect& region);

		// ���ػ���ӿ�
		virtual void _destroy();

		virtual nmtl_base* get_material(int lod, int mtl_id);		

		virtual const matrix44& get_world_matrix() const;
		virtual void _update_transform(const object_space& parent_space);

		virtual void serialize(narchive& ar);

		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);

	private:
		void update_vertex_data(nav_map_vertex_data* vert_buffer, nsize& out_height_range);
		void update_index_buffer(index_buffer16* indices);

		unsigned short get_index(int j, int i) const
		{
			return j*m_chunk_size+i;
		}

		void update_bounds(nsize height_range);
		void create_render_mesh();

	private:
		render_res_ptr<nrender_static_mesh_indexed>	m_mesh_ptr;
		int	m_hmap_x,
			m_hmap_y;
		int	m_chunk_size;

		// ��Ⱦ������ɫ�õĲ���
		nmtl_static	m_mtl;

		nDECLARE_NAMED_CLASS(nav_map_component)
	};

} // end of namespace nexus

#endif // end of _NEXUS_ENGINE_NAV_MAP_COMPONENT_H_