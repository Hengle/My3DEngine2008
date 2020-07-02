/**
*	Nexus Engine
*
*	Author: zewoo
*	Date:	2010-03-27
*/

#ifndef _NEXUS_ENGINE_NAV_MAP_ACTOR_H_
#define _NEXUS_ENGINE_NAV_MAP_ACTOR_H_
#include "../framework/nactor.h"
#include "../resource/nresource.h"
#include "../renderer/nrender_resource.h"
#include "nav_map_component.h"
#include "../../ncore/game_map/navigation_map.h"

namespace nexus
{
	class navigation_map;

	class nAPI nav_map_actor :
		public nactor
	{
	public:
		typedef shared_ptr<nav_map_actor> ptr;

		nav_map_actor(const nstring& name_str);
		virtual ~nav_map_actor(void);

		// ����nav_map_actor����ɹ������
		void create(navigation_map::ptr nav_map);

		// ����߶�ͼ����
		void import_heightmap(const nstring& img_file_name);

		// ��ȡ Height Map
		nheight_map16* get_height_map() const { return m_nav_map->get_height_map(); }
		nbit_map* get_walkable_map() const { return m_nav_map->get_walkable_map(); }
		navigation_map::ptr get_navigation_map() const { return m_nav_map; }

		// ��֧��rotation
		virtual void move(const vector3& pos, const vector3& rot, const vector3& scale);
		void move(const vector3& pos, const vector3& scale);

		// ���߼��
		virtual bool line_check(ncheck_result& ret, const vector3& start, const vector3& end, ELineCheckType check_type);
		/**	����һ�������ڵ������ϻ�������
		@return 1 == up, -1 == low */
		int classify_side(const vector3& wpt);

		bool empty() { return (!m_nav_map && !m_nav_map.get()); }

		// ����ͼ�����仯
		void post_heightmap_change(const nrect& region);

		// ���òü�����
		float get_cull_dist() const { return m_cull_dist; }
		void set_cull_dist(float value)  {  m_cull_dist = value; }

		// ���õ���ͼ������Ⱦ��ɫ
		const color4f& get_height_nav_color() const { return m_height_nav_color; }
		void set_height_nav_color(const color4f& value) { m_height_nav_color = value; }

		const color4f& get_walkable_nav_color() const { return m_walkable_nav_color; }
		void set_walkable_nav_color(const color4f& value) { m_walkable_nav_color = value; }

		bool get_walkable_map_visibale() const { return m_walkable_map_visibale; }
		void set_walkable_map_visibale(bool value) { m_walkable_map_visibale = value; }

	private:
		void recreate_chunks();

	private:
		navigation_map::ptr m_nav_map;

		// ����ͼ������Ⱦ��ɫ
		// �߶�ͼ��ɫ
		color4f		m_height_nav_color;
		// ������������ɫ
		bool		m_walkable_map_visibale;
		color4f		m_walkable_nav_color;

		// ����������Ⱦ���ο�Ĵ�С
		size_t				m_chunk_size;
		// �ü�����
		float				m_cull_dist;

		//-- ��Ⱦ���ݣ����ﱣ��һ�������������ʹ��
		typedef std::vector<nav_map_component::ptr> st_chunk_array;
		st_chunk_array		m_chunks;

		nDECLARE_NAMED_CLASS(nav_map_actor)
	};

} // end of namespace nexus


#endif // end of _NEXUS_ENGINE_NAV_MAP_ACTOR_H_