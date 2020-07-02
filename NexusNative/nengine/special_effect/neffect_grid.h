#ifndef _NEXUS_EFFECT_GRID_H_
#define _NEXUS_EFFECT_GRID_H_
#pragma once
#include "neffect_element.h"
#include "quad_face.h"
#include "neffect_vertex_data.h"
#include "../renderer/nrender_dynamic_mesh.h"

namespace nexus
{
	//Ʈ����Ч
	//�ɿ�Ԫ�أ�Ʈ���ڵ�Ĺ켣��Ʈ����������Ӱ�졢Ʈ���ܷ�����Ӱ��
	//-----------------------------------------------------------------------------------
	//Ʈ����Ƭ������
	enum ESFX_GridType
	{
		neffect_grid_plane,				//ƽ��
		neffect_grid_sphere_billboard,	//���ι����
		neffent_grid_cylinder_billboard,	//Բ�������
	};

	class neffect_grid_instance:
		public neffect_element_instance
	{
		friend class neffect_grid;
	public:
		typedef boost::shared_ptr<neffect_grid_instance> ptr;

		virtual void init(boost::shared_ptr<neffect_element> def);

		virtual bool update_render_data(const nviewport& view, float play_time, const matrix44& owner_mat);

		virtual nrender_mesh* get_render_mesh() const
		{
			return m_mesh.get();
		}

		void compute_vertex_info(int x,int y,float time,vector3& pos);
	protected:
		vector2	m_size;	//��ǰ�ߴ�
		color4f	m_color;//��ǰɫ��
		vector3	m_power;//��ǰ���
		//-----------------------------------------------------------------------------------
		vector3	m_view_forward;
		vector3 m_view_up;
		vector3 m_view_right;

		matrix44 m_transform;
		//-----------------------------------------------------------------------------------
		boost::shared_ptr<neffect_grid>		m_template;
		int	m_face_count;	//�����������ε�����
		//-- ��Ⱦ����
		neffect_vertex_data		m_vert_data;		
		render_res_ptr<nrender_dynamic_mesh>	m_mesh;		
	public:
		neffect_grid_instance();
		virtual ~neffect_grid_instance(){};
	};

	//-----------------------------------------------------------------------------------
	class nAPI neffect_grid:
		public neffect_element
	{
	public:
		typedef boost::shared_ptr<neffect_grid> ptr;
		virtual void init(const nstring& name);
		virtual neffect_element_instance::ptr create_instance();
		virtual void update_instance(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time);

		virtual void serialize(narchive& ar);
	public:
		//Ʈ��������
		ESFX_GridType	m_type;
		//Ʈ���س��ȷ����ϸ������
		int		m_length_div;
		//Ʈ���ؿ�ȷ����ϸ������
		int		m_width_div;
		//�����Ĵ����ٶ�
		float	m_wave_speed;
		//�����ڸ�����������
		ndistribution<vector3>::ptr		m_wave_power;
		//������˥���ٶ�
		float	m_attenuation;
		//Ʈ����ɫ����ʱ��ı�Ŀ���
		ndistribution<color4f>::ptr		m_color;
		//Ʈ���ĳߴ���ʱ��ı�Ŀ���
		ndistribution<vector2>::ptr		m_size;
	public:
		neffect_grid();
		~neffect_grid();
		nDECLARE_CLASS(neffect_grid)
	};
};
#endif