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
	 *	�༭����ʹ�õ�engine����
	 *	@remarks �ṩ����༭����render/update�ӿ�(Python friendly)
	*/
	class nEDAPI neditor_engine : public nengine
	{
	public:
		neditor_engine(void);
		virtual ~neditor_engine(void);

		/** ��Ⱦ/����һ��view port���� */
		virtual void render_level(const nstring& lv_name, const nviewport& view, bool present);		
		/** ����ָ����level�����ڵĶ��� */
		virtual void update_level(const nstring& lv_name, float delta_time);

		nactor::ptr pick_main_level_actor(const nviewport& view, const nstring& lv_name, int mouse_x, int mouse_y);		
	private:
		void render_level_interal(const nstring& lv_name, const nviewport& view, nhit_proxy_hash* hph, bool present);	

		nDECLARE_CLASS(neditor_engine);
	};
}//namespace nexus
#endif //_NEXUS_EDITOR_ENGINE_H_