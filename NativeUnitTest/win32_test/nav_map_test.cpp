#include "StdAfx.h"
#include "nav_map_test.h"
#include "../../NexusNative/nengine/navigation_map/nav_map_actor.h"
#include "../../NexusNative/ncore/timer/nprofile.h"

nDEFINE_CLASS(nav_map_test, nrender_test)



nav_map_test::nav_map_test(void)
{
	m_graph = NULL;
	m_debug_draw = NULL;
}

nav_map_test::~nav_map_test(void)
{
}

void nav_map_test::open( perspective_camera_controller* cam )
{
	nrender_test::open(cam);
	m_camera_ctrl = cam;

	//vector3 scale(128, 16, 128);
	//navigation_map::ptr nav_map(new navigation_map());
	//nav_map->create(65, 65, vector3(-32*scale.x,0,-32*scale.z), scale);

	//nav_map_actor::ptr nav = m_level_ptr->create_actor_t<nav_map_actor>(_T("test_nav_actor"));
	//nav->create(nav_map);
	//nav->import_heightmap(_T("../nexus_game/demo_data/terrain/heightMap.bmp"));

	m_nav_map.reset(new navigation_map());
	m_nav_map->serialize(nengine::instance()->get_file_sys(),_T("demo_data"), _T("/level/navigation_map_desc.xml"), EFileRead);

	nASSERT(m_nav_map);
	int w = m_nav_map->get_raw_width();
	int h = m_nav_map->get_raw_height();
	nASSERT(w>0 || h>0);

	// 创建导航图预览actor
	nav_map_actor::ptr nav = m_level_ptr->create_actor_t<nav_map_actor>(_T("test_nav_actor"));
	nav->create(m_nav_map);
	nav->set_cull_dist(2048 * 32.0f * 4.0f);

	m_graph = new ngraph_navigation_map(m_nav_map);

	// 创建角色控制测试相关对象
	m_debug_draw = new debug_draw_imp();

	m_game_level.reset(new ngame_level());
	m_game_level->attach_nav_map(m_nav_map);

	m_character.reset(new ncharacter_controller());

	re_find_pather();
}

void nav_map_test::close()
{
	nrender_test::close();

	if(m_graph)
	{
		delete m_graph; m_graph = NULL;
	}
	m_nav_map.reset();

	if(m_debug_draw)
	{
		delete m_debug_draw; m_debug_draw = NULL;
	}
}

void nav_map_test::debug_draw( nrender_primitive_draw_interface* PDI )
{
	// 显示debug信息
	std::wostringstream ss;
	ss << _T("start point: X=") << m_start.x << _T(", Y=") << m_start.y
		<< _T("  end point: X=") << m_end.x << _T(", Y=") << m_end.y
		<< _T("  paths size=") << m_ret_paths.size()
		<< _T("  take times=") << m_take_times << _T("ms");
	PDI->draw_debug_string(2, 24, ss.str(), color4ub(5,255,255,255));

	// 绘制路径
	const color4f path_color = color4f(1.0f,0.f, 0.f,1.0f); 
	for( int index = 0; index<(int)m_ret_paths.size(); ++index)
	{
		const npoint& path = m_ret_paths[index];
		vector3 world_pos = m_nav_map->tile2world(path.x, path.y);
		world_pos.y += 4.0f;

		PDI->draw_wire_star(world_pos,m_nav_map->get_scale().x, path_color);
	} 

	// 绘制角色控制相关
	m_debug_draw->pre_draw(PDI);
	m_character->draw_debug_info(m_debug_draw);

	// 绘制profile
	profile_value* profile_val = profile_manager::instance()->first_profile();
	while (profile_val!=NULL)
	{
		PDI->draw_debug_string(2, 50, profile_val->to_string(), color4ub(5,255,255,255));

		profile_val = profile_manager::instance()->next_profile();
	}
}

void nav_map_test::tick( float run_time,float delta_time)
{
	nrender_test::tick(run_time, delta_time);

	m_character->move(m_game_level.get(),delta_time);
}

void nav_map_test::on_key_down( unsigned int key )
{
	switch(key)
	{
	case VK_SPACE:
		{
			m_character->jump();
		}
		break;
	case VK_HOME:
		{
			m_camera_ctrl->set_look_at(m_character->get_position());
		}
		break;
	case VK_TAB:
		{
			re_find_pather();
		}
		break;
	case 65:	// 'a'
		{
			m_character->set_acceleration(vector3(-1,0,0));
		}
		break;
	case 68:	// 'd'
		{
			m_character->set_acceleration(vector3(1,0,0));
		}
		break;
	case 69:	// 'e'
		{
			m_character->set_acceleration(vector3(0,0,1));
		}
		break;
	case 81:	// 'q'
		{
			m_character->set_acceleration(vector3(0,0,-1));
		}
		break;
	case 70:	// 'f'
		{
			EMovementType current_movement_type = m_character->get_current_movement_type();
			if(current_movement_type != EMove_Fly)
			{
				m_character->fly();
			}
			else
			{
				m_character->set_current_movement_type(EMove_Ground);
			}
		}
		break;
	}
}

void nav_map_test::re_find_pather()
{
	nASSERT(m_nav_map);
	int w = m_nav_map->get_raw_width();
	int h = m_nav_map->get_raw_height();
	nASSERT(w>0 || h>0);

	m_start = npoint(rand()%w, rand()%h);
	m_end = npoint(rand()%w, rand()%h);
	clock_t clock_start = clock();
	m_graph->find_pather(m_start, m_end, 100.0f, m_ret_paths);
	clock_t clock_end = clock();

	m_take_times = float(clock_end - clock_start);

	const vector3 start_pos = m_nav_map->tile2world(m_start.x, m_start.y);
	const vector3 end_pos = m_nav_map->tile2world(m_end.x, m_end.y);
	m_character->transport(m_game_level.get(), start_pos,true);

	// 重新设置角色加速度
	m_character->set_acceleration(vec_normalize(end_pos-start_pos));
}