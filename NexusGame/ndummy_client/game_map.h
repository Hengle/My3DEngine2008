#pragma once

#include "ncommon.h"
#include "game_unit.h"
#include "game_map/navigation_map.h"

class game_canvas;
class console_cmd;

/**
 *	��Ϸ��ǰ�ĵ�ͼ
 *	@remarks ������ͼ+AOI��Χ�ڵ�����NPC�����Զ����ҵ���Ϸ����
*/
class game_map
{
public:
	game_map(void);
	~game_map(void);

	static game_map* instance();
	
	//!	��ʼ��
	void init();
	
	//!	
	void load_map(uint32 map_id);

	//!	���������ڲ��������Ϸ�߼�
	void update(float delta_time);

	//!	�򻭲��ϻ������ж���
	void draw(game_canvas* cvs);

	bool add_remote_unit(game_unit::ptr remote_unit_ptr);
	void remove_remote_unit(uint64 unit_id);
	uint32 get_remote_unit_num();

	game_unit::ptr get_game_unit(uint64 id);

	//!	ȡ���ڲ�����ͼ����ָ��
	navigation_map::ptr get_nav_map() const	{ return m_nav_map_ptr; }
	gameframework::ngame_level* get_nav_level() const	{ return m_game_level_ptr; }

	//!	��ͼ�ж��ٸ��ӿ�
	int get_map_width() const	{	return m_nav_map_ptr->get_raw_width(); }

	//!	��ͼ�ж��ٸ��Ӹ�
	int get_map_height() const	{	return m_nav_map_ptr->get_raw_height(); }
protected:
	//!	����̨���������ӡ��ͼ��Ϣ
	void cmd_map_info(const console_cmd& cmd);
protected:
	typedef std::map<uint64, game_unit::ptr>	UNIT_MAP;
	
	UNIT_MAP									m_aoi_units;			// AOI��Χ�ڵ���Ϸ����
	
	navigation_map::ptr							m_nav_map_ptr;
	gameframework::ngame_level*					m_game_level_ptr;
	gameframework::nlevel_info_resource::ptr	m_level_res_ptr;
};
