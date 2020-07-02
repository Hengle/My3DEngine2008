/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_EDITOR_ENGINE_H_
#define _NEXUS_EDITOR_ENGINE_H_
#include "../framework/nengine.h"

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
		virtual void render_level(const nstring& lv_name, const ncamera* cam, int wnd_handle);
		/** ����һ�����е�level�����ڵĶ��� */
		virtual void update_objects(float delta_time);

		nDECLARE_CLASS(neditor_engine);
	};
}//namespace nexus
#endif //_NEXUS_EDITOR_ENGINE_H_