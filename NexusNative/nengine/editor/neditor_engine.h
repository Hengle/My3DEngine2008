/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_EDITOR_ENGINE_H_
#define _NEXUS_EDITOR_ENGINE_H_
#include "../framework/nengine.h"
#include "../renderer/nhit_proxy_hash.h"

namespace nexus
{
	/**
	*	�༭command����
	*	@remarsk �������������ӵ�mgr�У�����ʵ��undo
	*/
	class nEDAPI neditor_cmd : public nobject
	{
	public:
		typedef shared_ptr<neditor_cmd> ptr;

		neditor_cmd(void)	{}
		virtual ~neditor_cmd(void)	{}

		virtual bool execute(void) = 0;
		virtual bool undo(void) = 0;
		virtual bool redo(void) = 0;
		virtual nstring get_name(void) = 0;
		virtual nstring get_desc(void) = 0;

		nDECLARE_VIRTUAL_CLASS(neditor_cmd)
	};

	/**
	 *	�༭����ʹ�õ�engine����
	 *	@remarks �ṩ����༭����render/update�ӿ�(Python friendly)
	*/
	class nEDAPI neditor_engine : public nengine
	{
	public:
		neditor_engine(void);
		virtual ~neditor_engine(void);

		static neditor_engine* instance();

		/** ��Ⱦ/����һ��view port���� */
		virtual void render_level(const nstring& lv_name, const nviewport& view, bool present);		
		/** ����ָ����level�����ڵĶ��� */
		virtual void update_level(const nstring& lv_name, float delta_time, const nviewport& view);

		nactor::ptr pick_main_level_actor(const nviewport& view, const nstring& lv_name, int mouse_x, int mouse_y);		

		//-- command manager��ؽӿ�
		virtual void push_command(neditor_cmd::ptr new_cmd) = 0;
		
	private:
		void render_level_interal(const nstring& lv_name, const nviewport& view, nhit_proxy_hash* hph, bool present);	

		nDECLARE_VIRTUAL_CLASS(neditor_engine);
	};
}//namespace nexus
#endif //_NEXUS_EDITOR_ENGINE_H_