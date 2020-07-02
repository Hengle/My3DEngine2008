#ifndef _NEXUS_EFFECT_CYLINDER_H_
#define _NEXUS_EFFECT_CYLINDER_H_
#include "neffect_element.h"
#include "ndistribution.h"
#include "neffect_vertex_data.h"
#include "../renderer/nrender_dynamic_mesh.h"


namespace nexus
{
	class neffect_cylinder;

	class neffect_cylinder_instance
		: public neffect_element_instance
	{
	public:
		neffect_cylinder_instance(void)	{}
		virtual ~neffect_cylinder_instance(void)	{}

		virtual void init(shared_ptr<neffect_element> def);
		virtual bool update_render_data(const nviewport& view, float play_time, const matrix44& owner_mat);
		virtual nrender_mesh* get_render_mesh() const;

	private:
		shared_ptr<neffect_cylinder>			m_template;

		//-- ��Ⱦ����
		neffect_vertex_data						m_vert_data;		
		render_res_ptr<nrender_dynamic_mesh>	m_mesh;
	};
	/**
	 *	��ЧԪ�أ�Ͳ�μ�����
	 *	@remarks �����γɱ�׼��Ͳ��Ҳ���Բ������Ȧ����ϰ뾶�ı仯������״������״�����壻��������ǡ������ǵȣ�
	*/
	class nAPI neffect_cylinder
		: public neffect_element
	{
	public:
		typedef boost::shared_ptr<neffect_cylinder> ptr;

		neffect_cylinder(void);
		~neffect_cylinder(void);

		virtual void init(const nstring& name);

		virtual neffect_element_instance::ptr create_instance();
		virtual void update_instance(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time);

		virtual void serialize(narchive& ar);
	public:

		//-- ��̬����
		int		m_div;		// ��Բ�ָܷ���ٴ�
		vector2 m_uv_scale;	// Ĭ���ǽ�һ����ͼ�����ĺ���Ͳ����

		//-- �仯����
		ndistribution<float>::ptr	m_num_circle;				// �������ٸ�������360��
		ndistribution<float>::ptr	m_height;					// �߶ȣ���ֱ���򳤶ȣ�
		ndistribution<float>::ptr	m_top_radius;				// �����뾶
		ndistribution<float>::ptr	m_bottom_radius;			// �ײ��뾶
		ndistribution<float>::ptr	m_div_top_radius_inc;		// �ָ�ʱ���뾶�����ٶ�
		ndistribution<float>::ptr	m_div_bottom_radius_inc;	// �ָ�ʱ�װ뾶�����ٶ�
		ndistribution<float>::ptr	m_div_top_radius_shrink;	// �ָ�ʱ��ÿ��һ���㣬���뾶�����̶�,�������������,��â�ǵ�
		ndistribution<float>::ptr	m_div_bottom_radius_shrink;
		ndistribution<float>::ptr	m_div_top_rise;				// ÿ�ηָ��߶������ٶ�
		ndistribution<float>::ptr	m_div_bottom_rise;			// ÿ�ηָ����߶������ٶ�
		ndistribution<color4f>::ptr	m_top_color;				// ����������ɫ
		ndistribution<color4f>::ptr	m_bottom_color;				// �ײ�������ɫ

		nDECLARE_CLASS(neffect_cylinder)
	};
}//namespace nexus

#endif //_NEXUS_EFFECT_CYLINDER_H_