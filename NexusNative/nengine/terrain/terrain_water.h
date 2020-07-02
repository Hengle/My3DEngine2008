#ifndef _NEXUS_WATER_COMPONENT_H_
#define _NEXUS_WATER_COMPONENT_H_
#include "../actor/nprimitive_component.h"
#include "../mesh/mesh_vertex_data.h"
#include "../renderer/nrender_mesh.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "../renderer/nrender_resource.h"
#include "../renderer/nrender_proxy.h"
#include "nwater_mtl.h"

/**
 *	----ˮ����ȾMesh����֯����----
 *	.�����������λ���Ϊ��С��ȵ�chunk,chunk�Ĵ�С���Ը��ݳ��������,ˮ�������Ŀ��ԼӴ�chunk�ߴ�;
 *	.���ڵ��ε�ÿ��chunk, ������߶�ͼ, ����߶ȵ���ָ����ˮ��߶�,����Ҫ��Ⱦˮ��mesh;
 *	.����ˮ��mesh���Թ���һ��vertex buffer, ��vertex factory�ж�ȡterrrain heightmap texture��ȡ�õ���߶�;
 *	.ʹ�õ���߶Ⱥ�ˮ��߶ȼ����͸����˥��, ������ˮ���Ե͸����Ч��;
 *	.����chunk��ɳ�һ��quad tree, ��water component����;
 
 *	.----ˮ��Render Mesh��ʵ��----------------
 *	.���е���chunk����ʹ��һ��static render mesh, ÿ��chunk����һ��section;
 *	.ʹ��һ��render mesh������������װ��������static render mesh, ѡ����ȷ��section�ύ����Ⱦ��;
*/
namespace nexus
{
	class nterrain_actor;
	class nterrain_water;

	/**
	 *	ʹ��һ��(0,0)-(1,1)�ľ���vertex��������terrain heightmap
	 *	@remarks ˮ����Ҫ���ݵ��θ߶ȼ���͸���ȣ�����Ҫ������vertex factory
	*/
	class nterrain_water_vertex_data
		: public nmesh_vertex_data
	{
	public:
		nterrain_water_vertex_data(void)	{}
		virtual ~nterrain_water_vertex_data(void)	{}

		virtual size_t get_num_stream() const	{	return 1; }
		virtual vertex_stream* get_stream(size_t stream_index)
		{
			(void)stream_index;
			return &m_pos_stream;
		}

		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;

		void init(size_t chunk_num_rows, size_t chunk_num_cols);
	};

	/**
	 *	��װterrain water�Ĺ���static mesh����
	 *	@remarks ѡȡ�Լ���Ӧ��һ��mesh section; �����ȷ��effect����.
	*/
	class nterrain_water_render_mesh
		: public nrender_mesh
	{
	public:
		nterrain_water_render_mesh(void);
		virtual ~nterrain_water_render_mesh(void);

		virtual nvertex_factory* get_vertex_factory()	{	return m_share_mesh->get_vertex_factory(); }
		virtual size_t get_num_section()				{	return 1;}
		virtual nrender_mesh_section* get_section(size_t section_index)
		{	
			(void)section_index;
			return m_share_mesh->get_section(m_section_id);
		}

		virtual void draw_setup_effect(nshading_effect* effect_ptr);

		void create(const nrect& rc, nrender_static_mesh_indexed* share_mesh, const index_buffer16& tri_list, nterrain_water* water_ptr);

	private:
		nrender_static_mesh_indexed*	m_share_mesh;
		size_t	m_section_id;
		nterrain_water*	m_water;

		vector2	m_quad_bias;
		vector2 m_quad_scale;
		vector2 m_texel_size;
	};

	struct water_render_proxy
		: public nrender_proxy
	{
		virtual const matrix44& get_world_matrix() const;
		virtual int get_render_lod() const					{	return 0; }
		virtual enum EDepthGroup get_depth_group() const	{	return EDG_World; }	
		virtual nrender_mesh* get_render_mesh(int lod) const	{	return const_cast<nterrain_water_render_mesh*>(&chunk_mesh);}
		virtual nmaterial_base* get_material(int lod, int mtl_id) const;
		virtual transparent_flag get_transparent_flag() const;
		virtual const box_sphere_bounds& get_bounds() const				{	return bounds;}
		virtual bool accept_dynamic_light() const	{	return false;}
		virtual bool cast_dynamic_shadow() const	{	return false;}
		virtual hit_id get_hit_proxy_id() const;

		water_render_proxy():owner(NULL)
		{}

		nterrain_water_render_mesh chunk_mesh;
		nterrain_water*		owner;
		box_sphere_bounds	bounds;
	};

	/**
	 *	����ָ����chunk��С����terrain�����з�Ϊquad tree������leaf node������Ƿ���Ҫ����water mesh
	*/
	struct water_quad_node
	{
		typedef boost::scoped_ptr<water_quad_node> ptr;

		water_quad_node(void);
		void create(nrect rc, nterrain_water* owner, size_t chunk_size);
		void render(const nviewport& view);

		nrect					zone;
		water_quad_node::ptr	children[4];
		boost::scoped_ptr<water_render_proxy>
								render_obj;
		bool					is_leaf;
	};

	class nterrain_water
		: public nprimitive_component
	{
	public:
		explicit nterrain_water(const nstring& name_str);
		virtual ~nterrain_water(void);

		void create(int height, size_t chunk_size);
		void create_material_basic(const resource_location& texture_loc);
		void create_material_natural(int render_target_w, int render_target_h,
			const resource_location& detail_map, const resource_location& bump_map);

		virtual nmaterial_base* get_material(int lod, int mtl_id);
		virtual transparent_flag get_transparent_flag() const	{	return m_trans_flag;}
		virtual const matrix44& get_world_matrix() const;

		virtual void render(const nviewport& view);

		int get_water_height() const	{	return m_water_height; }
		float get_water_height_scaled();
		nterrain_actor* get_terrain_actor();
		nrender_static_mesh_indexed* get_share_mesh()	{	return m_share_mesh.get(); }
		nrender_heightmap*	get_heightmap_tex()			{	return m_heightmap_tex; }

		void get_water_bounds(const nrect& rc, box_sphere_bounds& out_bounds);

		virtual void _destroy();
		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nactor* owner);
	private:
		//--
		nheight_map16*		m_heightmap;
		nrender_heightmap*	m_heightmap_tex;
		int					m_water_height;
		size_t				m_chunk_size;

		//-- render data
		transparent_flag	m_trans_flag;
		render_res_ptr<nrender_static_mesh_indexed>
						m_share_mesh;

		water_quad_node::ptr	m_root_node;

		typedef std::vector<nwater_mtl::ptr>	mtl_vector;
		mtl_vector		m_lod_mtl;	// ÿ��LOD��Ӧһ��Material

		nDECLARE_NAMED_CLASS(nterrain_water)
	};
}//namespace nexus

#endif //#ifndef _NEXUS_WATER_COMPONENT_H_