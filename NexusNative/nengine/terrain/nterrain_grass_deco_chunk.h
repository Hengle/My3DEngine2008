#ifndef _NEXUS_TERRAIN_GRASS_DECO_CHUNK_H_
#define _NEXUS_TERRAIN_GRASS_DECO_CHUNK_H_

#include "nterrain_deco_chunk_base.h"
#include "../mesh/mesh_lod.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "nterrain_deco_grass.h"

namespace nexus
{
	//��̬ģ��װ�β�Ķ���ʵ��
	class nAPI grass_deco_object:public terrain_deco_object
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
		grass_deco_object(nterrain_deco_layer* owner)
		{
			m_owner=owner;
		}
	public:
		nterrain_deco_layer*	m_owner;
		matrix44	m_matrix;
	};	

	//��Ҷװ�β���ÿһ��chunk�еĴ���
	class nAPI nterrain_grass_deco_chunk:public nterrain_deco_chunk_base
	{
	public:
		//��ָ�����괦���һ���µ�ʵ��
		virtual terrain_deco_object::ptr	add_instance(int x,int z,float wx,float wz);

		//����Ҫ��Ⱦ��mesh-element�����б�
		virtual void	get_mesh_element(int lod,vector<mesh_element>& mesh_elements,nmesh_component* comp);

		//�ֲ�ͼ�����ı�ʱ�Ĵ���
		virtual bool	on_density_map_changed();
		virtual bool	on_density_map_changed(nterrain_deco_layer* layer,const nrect& region);

		//���ò���
		virtual void	set_material(nmtl_base::ptr material);

		//������Ƭ����
		virtual void	set_cross(bool value)	{m_grass_buffer->set_cross(value);}

		//�豸��ʧʱ�Ĵ���
		virtual void on_device_lost()	{m_grass_buffer->on_device_lost();}
		//�豸����ʱ�Ĵ���
		virtual void on_device_reset()	{m_grass_buffer->on_device_reset();}

	private:
		grass_deco_buffer::ptr	m_grass_buffer;
	public:
		nterrain_grass_deco_chunk(nterrain_deco_layer* owner,nterrain_chunk* terrain_chunk);
		~nterrain_grass_deco_chunk();
	};

	//===================================================================================
	//��Ҷװ�β�.
	//�����е����ŵ�x-y������Ϊ��Ҷ��Ƭ�Ŀ�Ⱥ͸߶ȣ�z��������
	class nAPI nterrain_grass_deco_layer:public nterrain_deco_layer
	{
	public:
		typedef boost::shared_ptr<nterrain_grass_deco_layer>	ptr;

		nDECLARE_CLASS(nterrain_grass_deco_layer);
		nterrain_grass_deco_layer();
		virtual ~nterrain_grass_deco_layer();

		//����һ��deco chunk
		virtual boost::shared_ptr<nterrain_deco_chunk_base> create_deco_chunk(nterrain_chunk* terrain_chunk,int x,int y,int width,int height);

		//ÿ��loc��һ������
		virtual void	set_resource_loc(const resource_location& loc);

		//��ȡ��������Ƭ�Ŀ��
		virtual float	get_width()	{return m_width;}
		virtual void	set_width(float value)	{m_width=value;rebuild();}

		//��ȡ��������Ƭ�ĸ߶�
		virtual float	get_height()	{return m_height;}
		virtual void	set_height(float value)	{m_height=value;rebuild();}

		//��ȡ�������Ƿ���ʮ�ֽ�����Ƭ
		virtual bool	get_cross()		{return m_cross;}
		virtual void	set_cross(bool value);

		//���л�
		virtual void	serialize(narchive& ar);
	protected:
		nmtl_base::ptr	m_material;
		float	m_width;
		float	m_height;
		bool	m_cross;
	};
	
};

#endif