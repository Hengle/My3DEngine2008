/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_D3D9_LIGHT_DRAWING_POLICY_TYPE_H_
#define _NEXUS_D3D9_LIGHT_DRAWING_POLICY_TYPE_H_

#include "drawing_policy_type.h"
#include "util.h"

namespace nexus
{
	/**
	 *	������Ⱦ��ʹ�õ�drawing policy type
	 *	@remark �ڱ�׼dp type�������ˡ�lighting.hlsl������ϴ���
	*/
	class light_drawing_policy_type :
		public drawing_policy_type
	{
	public:
		light_drawing_policy_type(const nstring& _dp_name):drawing_policy_type(_dp_name)
		{	}
		virtual ~light_drawing_policy_type(void)	{	}

		virtual bool get_include_code(const char* include_name, std::string& out_code)
		{	
			if(_stricmp(include_name, "lighting.hlsl") == 0)
			{
				out_code = m_light_code;
				return true;
			}

			return false; 
		}

		virtual void load_shader_file()
		{			
			nstring dp_name = _T("shader_d3d9/drawing_light.fx");
			load_shader_source(dp_name, shader_source);
			load_shader_source(m_light_policy, m_light_code);
		}

	public:
		nstring	m_light_policy;
		//nstring	m_shadow_policy;

	private:
		std::string	m_light_code;
		//std::string	m_shadow_code;
	};
}//namespace nexus

#endif //_NEXUS_D3D9_LIGHT_DRAWING_POLICY_TYPE_H_