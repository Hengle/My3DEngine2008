#include "StdAfx.h"
#include "nterrain_deco_layer.h"

namespace nexus
{
	nDEFINE_CLASS(nterrain_deco_layer,nobject);
	//-----------------------------------------------------------------------------------
	nterrain_deco_layer::nterrain_deco_layer(void)
	{
		m_random_rotation=0.0f;	m_max_random_scale=1.0f;	m_min_random_scale=1.0f;	m_density=1.0f;		m_random_seed=0;
	}

	nterrain_deco_layer::~nterrain_deco_layer(void)
	{
	}

	void nterrain_deco_layer::do_brush( float x,float y,float radius )
	{
		//�����ܶȺ����ֵ�����ˢ��Χ�ڿ��Բ�������actor
	}

	void nterrain_deco_layer::set_resource_loc( const resource_location& loc )
	{
		m_res=loc;
		//�޸��Ѿ�������actor
	}
}//namespace nexus