/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_MATERIAL_SCRIPT_H_
#define _NEXUS_MATERIAL_SCRIPT_H_

#include "nmaterial_template.h"

namespace nexus
{
	/**
	 *	ʹ�ñ�׼��HLSL���붨����ʣ�
	 *	@remark ����HLSL�����е�ȫ�ֱ������壬��������ʲ�����
	 *	���ʲ�����������ĸ�ʽ����Ϊ��type user_name:sematic������sematic�����ԡ�MTL_����ͷ�����硰MTL_DIFFUSE_MAP��
	*/
	class nmaterial_script :
		public nmaterial_template
	{
	public:
		nmaterial_script(const nstring& name_str);
		virtual ~nmaterial_script(void);

		virtual void load_from_file(const resource_location& loc);
		virtual bool ready() const
		{
			// todo
			return true;
		}

	private:
		void parse_material_parameter_from_HLSL();
		void prase_config_from_HLSL();
	};
}//namespace nexus
#endif //_NEXUS_MATERIAL_SCRIPT_H_