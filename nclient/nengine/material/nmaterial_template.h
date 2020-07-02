/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NMATERIAL_TEMPLATE_H_
#define _NMATERIAL_TEMPLATE_H_
#include "../resource/nresource.h"
#include "material_parameter.h"
#include "../renderer/render_define.h"

namespace nexus
{
	/**
	 *	����ģ��
	 *	@remark �ӳ�������������Ӧһ��shader���룻������������������Ӧһ����Ʒ���ʵأ����硰�����Ľ��������ߡ�����Ľ������ȣ�
	 *		�����Խ�һ����ͨ��ָ�����������������ϸ�ڣ����硰diffuse��ͼ������normal map����
	*/
	class nmaterial_template :
		public nresource
	{
	public:
		typedef boost::intrusive_ptr<nmaterial_template> ptr;

		explicit nmaterial_template(const nstring& name_str):nresource(name_str),
			m_trans_type(ETrans_Opaque)
		{	}
		virtual ~nmaterial_template(void)		{	}

		const std::string& get_shader_code() const	{	return m_shader_code; }

		const material_parameter* get_parameter_by_name(const std::string& name)
		{
			for(size_t i=0; i<m_parameter_array.size();i++)
			{
				if(m_parameter_array[i].name == name)
					return &m_parameter_array[i];
			}

			return NULL;
		}

		const vector<material_parameter>& get_parameter_array() const
		{
			return m_parameter_array;
		}
	
		enum ETransparentType get_trans_type() const			{	return m_trans_type; }
		void set_trans_type(enum ETransparentType trans_type)	{	m_trans_type = trans_type;}
	protected:
		std::string					m_shader_code;
		vector<material_parameter>	m_parameter_array;

		enum ETransparentType	m_trans_type;
	};
}//namespace nexus
#endif//_NMATERIAL_TEMPLATE_H_