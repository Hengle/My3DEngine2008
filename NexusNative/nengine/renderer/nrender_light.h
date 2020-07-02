/**
*	Nexus Engine - ��Ⱦģ��ʹ�õĵƹ�ӿ�
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_RENDER_LIGHT_H_
#define _NEXUS_RENDER_LIGHT_H_
#include "ncore.h"

namespace nexus
{

	class nlight_component;

	enum ELightType
	{
		ELight_None,
		ELight_Point,
		ELight_Directional,		
		//Elight_Spot,
		//ELight_SkyLight,

		ELight_Max
	};

	/**
	*	��Ⱦϵͳʹ�õĵƹ�ӿ�
	*	@remark ����Ⱦϵͳ����Light�����ݣ���Ҫ�����ƹ����͡���ɫ������������Լ��ж�Light�Ƿ�Ӱ�쳡����ĳ������
	*/
	struct nAPI nrender_light_proxy : public nobject
	{
		nlight_component*	m_owner;
		enum ELightType		m_type;
		color4f				m_ambient_color;
		color4f				m_diffuse_color;

		nrender_light_proxy(void):m_owner(NULL),m_type(ELight_None)
		{}
		nrender_light_proxy(nlight_component* owner, enum ELightType lt)
			:m_owner(owner), m_type(lt), 
			m_ambient_color(0,0,0,0), 
			m_diffuse_color(1,1,1,1)
		{}

		virtual ~nrender_light_proxy()
		{}

		virtual void serialize(narchive& ar)
		{
			nobject::serialize(ar);
			nserialize_enum_as_int(ar, m_type, _T("m_type"));
			nSERIALIZE(ar, m_ambient_color);
			nSERIALIZE(ar, m_diffuse_color);
		}

		nDECLARE_VIRTUAL_CLASS(nrender_light_proxy)
	};

	struct nAPI npoint_light_proxy
		: public nrender_light_proxy
	{
		vector3	m_position;
		vector3	m_attenuation;

		npoint_light_proxy(void)
		{}
		npoint_light_proxy(nlight_component* owner):nrender_light_proxy(owner, ELight_Point),
			m_position(0,0,0), m_attenuation(1,0,0)
		{}

		virtual void serialize(narchive& ar)
		{
			nrender_light_proxy::serialize(ar);
			nSERIALIZE(ar, m_position);
			nSERIALIZE(ar, m_attenuation);
		}

		nDECLARE_CLASS(npoint_light_proxy)
	};

	struct nAPI ndirectional_light_proxy
		: public nrender_light_proxy
	{
		vector3	m_direction;

		ndirectional_light_proxy(void)
		{}
		ndirectional_light_proxy(nlight_component* owner):nrender_light_proxy(owner, ELight_Directional),
			m_direction(0,-1,0)
		{}

		virtual void serialize(narchive& ar)
		{
			nrender_light_proxy::serialize(ar);
			nSERIALIZE(ar, m_direction);			
		}

		nDECLARE_CLASS(ndirectional_light_proxy)
	};
}//namespace nexus

#endif //_NEXUS_RENDER_LIGHT_H_