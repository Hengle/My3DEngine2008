/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NEXUS_ENGINE_H_
#define _NEXUS_ENGINE_H_
#include "ncore.h"
#include "nlevel.h"
#include "../renderer/nrenderer_base.h"
#include "../renderer/nrender_resource_manager.h"
#include "view_info.h"

namespace nexus
{
	class nrender_resource_manager;

	struct engine_config : public render_config
	{
		nstring	renderer_class;		// ��Ⱦϵͳclass name		
		nstring	file_sys_root;		// �ļ�ϵͳ�ĸ�Ŀ¼
		nstring file_system_class;	// �ļ�ϵͳclass name	
		nstring engine_data_pkg;	// ���������ļ����ڵ�package name
	};

	/**
	 *	Nexus Engine Facade��
	 *	@remark �������ĸ������, �ṩ���ʽӿ�
	*/
	class nAPI nengine :
		public nobject
	{
	public:
		nengine(void);
		virtual ~nengine(void);

		static nengine* instance();

		virtual void init_core();
		virtual void init_modules(const engine_config& cfg);
		virtual void close();		

		nlevel::ptr create_level(const nstring& lv_name, const nstring& class_name);
		void free_level(nlevel::ptr lv_ptr)
		{
			m_level_list.remove(lv_ptr);
		}

		nrenderer_base* get_renderer();		
		nrender_resource_manager* get_render_res_mgr();
		nfile_system* get_file_sys();
		const nstring& get_engine_package_name() const	{	return m_config.engine_data_pkg;}
		

		void set_render_mode(enum ERenderMode rm);
		enum ERenderMode get_render_mode() const;

		void render_attach_element(nrender_element::ptr element_ptr);	
		void render_detach_element(nrender_element::ptr element_ptr);
	
	protected:
		virtual void update(float delta_time);
		virtual void render(const view_info& view);

		size_t load_plugins();

	protected:
		static nengine* s_engine_ptr;		

		typedef std::list<nlevel::ptr> level_list;

		engine_config	m_config;
		
		level_list						m_level_list;
		nrenderer_base::ptr				m_renderer;
		nrender_resource_manager::ptr	m_render_res_mgr;
		vector<nshared_lib::ptr>		m_plugin_array;
		nfile_system::ptr				m_file_sys;

		view_info	m_cur_view;

		friend class nengine_unit_test;
		nDECLARE_CLASS(nengine);
	};
}//namespace nexus
#endif _NEXUS_ENGINE_H_