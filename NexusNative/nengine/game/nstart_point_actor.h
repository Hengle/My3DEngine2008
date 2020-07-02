#ifndef _NEXUS_NSTART_POINT_ACTOR_H_
#define _NEXUS_NSTART_POINT_ACTOR_H_
#include "../framework/nactor.h"
#include "ngameframework.h"

namespace nexus
{
	using namespace gameframework;

	class gameframework::nlevel_info_resource;

	/**
	*	������
	*/
	class nAPI nstart_point_actor
		: public nactor
	{
	public:
		typedef shared_ptr<nstart_point_actor> ptr;

		nstart_point_actor(const nstring& name_str);
		virtual ~nstart_point_actor(void);

		// ���ظú�����ͬ��start_point_dataλ�úͳ���
		virtual void update_cmp_transform();
		virtual void _set_name(const nstring& new_name)
		{
			nactor::_set_name(new_name);
			m_data.name.reset(new_name);
		}

		// �������л��ӿ�
		virtual void serialize(narchive& ar);

		// �ⲿ��ȡ����
		const start_point_data& get_data() const { return m_data; }
		start_point_data& get_data() { return m_data; }

		//-- ������Դ�ӿ�
		virtual bool export_info_resource(gameframework::nlevel_info_resource* res);

	private:
		start_point_data  m_data;

		nDECLARE_NAMED_CLASS(nstart_point_actor)
	};
}//namespace nexus

#endif //_NEXUS_NSTART_POINT_ACTOR_H_