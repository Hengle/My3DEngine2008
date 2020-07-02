#ifndef _NEXUS_AVATAR_ATTACHMENT_H_
#define _NEXUS_AVATAR_ATTACHMENT_H_
#include "nstatic_mesh_component.h"
#include "../mesh/skeletal_anim.h"

namespace nexus
{
	/**
	 *	ר����������װ���صľ�̬mesh���
	 *	@remarks �ڻ���Ļ����������������ܣ�1����ָ���Ĺ����Զ�����world����2�����첽IO��Դ�滻�����������ʧ����
	*/
	class navatar_attachment
		: public nstatic_mesh_component
	{
	public:
		navatar_attachment(const nstring& name_str);
		virtual ~navatar_attachment(void);

		void bind_to_bone(const nstring& bone_name, const nskeleton_define* skel);
		virtual void update(float delta_time, const nviewport& view);

	protected:
		nstring		m_bone_name;
		int			m_bone_index;
		matrix44	m_ref_base;

		nDECLARE_NAMED_CLASS(navatar_attachment)
	};
}//namespace nexus

#endif //_NEXUS_AVATAR_ATTACHMENT_H_