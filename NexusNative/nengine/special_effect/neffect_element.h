#ifndef _NEXUS_EFFECT_ELEMENT_H_
#define _NEXUS_EFFECT_ELEMENT_H_
#include "ncore.h"
#include "../renderer/nrender_mesh.h"
#include "../material/nmtl_static.h"
#include "../framework/view_info.h"
#include "ndistribution.h"
#include "nsub_uv.h"

namespace nexus
{
	class neffect_element;

	/**
	 *	��������ЧԪ�ص�ʵ�����ݣ���speclial effect instance����
	 *	@�������һ����ЧԪ�صĵ�ǰʱ���״̬���Լ���Ⱦ�õ���Դ��
	 *	@see class nspecial_effect_instance
	*/
	class neffect_element_instance
	{
	public:
		typedef boost::shared_ptr<neffect_element_instance> ptr;

		neffect_element_instance(void):m_pos(0,0,0),m_rot(0,0,0)
		{			
			m_active = false;
		}

		virtual ~neffect_element_instance(void)	{}

		virtual void init(shared_ptr<neffect_element> def);
		
		//-- �����ڲ�Mesh����Ⱦ���ݣ��ⲿȷ�϶���ɼ�����Ҫ��Ⱦ��ʱ���á�
		virtual bool update_render_data(const nviewport& view, float play_time, const matrix44& owner_mat) = 0;

		virtual nrender_mesh* get_render_mesh() const = 0;
		virtual nmtl_base* get_material(int mtl_id) const;		

	public:
		bool				m_active;		// �Ƿ��Ѿ���������ڲ��ţ�		
		nmtl_base::ptr		m_mtl;

		vector3		m_pos;
		vector3		m_revolution;
		vector3		m_rot;
	};

	/**
	 *	��ЧԪ�ض������
	 *	@remarks ������ЧԪ�صľ�̬���ݣ������ɱ༭���༭���ɣ���resource�������
	 *	@see class nresource_special_effect
	*/
	class nAPI neffect_element
		: public nobject
	{
	public:
		typedef boost::shared_ptr<neffect_element> ptr;

		neffect_element(void);
		virtual ~neffect_element(void);

		virtual void init(const nstring& name);

		virtual neffect_element_instance::ptr create_instance() = 0;

		// ֻ������Ч�ڲ�״̬����������Ⱦ��Դ
		virtual void update_instance(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time) = 0;

		//��ȡ����
		nmtl_base::ptr get_material() const
		{
			return m_mtl;
		}

		//���ò�������
		void set_material_texture(resource_location loc)
		{
			m_Texture=loc;
			if(m_mtl.get()!=NULL && m_Texture.is_valid())
			{
				m_mtl->set_texture2d_parameter("MTL_DiffuseMap",m_Texture);
			}
		}

		//�Ƿ���ʾ
		bool	is_visible(){return m_visible;}

		//�����Ƿ���ʾ
		void	set_visible(bool v){m_visible=v;}

		//��ȡ����
		const nstring& get_name() const	{	return m_name;}

		//���л�
		virtual void serialize(narchive& ar);

	public:
		//-- ��̬����
		nmtl_static::ptr	m_mtl;

		//���ӵĲ�������
		resource_location	m_Texture;

		//ϸ������
		nsub_uv		m_sub_uv;

		nstring		m_name;
		float		m_start_time;	// ��������Ч�����˶��֮��Ų����Լ����Ԫ��
		vector3		m_init_pos;
		vector3		m_init_revolution;		// ��ת��todo���Ƿ������ת��
		vector3		m_init_rot;	//��ת���ڹ����ģʽ�£���תֻ��һ��ת��������

		//�Ƿ���ʾ
		bool		m_visible;

		//-- �仯����
		//----λ�á���ת�ٶ�
		ndistribution<vector3>::ptr	m_vel;			//	�ƶ��ٶ�		
		ndistribution<vector3>::ptr	m_revolution_vel;	//	��ת�ٶ�
		ndistribution<vector3>::ptr	m_rotate_vel;	//	��ת�ٶ�
		float	m_gravity;		//�������ٶȣ�0��ʾ��������Ӱ��

	protected:
		void init_instance_base(neffect_element_instance::ptr inst);
		bool update_instance_base(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time);

		nDECLARE_VIRTUAL_CLASS(neffect_element)
	};
}//namespace nexus

#endif //_NEXUS_EFFECT_ELEMENT_H_