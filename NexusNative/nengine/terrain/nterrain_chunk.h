/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	April, 2010
*/
#ifndef _NEXUS_TERRAIN_CHUNK_H_
#define _NEXUS_TERRAIN_CHUNK_H_
#include "../actor/nmesh_component.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "nterrain_mtl.h"

namespace nexus
{
	class nterrain_mtl_setup;
	class nterrain_actor;
	class nterrain_chunk_vertex_data;
	class nterrain_deco_layer;
	class nterrain_deco_chunk_base;

	/** ���ο�Meshʹ��Static Mesh��������
	 *	@remarks ����LODѡ��ͬ��Section
	*/
	class nterrain_chunk_mesh_adapter
		: public nrender_mesh
	{
	public:
		nterrain_chunk_mesh_adapter(void)	{}
		virtual ~nterrain_chunk_mesh_adapter(void)	{}

		//! �����ڲ���static mesh�����Լ�����
		void create(const nmesh_vertex_data* vert_data, const index_buffer16* index_data);

		//! ʹ��interlocked tile��������lodʱ����Ӷ��index buffer
		void add_interlocked_tile_lod(const index_buffer16* index_data);

		//! ����chunk����
		void set_chunk(int chunk_size, int terrain_w, int terrain_h);

		//-- ����ӿ�
		virtual nshader_modifier* get_shader_mod()		{	return m_render_mesh->get_shader_mod(); }
		virtual nvertex_factory* get_vertex_factory()	{	return m_render_mesh->get_vertex_factory();}
		virtual size_t get_num_section()				{	return 1;}
		virtual nrender_mesh_section* get_section(size_t section_index);
		virtual bool pre_render()	{	return m_render_mesh->pre_render(); }
		virtual void post_render()	{	return m_render_mesh->post_render(); }

		virtual void draw_setup_effect(nshading_effect* effect_ptr);	

		virtual void update_vertex_buffer(const nmesh_vertex_data* vert_data)
		{
			m_render_mesh->update_vertex_buffer(vert_data);
		}

		void destroy()
		{
			m_render_mesh.reset();
		}

	private:
		render_res_ptr<nrender_static_mesh_indexed>	m_render_mesh;
		vector2	m_chunk_uv_scale;
	};

	/**
	 *	��Ⱦһ�����ο���ʹ�õ�component
	*/
	class nterrain_chunk
		: public nmesh_component
	{
	public:
		typedef boost::shared_ptr<nterrain_chunk> ptr;

		nterrain_chunk(const nstring& name_str);
		virtual ~nterrain_chunk(void);

		//! �����ڲ���Ⱦ�õ�mesh����
		void create(int x, int y, int chunk_size);

		//! �ڸ߶�ͼ�����仯ʱ�����¼���vertex buffer���Լ�bounding box
		void post_heightmap_change(const nrect& region);

		//!	��viz map�ı�ʱ�����¼���index buffer
		void post_vizmap_change(const nrect& region);

		//! ֪ͨ�����ڲ����ʶ���
		void _post_material_create(nterrain_mtl_setup* mtl);

		//! ��ͼ�����÷����仯ʱ�����ڲ����ʶ���
		void _post_layer_change();

		//! ��ͼ���alpha�����仯ʱ�����ڲ����ʶ���
		void _post_layer_alpha_change(const nstring& layer_name, const nrect& region);

		//! ���һ��װ�β�
		void	add_deco_layer(nterrain_deco_layer* layer);

		//ɾ��һ��װ�β�
		void	del_deco_layer(nterrain_deco_layer* layer);

		//װ�β㷢���仯
		void	_post_deco_layer_change();

		//! ����chunk���ǵ��ε��Ǹ���������
		nrect get_map_rect() const
		{
			return nrect(m_hmap_x, m_hmap_y, m_hmap_x+m_chunk_size, m_hmap_y+m_chunk_size);
		}

		//-- ����ӿ�
		virtual nmtl_base* get_material(int lod, int mtl_id);
		
		void get_mesh_element(int lod,vector<mesh_element>& mesh_elements);

		virtual const matrix44& get_world_matrix() const
		{
			return m_owner->get_world_matrix();
		}
		virtual void _update_transform(const object_space& parent_space);
		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);
	private:
		void update_mesh_element();
		void update_vertex_data(nterrain_chunk_vertex_data* vert_buffer, nsize& out_height_range);
		void update_index_buffer(index_buffer16* indices);
		unsigned short get_index(int j, int i) const
		{
			return j*m_chunk_size+i;
		}

		void update_bounds(nsize height_range);
		void create_render_mesh();
	
	private:
		int	m_hmap_x,
			m_hmap_y;
		int	m_chunk_size;

		nterrain_chunk_mesh_adapter	m_render_mesh;
		nterrain_mtl_basic::ptr		m_mtl_low;	// ʹ��nterrain_mtl_basic��Ϊ�;��Ȳ���
		ntexture_splatting::ptr		m_mtl_high;	// ʹ��ntexture_splatting��Ϊ�߾��Ȳ���

		vector<boost::shared_ptr<nterrain_deco_chunk_base> >	m_deco_chunk_list;

		nDECLARE_NAMED_CLASS(nterrain_chunk)
	};
}//namespace nexus

#endif //_NEXUS_TERRAIN_CHUNK_H_