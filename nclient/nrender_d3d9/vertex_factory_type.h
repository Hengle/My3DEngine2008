/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_VERTEX_FACTORY_TYPE_H_
#define _NEXUS_VERTEX_FACTORY_TYPE_H_
#include "ncore.h"
#include "d3d_ptr.h"
#include "d3d_view_info.h"

namespace nexus
{
	class d3d9_shading_effect;

	/**
	 *	һ��vertex factory type
	 *	@remark 
	 *	.����һ��vertex factory����, ����Ӧһ��������vertex decl�Լ�һ��������shader�ӿ��ļ�
	 *	.ÿ��vertex_factory_type��������,��Ӧһ����̬ȫ�ֱ���
	*/
	class vertex_factory_type
	{
	public:
		typedef shared_ptr<vertex_factory_type> ptr;

		vertex_factory_type(const nstring& mesh_vertex_data_name);
		~vertex_factory_type(void);

		void create_for_static(const nmesh_vertex_data* vb);
		void create_for_morph(const nmesh_vertex_data* vb);
		void destroy();

		void draw_set_vert_decl();
		
		bool is_created() const	{	return m_vert_decl!=NULL;}
		void* get_shader_source()	{	return (void*)m_shader_code.c_str();}
		size_t get_shader_size()	{	return m_shader_code.size();}
		const nstring& get_name()	{	return m_mesh_vertex_data_name;}

		const shader_macro_array& get_shader_macro_array() const	{	return m_shader_macro_array;}
	
	protected:
		void create_shared(const nmesh_vertex_data* vb, vector<D3DVERTEXELEMENT9>& d3d_element_array);
	protected:
		nstring	m_mesh_vertex_data_name;
		std::string								m_shader_code;
		shader_macro_array						m_shader_macro_array;	// �������ݣ���nmesh_vertex_data���Ѿ���һ����
		d3d_ptr<IDirect3DVertexDeclaration9>	m_vert_decl;
	};

	/**
	 *	��������ʹ�ù���vertex_factory_type����
	 *	@remark ֱ����������ʱ���ͷ�
	*/
	class vertex_factory_type_lib 
		: public singleton<vertex_factory_type_lib>
	{
		enum ECreateType
		{
			ECreate_Static,
			ECreate_Morph
		};
	public:
		vertex_factory_type_lib(void);
		~vertex_factory_type_lib(void);

		void conditional_create_for_static(const nmesh_vertex_data* vb);
		void conditional_create_for_morph(const nmesh_vertex_data* vb);

		vertex_factory_type* get_type(nstring type_name);		

		void destroy()	
		{
			m_type_map.clear();
		}

		void draw_set_vert_decl_static_pos_only();
		void draw_set_vert_decl_morph_pos_only();
	
	private:
		void conditional_create(const nmesh_vertex_data* vb, enum ECreateType ct);

	private:
		typedef std::map<nstring, vertex_factory_type::ptr>	vf_type_map;
		vf_type_map	m_type_map;
		d3d_ptr<IDirect3DVertexDeclaration9>	m_vert_decl_static_pos_only;
		d3d_ptr<IDirect3DVertexDeclaration9>	m_vert_decl_morph_pos_only;
	};
}//namespace nexus

#endif //_NEXUS_VERTEX_FACTORY_TYPE_H_