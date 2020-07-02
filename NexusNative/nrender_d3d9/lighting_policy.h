/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_D3D9_LIGHTING_POLICY_H_
#define _NEXUS_D3D9_LIGHTING_POLICY_H_
#include "light_drawing_policy_type.h"

namespace nexus
{
	class light_drawing_policy_type;
	class d3d9_shading_effect;
	/**
	*	������Ⱦ����
	*	@remark ������¹�����
	*		1.һ��ʵ�ֹ��յ�HLSL���룻
	*		2.����drawing policy�������ɵ�macro��
	*		3.֪���Լ�����Щeffect�������Լ�����������ǣ�
	*/
	class lighting_policy
	{
	public:
		lighting_policy(void){	}
		virtual ~lighting_policy(void)	{	}

		static nstring get_name(void)	{	return nstring(_T("light_none"));}
		static void modify_drawing_policy_type(light_drawing_policy_type* dp_type)	{}
	};
}//namespace nexus
#endif //_NEXUS_D3D9_LIGHTING_POLICY_H_