#ifndef _NEXUS_TERRAIN_DECO_SETUP_H_
#define _NEXUS_TERRAIN_DECO_SETUP_H_

#include "ncore.h"
#include "../framework/nactor.h"
#include "../resource/nresource.h"
#include "../renderer/nrender_resource.h"
#include "../framework/world_define.h"

namespace nexus
{
	struct terrain_deco_object
	{
		//ʵ����λ��
		vector3	m_pos;
		//ʵ������ת
		vector3 m_rotation;
		//ʵ��������
		vector3 m_scaline;
		terrain_deco_object():m_pos(0,0,0),m_rotation(0,0,0),m_scaline(1.0f,1.0f,1.0f)
		{}
		void	ToMatrix(matrix44& matrix)
		{
			mat_set_scaling(matrix,m_scaline.x,m_scaline.y,m_scaline.z);
			mat_rotate_Z(matrix, m_rotation.z);
			mat_rotate_X(matrix, m_rotation.x);
			mat_rotate_Y(matrix, m_rotation.y);
			mat_translate(matrix,m_pos);
		}
	};

	class nAPI nterrain_deco_layer : public nobject
	{
	public:
		typedef shared_ptr<nterrain_deco_layer> ptr;
		//===================================================================================
		//ִ��һ�α�ˢ�����ݱ�ˢ�����������򳡾������actor.����:/��ˢ����x/��ˢ����y/��ˢ�İ뾶/
		void	do_brush(float x,float y,float radius);
		//===================================================================================
		//��ȡ�����ò������
		const	nstring&	get_name()			{return m_name;}
		void	set_name(const nstring& value)	{m_name=value;}

		//��ȡ�����õ�ǰ���ģ����Դ����������˲�ͬ����Դ�����������еĶ���ģ��
		const	resource_location&	get_resource_loc()	{return m_res;}
		void	set_resource_loc(const resource_location& loc);

		//��ȡ������Ĭ�ϵ������תֵ����ֵ������󴴽�ʱ��Y��������ת.�޸ĸ�ֵ��Ӱ�����еĶ���
		float	get_random_rotation()	{return m_random_rotation;}
		void	set_random_rotation(float value)	{m_random_rotation=value;}

		//��ȡ������������ŵ����ֵ����Сֵ���޸���Щֵ��Ӱ�����еĶ���
		float	get_max_random_scale()	{return m_max_random_scale;}
		void	set_max_random_scale(float value)	{m_max_random_scale=value;}

		float	get_min_random_scale()	{return m_min_random_scale;}
		void	set_min_random_scale(float value)	{m_min_random_scale=value;}

		//��ȡ�����ñ�ˢ���ܶ�,�޸ĸ�ֵ��Ӱ�����еĶ���
		float	get_brush_density()	{return m_density;}
		void	set_brush_density(float value)	{m_density=value;}

		//��ȡ���������������,�޸ĸ�ֵ��Ӱ�����еĶ���
		float	get_random_seed()	{return m_random_seed;}
		void	set_random_seed(float value)	{m_random_seed=value;}

	private:

		//�������
		nstring		m_name;

		//�ò�ʹ�õ�ģ��
		resource_location	m_res;

		//Ĭ�ϵ������תֵ����,���ȡֵΪ30��������-30~30��֮����ת��ע�⣬����ʹ�õ��Ƕȣ����ǻ��ȡ�
		float	m_random_rotation;

		//Ĭ�ϵ��������ֵ
		float	m_min_random_scale;
		float	m_max_random_scale;

		//��ˢ���ܶ�
		float	m_density;

		//ʵ����࣬��������ʵ��֮��Ļ�����࣬����ʱ��ʵ�������������Ϊ׼�����ʵ��֮���Կ��ܷ�����ײ
		float	m_grid_size;

		//���������
		int		m_random_seed;

		//�ò�����ʵ���б�
		vector<terrain_deco_object>	m_object_list;

		//ÿ��ʵ����Ӧ��actor
		vector<nactor::ptr>	m_actor_list;

	public:
		nDECLARE_CLASS(nterrain_deco_layer);
		nterrain_deco_layer(void);
		virtual ~nterrain_deco_layer(void);
	};
}//namespace nexus
#endif //_NEXUS_TERRAIN_DECO_SETUP_H_