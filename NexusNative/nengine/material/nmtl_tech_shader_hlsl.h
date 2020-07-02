#ifndef _NEXUS_MATERIAL_TECH_SHADER_HLSL_H_
#define _NEXUS_MATERIAL_TECH_SHADER_HLSL_H_
#include "nmtl_tech_shader.h"

namespace nexus
{
	/**
	*	ʹ�ñ�׼��HLSL���붨�����Pass��
	*	@remark ����HLSL�����е�ȫ�ֱ������壬��������ʲ�����
	*	���ʲ�����������ĸ�ʽ����Ϊ��type user_name:sematic������sematic���뿪ͷΪ��MTL_�������硰MTL_DIFFUSE_MAP��
	*/
	class nAPI nmtl_tech_shader_hlsl : public nmtl_tech_shader
	{
	public:
		nmtl_tech_shader_hlsl(const nstring& name_str);
		virtual ~nmtl_tech_shader_hlsl(void);

		virtual void load_from_file(const resource_location& loc);

	private:
		void parse_parameter_from_hlsl();		
		void parse_annotation(const std::string text, material_parameter& param);
	};
}//namespace nexus
#endif //_NEXUS_MATERIAL_TECH_SHADER_HLSL_H_