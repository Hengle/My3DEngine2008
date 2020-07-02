/**
 *	nexus ngame - nmap
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NMAP_H_
#define _NGAME_NMAP_H_

#include "ncommon.h"
#include "nbroadcast_helper.h"
#include "ntimer.h"
#include "base_define.h"
#include "math/vector.h"
#include "math/rect.h"
#include "game_map/navigation_map.h"
#include "ngameframework.h"

namespace nexus {

	class nworld_object;
	class nunit;
	class nplayer;
	class ncreature;

	class ntile;
	struct nmap_info;
	class navigation_map;
	class ngraph_navigation_map;
	class nregion;

	typedef point<int32> ntile_coord;

	class nmap : private nnoncopyable
	{
	public:
		typedef boost::unordered_map<uint64, nplayer*>		PLAYER_MAP;					// 64id�����չ
		typedef boost::unordered_map<uint32, ncreature*>	CREATURE_MAP;				// nworld_object��Ϊ�������������

	public:
		nmap();
		virtual ~nmap();

		virtual bool init(uint32 instance_id, const nmap_info* map_info_ptr, gameframework::nlevel_info_resource::ptr level_res_ptr);
		virtual void destroy();
		virtual void update(uint32 elapse);

		uint32 get_map_id() const		{ return m_map_id; }
		uint32 get_instance_id() const	{ return m_instance_id; }
		navigation_map::ptr get_nav_map() const					{ return m_nav_map_ptr; }
		gameframework::ngame_level* get_game_level() const	{ return m_game_level_ptr; }
		ngraph_navigation_map* get_ngraph_navigation_map() const{ return m_path_finder; }
		uint32 get_player_num() const	{ return uint32(m_player_map.size()); }

		virtual int16 can_enter(nplayer* player_ptr) { return 0; }
		virtual bool add_player(nplayer* player_ptr);
		virtual void remove_player(nplayer* player_ptr, bool exit = true);								// ���ؽ�ɫ�б����ߣ��л���ͼ

		bool add_creature(ncreature* creature_ptr, bool map_loaded = false);
		void remove_creature(ncreature* creature_ptr);

		// һ���ڵ�ͼ�߳�֮��ʹ��
		nunit* get_unit(uint64 guid) const;
		nplayer* get_player(uint64 guid) const;
		ncreature* get_creature(uint64 guid) const;

		// ��Ϣ�㲥
		// aoi or ��Χ�㲥
		void broadcast(nworld_object* obj_ptr, const void* msg_ptr, uint32 size);
		void broadcast(nworld_object* obj_ptr, const void* msg_ptr, uint32 size, float dist);
		void broadcast(nplayer* player_ptr, const void* msg_ptr, uint32 size, bool to_self);
		void broadcast(nplayer* player_ptr, const void* msg_ptr, uint32 size, float dist, bool to_self);
		// map �㲥
		void map_broadcast(nplayer* player_ptr, const void* msg_ptr, uint32 size, bool to_self);
		// �¼��㲥
		void broadcast_event(nworld_object* obj_ptr, const void* data_ptr, uint32 size, float dist, uint32 mask, bool to_self = false, uint32 delay = 0);
		void map_broadcast_event(nworld_object* obj_ptr, const void* data_ptr, uint32 size, uint32 mask, bool to_self = false, uint32 delay = 0);
		// ȡ�÷�Χ�ڵĶ��󼯺�
		void get_objects(nworld_object* obj_ptr, nregion* region_ptr, std::list<nworld_object*>& objs, uint32 mask, bool include_self = false);

		void object_move(nworld_object* obj_ptr, const vector3& new_pos);

	private:
		// ��������to tile����
		ntile_coord _world2tile(float wx, float wz) const;
		uint32 _get_tile_index(const ntile_coord& coord) const;
		ntile* _get_tile(uint32 tile_index) const;
		ntile* _get_tile(const ntile_coord& coord) const;
		void _update_world_obejct_visibility_to_player(nplayer* player_ptr, bool enter_map = true);
		void _update_world_obejct_visibility(nworld_object* obj_ptr, bool visible);
		void _world2tile_mod(float wx, float wz, int32& tx, int32& tz);
		void _set_world_object_tile(nworld_object* obj_ptr, const vector3& new_pos);

	private:
		// ��������
		PLAYER_MAP				m_player_map;
		CREATURE_MAP			m_creature_map;

		// ��ͼ��Ϣ
		uint32					m_map_id;									// ��ͼ����id
		uint32					m_instance_id;								// �������ͼΪ0
		const nmap_info*		m_map_info_ptr;								// ��ͼ�߼���Ϣ
		navigation_map::ptr		m_nav_map_ptr;								// ����ͼ

		gameframework::ngame_level*					m_game_level_ptr;
		gameframework::nlevel_info_resource::ptr	m_level_res_ptr;
		ngraph_navigation_map*						m_path_finder;

		uint32					m_tile_width;								// ��ͼ���ת��Ϊtile����Ŀ
		uint32					m_tile_height;								// ��ͼ����ת��Ϊtile����Ŀ
		ntile*					m_tile_ptrs;								// tile�ڴ����飬���ݵ�ͼʵ�ʴ�С����

		float					m_visible_distance;							// ��Ұ����aoi�뾶

		nbroadcast_helper		m_broadcaster;								// �㲥������, ���ڲ���ִ��ÿ����ͼʵ��һ������
		ninterval_timer			m_update_timer;								// ���ƶ������ʱ��
		uint32					m_last_elapse;
	};

} // namespace nexus

#endif // _NGAME_NMAP_MGR_H_
