/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	July, 2009
*/
#ifndef _NEXUS_SKY_BOX_H_
#define _NEXUS_SKY_BOX_H_
#include "sky_base.h"
#include "../resource/nresource.h"
#include "../mesh/mesh_vertex_data.h"
#include "../renderer/nrender_static_mesh.h"
#include "../renderer/nrender_texture.h"
#include "../material/nmtl_static.h"

namespace nexus
{
	/**
	 *	ֻ����һ��pos stream
	 *	@remarks ��������պУ�ʹ��pos��Ϊcube map����������
	*/
	class nsky_cube_vertex_data
		: public nmesh_vertex_data
	{
	public:
		nsky_cube_vertex_data(void);
		virtual ~nsky_cube_vertex_data(void);

		//!	��ʼ��������ڲ�stream
		void init();

		//--
		virtual size_t get_num_stream() const	{	return 1;}
		virtual vertex_stream* get_stream(size_t stream_index)	
		{
			(void)stream_index;
			return &m_pos_stream;
		}
		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;
	};

	/**
	 *	ʹ��һ��Skybox��һ��Cube Map��Ⱦ�򵥵����Ч��
	*/
	class nsky_box
		: public nsky_component
	{
	public:
		explicit nsky_box(const nstring& name_str);
		virtual ~nsky_box(void);

		//!	�����ڲ�mesh�Ͳ���
		void create(const resource_location& texture_loc);
		virtual nmtl_base* get_material(int lod, int mtl_id);		
		virtual nrender_mesh* get_render_mesh(int lod);		

		virtual void _destroy();
		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nactor* owner);
	
		virtual void _on_device_lost(int param);		
		virtual bool _on_device_reset(int param);		
	private:
		void create_render_mesh();

		render_res_ptr<nrender_static_mesh>	m_mesh;
		nmtl_static							m_mtl;	// ʹ��һ��Cube Map�γ����Diffuse

		nDECLARE_NAMED_CLASS(nsky_box)
	};
}//namespace nexus
#endif //_NEXUS_SKY_BOX_H_