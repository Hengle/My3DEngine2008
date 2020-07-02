#ifndef _NEXUS_TERRAIN_STATIC_MESH_DECO_CHUNK_H_
#define _NEXUS_TERRAIN_STATIC_MESH_DECO_CHUNK_H_

#include "nterrain_deco_chunk_base.h"
#include "../resource/nresource_static_mesh.h"
#include "../Actor/nstatic_mesh_component.h"

namespace nexus
{
	class static_deco_object;
	class nterrain_static_mesh_deco_layer;
	class nterrain_static_mesh_deco_chunk;

	//��̬ģ��װ�β�Ķ���ʵ��
	class nAPI static_deco_object:public terrain_deco_object
	{
	public:
		void	move(const vector3& pos,const vector3& rot,const vector3& scale)
		{
			m_pos=pos;	m_rotation=rot;	m_scaling=scale;
			update_matrix();
		}
		virtual void update_matrix()
		{
			to_matrix(m_matrix);
		}
		virtual void get_mesh_element(int lod,vector<mesh_element>& mesh_elements,nmesh_component* prim_comp);
	private:
		nterrain_deco_layer*	m_owner;
		matrix44	m_matrix;
		nstatic_mesh_component::ptr		m_compnent;
	public:
		static_deco_object(nterrain_deco_layer* owner);
	};

	//��̬ģ��ʵ��װ�β���ÿһ��chunk�еĴ���
	class nAPI nterrain_static_mesh_deco_chunk:public nterrain_deco_chunk_base
	{
	public:
		//��ָ�����괦���һ���µ�ʵ��
		virtual terrain_deco_object::ptr	add_instance(int x,int z,float wx,float wz);

		//����Ҫ��Ⱦ��mesh-element�����б�
		virtual void	get_mesh_element(int lod,vector<mesh_element>& mesh_elements,nmesh_component* comp);

		//�ֲ�ͼ�����ı�ʱ�Ĵ���
		virtual bool	on_density_map_changed();
		virtual bool	on_density_map_changed(nterrain_deco_layer* layer,const nrect& region);
	public:
		nterrain_static_mesh_deco_chunk(nterrain_deco_layer* owner,nterrain_chunk* terrain_chunk);
		virtual ~nterrain_static_mesh_deco_chunk();
	};

	//===================================================================================

	//��̬ģ��ʵ��װ�β�
	class nAPI nterrain_static_mesh_deco_layer:public nterrain_deco_layer
	{
	public:
		typedef boost::shared_ptr<nterrain_static_mesh_deco_layer>	ptr;

		nDECLARE_CLASS(nterrain_static_mesh_deco_layer);
		nterrain_static_mesh_deco_layer();
		virtual ~nterrain_static_mesh_deco_layer();

		//����һ��deco chunk
		virtual boost::shared_ptr<nterrain_deco_chunk_base> create_deco_chunk(nterrain_chunk* terrain_chunk,int x,int y,int width,int height);

		virtual void	set_resource_loc(const resource_location& loc)
		{
			m_resource=loc;
			m_mesh->load_from_file(loc);
		}
		nresource_static_mesh::ptr	get_mesh()	{return m_mesh;}

		//���л�
		virtual void	serialize(narchive& ar);
	protected:
		//��̬ģ��
		nresource_static_mesh::ptr	m_mesh;
	};
	
};

#endif
