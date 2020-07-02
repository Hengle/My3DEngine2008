#ifndef _NEXUS_MATERIAL_TECH_H_
#define _NEXUS_MATERIAL_TECH_H_
#include "ncore.h"
#include "nmtl_tech_shader.h"
#include "nrender_state.h"

namespace nexus
{
	class nshading_effect;
	/**
	 *	���ʵ�һ��tech������shader��render state�����
	 *	@remarks ֻ����render state�����ܿ���shader�꣬����ÿ��mtl techʵ����Ҫ������Ⱦ��shader effect��̫���ˡ�
	*/
	class nAPI nmtl_tech : public nobject
	{
	public:
		typedef boost::shared_ptr<nmtl_tech> ptr;

		explicit nmtl_tech(const nstring& name_str);
		virtual ~nmtl_tech(void);
		
		const nstring& get_name() const				{	return m_name;}
		nmtl_tech_shader::ptr get_shader() const	{	return m_shader_res;}
		
		void set_render_state(enum ERenderState es, int value);		
		void set_render_state(enum ERenderState es, float value);		
		void enable_two_side();
		void enable_alpha_mask(int alpha_mask);
		void enable_alpha_blend();
		void enable_additive_blend();

		bool is_two_side() const		{	return m_two_side;}
		bool is_alpha_masked() const	{	return m_alpha_masked;}

		void set_effect_state(nshading_effect* eft);
	public:
		void _init(nmtl_tech_shader::ptr shader);
		virtual void serialize(narchive& ar);

	protected:
		typedef std::vector< std::pair<enum ERenderState, int> > st_render_state_array;

		nstring					m_name;		
		nmtl_tech_shader::ptr	m_shader_res;
		st_render_state_array	m_render_states;

		//-- ����״̬�洢�������ϲ�������״̬��־
		bool	m_two_side;
		bool	m_alpha_masked;

		nDECLARE_NAMED_CLASS(nmtl_tech)
	};
}//namespace nexus
#endif //_NEXUS_MATERIAL_TECH_H_