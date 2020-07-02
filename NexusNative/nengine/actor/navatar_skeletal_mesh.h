#ifndef _NEXUS_AVATAR_SKELETAL_MESH_H_
#define _NEXUS_AVATAR_SKELETAL_MESH_H_
#include "nmesh_component.h"
#include "../resource/nresource_skeletal_mesh.h"
#include "../renderer/ncpu_skin_mesh.h"
#include "../renderer/nrender_element.h"
#include "../animation/nskel_anim_controller.h"
#include "nskeletal_mesh_component.h"

namespace nexus
{
	/**
	 *	��װϵͳר�õ�skeletal mesh���
	 *	@remarks �롰nskeletal_mesh_component���Ĳ�ͬ�����ڣ�1����navator_actor�����һ��������������2������Դ�滻���ܣ�
	 *	@see class nskeletal_mesh_component
	*/
	class navatar_skeletal_mesh
		: public nskeletal_mesh_component
	{
	public:
		navatar_skeletal_mesh(const nstring& name_str);
		virtual ~navatar_skeletal_mesh(void);

		virtual void update(float delta_time, const nviewport& view);

	protected:


		nDECLARE_NAMED_CLASS(navatar_skeletal_mesh)
	};
}//namespace nexus
#endif