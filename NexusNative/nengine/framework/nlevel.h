/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NEXUS_LEVEL_H_
#define _NEXUS_LEVEL_H_
#include "ncore.h"
#include "nevent.h"
#include "../scene/nscene_manager.h"
#include "nactor.h"
#include "view_info.h"
#include "nscene_capture.h"

namespace nexus
{
	class nhit_proxy_hash;

	//	����levelʱ���callback�ӿڣ�֪ͨ�ϲ���ص��¼�
	//	������������������������actor��һЩ���ƴ���ȵ�
	struct nlevel_serialize_callback
	{
		virtual void begin_actors(size_t total_actors) = 0;
		virtual void actor_loaded(size_t index, nactor::ptr obj) = 0;
		virtual void actor_saved(size_t index, nactor::ptr obj) = 0;
		virtual void end_actors() = 0;
	};

	/**
	 *	�ؿ��Ļ���
	 *	@remark ���actors��level geom����ǰ����Ҫ����ɽ����ģ�������Ҫ������̬װ�Σ�û����Ϸ�߼����������ж�����
	 *	���ڴ���/���̣�����������������һ��xml�ļ��У�����еĶ�����Ҫ��ȡ���Ķ��������ݣ����Ե�������һ��bin�ļ��С�
	*/
	class nAPI nlevel :
		public nevent_target
	{
	public:
		typedef shared_ptr<nlevel> ptr;

		nlevel(const nstring& name_str);
		virtual ~nlevel(void);

		virtual void init(const nstring& scene_class_name);
		virtual void _destroy();

		virtual nactor::ptr create_actor(const nstring& actor_name, const nstring& class_name=_T("nactor"));
		virtual void destroy_actor(const nname& actor_name);
		nactor::ptr find_actor(unsigned int name_crc);
		nactor::ptr find_actor(const nstring& name_str);

		void rename_actor(const nstring& old_name, const nstring& new_name);

		bool line_check(ncheck_result& ret, const vector3& start, const vector3& end);

		template<typename NClass>
		shared_ptr<NClass> create_actor_t(const nstring& obj_name)
		{
			return dynamic_pointer_cast<NClass>( 
				create_actor(obj_name, 
					NClass::reflection_get_static_class()->get_name())
				);
		}

		virtual void update(float delta_time, const nviewport& view);
		virtual void render(const nviewport& view, nhit_proxy_hash* hph);
		virtual void on_event(nevent_base& evt);
		void render_scene_captures(const nviewport& view);

		void destroy_all_actors();

		nscene_manager::ptr get_scene_manager() {return m_scene_ptr;}
		const nstring& get_name() const	{	return m_name; }

		void add_scene_capture(nscene_capture::ptr cap);
		void remove_scene_capture(nscene_capture::ptr cap);
		
		static void set_serialize_callback(nlevel_serialize_callback* callback)
		{
			s_serialize_callback = callback;
		}

		virtual void serialize(narchive& ar);

		nactor::ptr first_actor()
		{
			m_actor_iter = m_all_actor.begin();
			return m_actor_iter->second;
		}
		nactor::ptr next_actor()
		{
			nactor::ptr ret;
			
			++m_actor_iter;
			if( m_actor_iter != m_all_actor.end() )
				ret = m_actor_iter->second;
			return ret;
		}

	protected:
		// �����л��������load����ʱ���ã�������������ĳ�ʼ������
		// ��Ϊ���л���������ȫ�ָ�����״̬����Щ����ʱ�����Ķ�����Ҫ���������⴦��
		virtual void serialize_loaded();
	protected:
		typedef std::map<unsigned int, nactor::ptr>		nactor_map;
		typedef std::list<nscene_capture::ptr>		nscene_capture_list;
		
		/**
		 * scene captures�����б�
		 * @remarks ���ڼ���һ��level�е�capture����������ܶ࣬����ʹ��һ��list + per object frustum cull
		*/
		nscene_capture_list		m_all_captures;
		nactor_map				m_all_actor;	// todo:Ŀǰ��scene capture����ʽ����Ҫȷ��actor������
		nactor_map::iterator	m_actor_iter;

		nstring				m_scene_class_name;
		nscene_manager::ptr	m_scene_ptr;
		
		static nlevel_serialize_callback* s_serialize_callback;

	private:
		nstring	m_name;

		nDECLARE_NAMED_CLASS(nlevel)
	};
}//namespace nexus
#endif //_NEXUS_LEVEL_H_