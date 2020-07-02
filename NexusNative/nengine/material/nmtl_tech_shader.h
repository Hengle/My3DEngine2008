#ifndef _NEXUS_MATERIAL_TECH_SHADER_H_
#define _NEXUS_MATERIAL_TECH_SHADER_H_
#include "../resource/nresource.h"
#include "material_parameter.h"

namespace nexus
{
	/**
	 *	���ʵ���Pass��Ҫ��Shader�ű�
	 *	@remarks �������Shader���룬�Լ���Ҫ��Щ����
	*/
	class nAPI nmtl_tech_shader : public nresource
	{
	public:
		typedef boost::intrusive_ptr<nmtl_tech_shader> ptr;

		nmtl_tech_shader(const nstring& name_str);
		virtual ~nmtl_tech_shader(void);

		const std::string& get_shader_code() const	{	return m_shader_code; }
		size_t get_num_param() const	{	return m_param_array.size();}
		const material_parameter& get_param(size_t i) const;

	protected:
		typedef std::vector<material_parameter> st_param_array;

		std::string		m_shader_code;	// shader���룬��ΪD3D�ײ�ʹ��char*��������������std::string
		st_param_array	m_param_array;	// ���ʲ������������
	};
}//namespace nexus
#endif //_NEXUS_MATERIAL_TECH_SHADER_H_