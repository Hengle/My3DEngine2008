/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NEXUS_LEVEL_H_
#define _NEXUS_LEVEL_H_
#include "ncore.h"
#include "math/ncheck_result.h"
#include "nevent.h"
#include "../scene/nscene_manager.h"
#include "nactor.h"
#include "view_info.h"
#include "nscene_capture.h"
#include "../physics/nphys_scene.h"
#include "../../ncore/game_map/navigation_map.h"
#include "ngameframework.h"
#include "../renderer/nrenderer_base.h"
#include "../resource/nresource.h"

namespace nexus
{
	class nhit_proxy_hash;
	class nlevel_info_resource;

	struct nlevel_desc
	{
		nstring class_name;
		nstring object_name;
		size_t	num_actors;
	};

	struct nlevel_layer_desc
	{
		bool	is_visible;		//���Ƿ�ɼ�
		bool	is_locked;		//���Ƿ�����
	};

	//	����levelʱ���callback�ӿڣ�֪ͨ�ϲ���ص��¼�
	//	������������������������actor��һЩ���ƴ���ȵ�
	struct nlevel_serialize_callback
	{
		virtual void set_level_desc(const nlevel_desc& ld) = 0;
		virtual void begin_actors() = 0;
		virtual void actor_loaded(nactor::ptr obj) = 0;
		virtual void actor_saved(nactor::ptr obj) = 0;
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
		typedef shared_ptr<nlevel_layer_desc> layer_ptr;

		explicit nlevel(const nstring& name_str);
		virtual ~nlevel(void);

		virtual void init();
		virtual void _destroy();

		// physics interface
		virtual void init_level_phys( const vector3& gravity = vector3(0.f,-9.8f,0.f) );

		nphys_scene* get_phys_scene()
		{
			return m_phys_scene.get();
		}
		
		virtual nactor::ptr create_actor(const nstring& actor_name, const nstring& class_name=_T("nactor"));
		virtual bool destroy_actor(const nname& actor_name);
		nactor::ptr find_actor(unsigned int name_crc);
		nactor::ptr find_actor(const nstring& name_str);

		//����ָ����actor,��actor������ԭʼactor��λ�ò��Һ�ԭʼactor��ȫһ�£�������actor��ָ��
		nactor::ptr clone_actor(nactor::ptr actor);

		bool rename_actor(const nstring& old_name, const nstring& new_name);

		//! ��һ���Ѿ������õ�NActor��ӵ��ؿ���
		void attach_actor(nactor::ptr actor);
		//! ��һ���Ѿ������õ�NActor�ӹؿ����Ƴ������ǲ���ɾ����NActor
		bool deattach_actor(nactor::ptr actor);

		bool line_check(ncheck_result& ret, const vector3& start, const vector3& end, ELineCheckType check_type);

		template<typename NClass>
		shared_ptr<NClass> create_actor_t(const nstring& obj_name)
		{
			return dynamic_pointer_cast<NClass>( 
				create_actor(obj_name, 
					NClass::reflection_get_static_class()->get_name())
				);
		}

		virtual void update(float delta_time, const nviewport& view);
		virtual void render(const nviewport& view);
		virtual void on_event(nevent_base& evt);
		void render_scene_captures(const nviewport& view);

		void destroy_all_actors();

		nscene_manager::ptr get_scene_manager() {return m_scene_ptr;}
		const nstring& get_name() const	{	return m_name; }

		void add_scene_capture(nscene_capture::ptr cap);
		void remove_scene_capture(nscene_capture::ptr cap);

		void do_serialize(const struct resource_location& folder, enum EFileMode fmode, nlevel_serialize_callback* callback);
		virtual void serialize(narchive& ar);

		size_t get_num_actors() const	{	return m_all_actor.size(); }
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

		render_setting*	get_render_setting()
		{
			return &m_render_setting;
		}

		//-- layer API
		unsigned int  get_layer_count();
		nstring get_layer_name(unsigned int index);
		bool is_layer_exist(const nstring& layer_name);
		void new_layer(const nstring& layer_name);
		void remove_layer(const nstring& layer_name);
		//��������
		void rename_layer(const nstring& old_layer_name,const nstring& new_layer_name);
		//���õ�ǰ������Ĳ�
		void set_active_layer(const nstring& layer_name);
		//��ȡ��ǰ������Ĳ������
		nstring get_active_layer();
		//�������߽���ָ�����ֵĲ�
		void lock_layer(const nstring& layer_name,bool lock);
		//ָ�����ֵĲ��Ƿ�����
		bool is_layer_locked(const nstring& layer_name);
		//��ʾ��������ָ���Ĳ�
		void show_layer(const nstring& layer_name,bool show);
		//�Ƿ���ʾָ����
		bool is_layer_visible(const nstring& layer_name);
		//��ָ����actor�ƶ�����ǰ�����
		void move_actor_to_active_layer(const nstring& actor_name);

		// ����ͼ��ؽӿ�
		navigation_map::ptr get_navigation_map() const { return m_nav_map; }
		// ref navigation_map::line_of_sight
		bool line_of_sight(ncheck_result& ret, const vector3& start, const vector3& end, bool walkable_check);

		bool get_world_walkable(float wx, float wz) const { return m_nav_map->get_world_walkable(wx,wz); }
		bool get_tile_walkable(int tx, int tz) const { return m_nav_map->get_tile_walkable(tx,tz); }
		float get_world_height(float wx, float wz) const { return m_nav_map->get_world_height(wx,wz); }
		float get_tile_height(int tx, int tz) const { return m_nav_map->get_tile_height(tx,tz); }

		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);

		/**
		* ����level�����������л���ͬ���ǣ�������Ϊ���ṩ������������Ҫ����Դ
		* param pkg_name: ��������
		* param file_path: �����ļ�·����ʵ�ʴ����ļ���Ϊ file_path + _T("/level_info_resource.xml")
		* return �ܼƵ����˶��ٸ�actor
		*/
		virtual int export_level(const nstring& pkg_name, const nstring& file_path);
		virtual int export_level(const resource_location& res_loc);

	protected:
		//-- ����actor�������е��������򷵻�true
		virtual bool export_actor(gameframework::nlevel_info_resource* res, nactor::ptr actor);

		// �����л��������load����ʱ���ã�������������ĳ�ʼ������
		// ��Ϊ���л���������ȫ�ָ�����״̬����Щ����ʱ�����Ķ�����Ҫ���������⴦��
		virtual void serialize_loaded();
		virtual void serialize_layer_name(narchive& ar, const nstring& layer_name,nlevel_serialize_callback* callback);

		//-- ��ȡ����ͼ��Ϣ
		virtual void serialize_navigation_map(nfile_system* fs, const resource_location& level_folder, enum EFileMode fmode);

		// simulate level physics
		virtual void simulate_level_phys(float delta_time);
		virtual void get_level_phys_result();
		virtual void release_level_phys();
	protected:
		typedef std::map<unsigned int, nactor::ptr>		nactor_map;
		typedef std::set<nstring> st_string_set;
		typedef std::list<nscene_capture::ptr>		nscene_capture_list;
		typedef std::map<nstring,nlevel_layer_desc>	nlayer_map;

		/**
		 * scene captures�����б�
		 * @remarks ���ڼ���һ��level�е�capture����������ܶ࣬����ʹ��һ��list + per object frustum cull
		*/
		nscene_capture_list		m_all_captures;
		nactor_map				m_all_actor;	// todo:Ŀǰ��scene capture����ʽ����Ҫȷ��actor������
		nactor_map::iterator	m_actor_iter;

		st_string_set		m_layer_set;
		nlayer_map			m_layer_desc;
		nstring				m_active_layer;

		nstring				m_scene_class_name;
		nscene_manager::ptr	m_scene_ptr;
		
		// physics data
		nphys_scene::ptr	m_phys_scene;

		render_setting		m_render_setting;
	private:
		nstring	m_name;

		//! ����ͼ��Ϣ
		navigation_map::ptr		m_nav_map;

		nDECLARE_NAMED_CLASS(nlevel)
	};
}//namespace nexus
#endif //_NEXUS_LEVEL_H_