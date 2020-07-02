#ifndef _NEXUS_MATERIAL_BASE_H_
#define _NEXUS_MATERIAL_BASE_H_
#include "ncore.h"
#include "nmtl_tech_shader.h"
#include "../renderer/render_define.h"

namespace nexus
{
	class nshading_effect;

	const TCHAR* const MTL_DEFAULT_TECH_CONST_ALL = _T("MtlDftTechConstAll");
	const TCHAR* const MTL_DEFAULT_TECH_CONST_COLOR = _T("MtlDftTechConstColor");

	/**
	 *	���ʻ���
	 *	@remarks ������pass��shader���Լ�����shader��Ҫ�Ĳ���
	*/
	class nAPI nmtl_base : public nobject
	{
	public:
		typedef boost::shared_ptr<nmtl_base> ptr;

		explicit nmtl_base(const nstring& name_str);
		virtual ~nmtl_base(void);

		//!	��hlsl�ļ�������������
		virtual void create_from_hlsl(const nstring& name_str, const struct resource_location& shader_loc);		

		//! �Ӳ����ļ��д���һ������
		static nmtl_base::ptr create_from_file( const resource_location& loc );

		//! ������ʵ��ļ�
		void save_to_file( const resource_location& loc );

		/**	ȡ�ò��ʵ�͸������
		 *	���ֵ�����ϲ�ά�������κ�һ��Pass��Ҫ͸�������������͸����������
		 */
		virtual enum EBlendType get_blend_type() const	{	return m_blend_type; }

		//!	����͸������
		virtual void set_blend_type(enum EBlendType val)	{	m_blend_type = val;}

		virtual void set_render_state(nshading_effect* effect_ptr);

		virtual void set_two_side( bool btrue) { is_two_side = btrue; }
		virtual bool get_two_side( ) const	{ return is_two_side; }

		virtual bool is_translucent()
		{
			return m_blend_type == ETranslucent || m_blend_type == EAdditive;
		}

		//!	ȡ��ĳ��tech��ʹ�õ�shader
		nmtl_tech_shader::ptr get_shader();

		resource_location	get_shader_loc()	{return m_shader_loc;}

		//!	��ʼ����ĳ��pass��������Ҫ�Ĳ������õ���Ⱦ�õ�effect�У���������Ӧ����Ⱦ״̬
		virtual void setup_effect(nshading_effect* effect_ptr) const 
		{
			(void)effect_ptr;
		};

		//!	�Ƿ��ڲ��õ���������Դ�Ѿ��������
		virtual bool resource_ready() const	{	return true;}

		//!	�ͷ��ڲ�����ʹ�õ�����Դ
		virtual void _destroy()	{}

		const nstring& get_name() const	{	return m_name; }
		void set_name(const nstring& nm){	m_name = nm; }

		virtual void serialize(narchive& ar);

		virtual const nshader_modifier* get_shader_mod()	
		{	
			return &s_shader_modifier; 
		}
	protected:
		virtual void bind_shader_param(nmtl_tech_shader::ptr mtl_shader)	{}

	protected:
		nstring			m_name;
		nmtl_tech_shader::ptr	m_shader;
		resource_location		m_shader_loc;
		EBlendType		m_blend_type;
		bool			is_two_side;
		nshader_modifier s_shader_modifier;
		nDECLARE_VIRTUAL_CLASS(nmtl_base)
	};
}//namespace nexus
#endif //_NEXUS_MATERIAL_BASE_H_