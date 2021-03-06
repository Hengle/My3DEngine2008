#include "stdafx.h"
#include "nav_map_editor.h"
#include "nbrush_noise.h"
#include "terrain_brush.h"

namespace nexus
{
	// 编辑导航图高度命令
	template<typename T>
	class nav_map_brush_cmd : public neditor_cmd
	{
	public:
		nav_map_brush_cmd()
		{
		}
		virtual ~nav_map_brush_cmd() {}

		bool init(nav_map_actor::ptr nav_actor, nheight_map<T>* target, const nterrain_brush& brush, npoint cursor_pos)
		{
			if( !nav_actor )
				return false;
			if( !target )
				return false;

			// cache data 
			m_nav_actor = nav_actor;
			m_target = target;
			m_brush = brush;
			m_cursor_pos = cursor_pos;

			return true;
		}

		virtual bool execute(void)
		{
			nav_map_actor::ptr nav_actor(m_nav_actor);
			if(!nav_actor || nav_actor->empty())
				return false;

			//-- backup old heightmap			
			m_backup_region.left		= m_cursor_pos.x-m_brush.outer_radius;
			m_backup_region.right	= m_cursor_pos.x+m_brush.outer_radius;
			m_backup_region.top		= m_cursor_pos.y-m_brush.outer_radius;
			m_backup_region.bottom	= m_cursor_pos.y+m_brush.outer_radius;
			clip_rect(m_backup_region, m_target->get_width(), m_target->get_height());

			m_backup_map = m_target->create_sub_map(m_backup_region.left, m_backup_region.top,
				m_backup_region.get_width(), m_backup_region.get_height());


			//-- change heightmap
			switch(m_brush.op)
			{
			case EOP_Paint:
				{
					trn_brush_add<T> brush_op;
					m_dirty_region = do_terrain_brush(m_target,
						m_cursor_pos,
						m_brush,
						brush_op);
				}
				break;
			case EOP_Flatten:
				{
					trn_brush_average<T> avg;

					do_terrain_brush(m_target,
						m_cursor_pos,
						m_brush,
						avg);
					if( avg.get_count() > 0 )
					{
						trn_brush_apply_val<T> apply(avg.get_result());

						m_dirty_region = do_terrain_brush(m_target,
							m_cursor_pos,
							m_brush,
							apply);						
					}
				}
				break;
			case EOP_Smooth:
				{
					trn_brush_smooth<T> brush_op(3);
					m_dirty_region = do_terrain_brush(m_target,
						m_cursor_pos,
						m_brush,
						brush_op);
					brush_op.apply(m_target);
				}
				break;
			case EOP_Apply:
				{
					trn_brush_apply_val<T> brush_op(m_brush.strength);
					m_dirty_region = do_terrain_brush(m_target,
						m_cursor_pos,
						m_brush,
						brush_op);					
				}
				break;
			case EOP_Noise:
				{
					nrect brush_rect;
					brush_rect.left = m_cursor_pos.x-m_brush.outer_radius;
					brush_rect.top = m_cursor_pos.y-m_brush.outer_radius;
					brush_rect.right = m_cursor_pos.x+m_brush.outer_radius;
					brush_rect.bottom = m_cursor_pos.y+m_brush.outer_radius;					
					clip_rect(brush_rect, m_target->get_width(), m_target->get_height());

					trn_brush_noise<T> brush_op(brush_rect.get_width(), 
						brush_rect.get_height(),
						npoint(brush_rect.left, brush_rect.top)
						);
					m_dirty_region = do_terrain_brush(m_target,
						m_cursor_pos,
						m_brush,
						brush_op);					
				}
				break;
			}// end of switch

			return true;
		}

		virtual bool undo(void)
		{
			nav_map_actor::ptr nav_actor(m_nav_actor);
			if(!nav_actor || nav_actor->empty())
				return false;

			// write data
			m_target->blit(m_backup_map.get(), m_backup_region.left, m_backup_region.top);			

			return true;
		}

		virtual bool redo(void)
		{
			nav_map_actor::ptr nav_actor(m_nav_actor);
			if(!nav_actor || nav_actor->empty())
				return false;

			return execute();
		}

	protected:
		boost::weak_ptr<nav_map_actor>	m_nav_actor;
		nheight_map<T>*	m_target;
		nterrain_brush	m_brush;
		npoint			m_cursor_pos;

		shared_ptr<nheight_map16 >	m_backup_map;
		nrect						m_backup_region;
		nrect						m_dirty_region;
	};

	/** 编辑导航图的高度 */
	class nav_map_brush_cmd_hmap : public nav_map_brush_cmd<unsigned short>
	{
	public:
		nav_map_brush_cmd_hmap()	{}
		virtual ~nav_map_brush_cmd_hmap()	{}

		virtual bool execute(void)
		{
			nav_map_actor::ptr nav_actor( m_nav_actor );
			if( !nav_actor )
				return false;

			if( nav_map_brush_cmd::execute() )
			{
				nav_actor->post_heightmap_change(m_dirty_region);
				return true;
			}

			return false;
		}

		virtual bool undo(void)
		{
			nav_map_actor::ptr nav_actor( m_nav_actor );
			if( !nav_actor 
				|| !nav_actor->is_valid())
				return false;

			if(nav_map_brush_cmd::undo())
			{
				nav_actor->post_heightmap_change(m_backup_region);
				return true;
			}

			return true;
		}

		virtual nstring get_name(void)
		{
			return _T("terrain heightmap brush");
		}

		virtual nstring get_desc(void)
		{
			wostringstream oss;
			oss << get_name()
				<< _T("(")
				<< m_dirty_region.left << _T(",")
				<< m_dirty_region.top << _T(",")
				<< m_dirty_region.right << _T(",")
				<< m_dirty_region.bottom << _T(",")
				<< _T(")");
			return oss.str();
		}
	};


	/** 导航图位图编辑 */
	class nav_map_brush_cmd_bitmap : public neditor_cmd
	{
	public:
		nav_map_brush_cmd_bitmap() {}
		virtual ~nav_map_brush_cmd_bitmap() {}

		bool init(nav_map_actor::ptr nav_actor, nbit_map* target, const nterrain_brush& brush, npoint cursor_pos)
		{
			if( !nav_actor )
				return false;
			if( !target )
				return false;

			// cache data 
			m_nav_actor = nav_actor;
			m_target = target;
			m_brush = brush;
			m_cursor_pos = cursor_pos;

			return true;
		}

		virtual bool execute(void)
		{
			nav_map_actor::ptr nav_actor(m_nav_actor);
			if(!nav_actor || nav_actor->empty())
				return false;

			//-- backup old heightmap			
			m_backup_region.left		= m_cursor_pos.x-m_brush.outer_radius;
			m_backup_region.right	= m_cursor_pos.x+m_brush.outer_radius;
			m_backup_region.top		= m_cursor_pos.y-m_brush.outer_radius;
			m_backup_region.bottom	= m_cursor_pos.y+m_brush.outer_radius;
			clip_rect(m_backup_region, (long)m_target->get_width(), (long)m_target->get_height());

			m_backup_map = m_target->create_sub_map(m_backup_region.left, m_backup_region.top,
				m_backup_region.get_width(), m_backup_region.get_height());

			//-- change heightmap
			switch(m_brush.op)
			{
			case EOP_Paint:
			case EOP_Flatten:
			case EOP_Smooth:
			case EOP_Apply:
			case EOP_Noise:
				{
					bitmap_brush_apply_val brush_op;
					m_dirty_region = do_brush(m_target,m_cursor_pos,m_brush,brush_op);
				}
				break;
			}// end of switch

			return true;
		}

		virtual bool undo(void)
		{
			nav_map_actor::ptr nav_actor(m_nav_actor);
			if(!nav_actor || nav_actor->empty())
				return false;

			// write data
			m_target->blit(m_backup_map.get(), m_backup_region.left, m_backup_region.top);			

			return true;
		}

		virtual bool redo(void)
		{
			nav_map_actor::ptr nav_actor(m_nav_actor);
			if(!nav_actor || nav_actor->empty())
				return false;

			return execute();
		}

		virtual nstring get_desc(void)
		{
			wostringstream oss;
			oss << get_name()
				<< _T("(")
				<< m_dirty_region.left << _T(",")
				<< m_dirty_region.top << _T(",")
				<< m_dirty_region.right << _T(",")
				<< m_dirty_region.bottom << _T(",")
				<< _T(")");
			return oss.str();
		}
	protected:
		boost::weak_ptr<nav_map_actor>	m_nav_actor;
		nbit_map*	m_target;
		nterrain_brush	m_brush;
		npoint			m_cursor_pos;

		shared_ptr<nbit_map >		m_backup_map;
		nrect						m_backup_region;
		nrect						m_dirty_region;
	};


	class walkable_map_brush_cmd : public nav_map_brush_cmd_bitmap
	{
	public:
		walkable_map_brush_cmd()	{}
		virtual ~walkable_map_brush_cmd()	{}

		virtual bool execute(void)
		{
			nav_map_actor::ptr nav_actor( m_nav_actor );
			if( !nav_actor )
				return false;

			if( nav_map_brush_cmd_bitmap::execute() )
			{
				nav_actor->post_heightmap_change(m_dirty_region);
				return true;
			}

			return false;
		}

		virtual bool undo(void)
		{
			nav_map_actor::ptr nav_actor( m_nav_actor );
			if( !nav_actor 
				|| !nav_actor->is_valid())
				return false;

			if(nav_map_brush_cmd_bitmap::undo())
			{
				nav_actor->post_heightmap_change(m_backup_region);
				return true;
			}

			return true;
		}

		virtual nstring get_name(void)
		{
			return _T("walkable map brush");
		}
	};


	nav_map_editor::nav_map_editor(void):m_cursor_pos(0,0)
	{
	}

	nav_map_editor::~nav_map_editor(void)
	{
	}

	void nav_map_editor::draw_widgets(nrender_primitive_draw_interface* PDI)
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return;

		navigation_map::ptr nav_map = nav_actor->get_navigation_map();
		if( nav_map.get() )
		{
			//-- 显示笔刷区域
			draw_brush_cursor(PDI);

			//-- 显示光标信息
			vector3 wp = nav_map->tile2world(m_cursor_pos.x, m_cursor_pos.y);
			std::wostringstream oss;
			oss << _T("(") << m_cursor_pos.x
				<< _T(",") << m_cursor_pos.y
				<< _T(")-h=") << nav_map->get_raw_height(m_cursor_pos.x, m_cursor_pos.y)
				<< _T(", world(") << wp.x
				<< _T(",") << wp.y
				<< _T(",") << wp.z
				<< _T(")")
				;
			PDI->draw_debug_string(2,2+16, oss.str(), color4ub(255,255,250,255));
		}
	}

	void nav_map_editor::on_mouse_left_down(const npoint& pt)
	{
		UNREFERENCED_PARAMETER(pt);

		m_left_drag.begin_drag();
		nbrush_noise::instance()->reset_brush_noise(m_brush.noise_freq, m_brush.strength);

		// 执行笔刷操作
		brush_paint(false);
	}

	void nav_map_editor::on_mouse_left_up()
	{
		m_left_drag.end_drag();
	}

	void nav_map_editor::on_mouse_right_down()
	{
		m_right_drag.begin_drag();

		nbrush_noise::instance()->reset_brush_noise(m_brush.noise_freq, m_brush.strength);

		brush_paint(true);
	}

	void nav_map_editor::on_mouse_right_up()
	{
		m_right_drag.end_drag();
	}

	void nav_map_editor::on_mouse_wheel(int delta)
	{
		UNREFERENCED_PARAMETER(delta);
	}

	void nav_map_editor::brush_paint(bool neg)
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return;

		nterrain_brush my_brush = m_brush;

		// 计算笔刷强度值取反
		if( neg )
			my_brush.strength = -m_brush.strength;

		boost::shared_ptr<neditor_cmd> new_cmd;
		switch(my_brush.target)
		{
		case EBT_NavigationMap:
			{ 
				// 导航图编辑功能
				boost::shared_ptr<nav_map_brush_cmd_hmap> cmd(new nav_map_brush_cmd_hmap);
				if( cmd->init(nav_actor, nav_actor->get_height_map(), my_brush, m_cursor_pos) )
				{
					if( cmd->execute() )
						new_cmd = boost::dynamic_pointer_cast<neditor_cmd>(cmd);
				}
			}
			break;

		case EBT_WalkableMap:
			{
				// 可行走区域编辑
				boost::shared_ptr<walkable_map_brush_cmd> cmd(new walkable_map_brush_cmd);
				if( cmd->init(nav_actor, nav_actor->get_walkable_map(), my_brush, m_cursor_pos) )
				{
					if( cmd->execute() )
						new_cmd = boost::dynamic_pointer_cast<neditor_cmd>(cmd);
				}
			}
			break;
		}// end of switch

		//-- 把command交给enditor engine管理
		if( new_cmd )
			neditor_engine::instance()->push_command(new_cmd);

	}

	void nav_map_editor::on_mouse_move(const npoint& pt, bool ctrl_down, const ncamera* cam)
	{
		UNREFERENCED_PARAMETER(ctrl_down);
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		navigation_map::ptr nav_map(m_nav_map);
		if( !nav_actor || !nav_map)
			return;

		//-- update brush cursor pos
		ray mouse_ray;
		cam->get_mouse_ray(pt.x, pt.y, mouse_ray);

		const float CHECK_LENGTH = 10240;
		vector3 s = nav_map->get_scale();
		float scale = std::max(s.x, s.z);
		float len = CHECK_LENGTH*scale;
		vector3 end = mouse_ray.origin+mouse_ray.dir*len;

		ncheck_result chk;
		if( nav_actor->line_check(chk, mouse_ray.origin, end, ELCT_Navigation) )
		{
			m_cursor_pos = nav_map->world2tile(chk.location.x, chk.location.z);
		}

		//-- 鼠标拖动操作
		if(m_left_drag.repeat())
		{
			brush_paint(false);
		}
		else if(m_right_drag.repeat())
		{
			brush_paint(true);
		}
	}

	void nav_map_editor::draw_brush_cursor(nrender_primitive_draw_interface* PDI)
	{
		switch(m_brush.shape)
		{
		case EBS_Circle:
			draw_terrain_circle(m_cursor_pos.x, m_cursor_pos.y, m_brush.inner_radius, PDI);
			draw_terrain_circle(m_cursor_pos.x, m_cursor_pos.y, m_brush.outer_radius, PDI);
			break;
		case EBS_Square:
			{
				int x = m_cursor_pos.x;
				int z = m_cursor_pos.y;
				int r = m_brush.inner_radius;
				draw_terrain_quad(x-r, x+r, z-r, z+r, PDI);
				r = m_brush.outer_radius;
				draw_terrain_quad(x-r, x+r, z-r, z+r, PDI);
			}
			break;
		default:
			nASSERT(0 && "unknown terrain brush shape.");
		}
	}

	void nav_map_editor::draw_terrain_quad(int left,int right,int top,int bottom, nrender_primitive_draw_interface* PDI)
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		navigation_map::ptr nav_map(m_nav_map);
		if( !nav_actor || !nav_map)
			return;

		clip_rect(left, top, right, bottom, 
			nav_map->get_raw_width(), nav_map->get_raw_height());

		std::vector<vector3> line_vb;
		int x,y;

		//--left
		x = left;
		for(y=top; y<bottom; y++)
			line_vb.push_back(nav_map->tile2world(x,y));

		//--bottom
		y=bottom-1;
		for(x=left; x<right; x++)
			line_vb.push_back(nav_map->tile2world(x,y));

		//--right
		x=right-1;
		for(y=bottom-1; y>=top; y--)
			line_vb.push_back(nav_map->tile2world(x,y));

		//--top
		y=top;
		for(x=right-1; x>=left; x--)
			line_vb.push_back(nav_map->tile2world(x,y));

		for(size_t i=0; i<line_vb.size(); i++)
			line_vb[i].y += TRN_BRUSH_CURSOR_ZBIAS;
		PDI->draw_line_strip(line_vb, color4f(1,1,0,1));
	}

	void nav_map_editor::draw_terrain_circle(int centre_x, int centre_z, int radius, nrender_primitive_draw_interface* PDI)
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		navigation_map::ptr nav_map(m_nav_map);
		if( !nav_actor || !nav_map)
			return;

		vector3 trn_scale = nav_map->get_scale();
		float rx = radius*trn_scale.x;
		float rz = radius*trn_scale.z;

		vector3 center = nav_map->tile2world(centre_x, centre_z);
		float cx = center.x;
		float cz = center.z;

		std::vector<vector3> line_vb;

		vector3 start_pt(cx+rx, nav_map->get_world_height(cx+rx, cz), cz);
		line_vb.push_back(start_pt);

		vector3 v;
		int num_split =(int)std::min(64, int((2*nPI*rx)/trn_scale.x+4) );
		for(int i=1; i<num_split; i++)
		{
			float a = (nPI*2/num_split)*i;
			v.x = cx + cosf(a)*rx;
			v.z = cz + sinf(a)*rz;
			v.y = nav_map->get_world_height(v.x, v.z)+TRN_BRUSH_CURSOR_ZBIAS;
			line_vb.push_back(v);
		}

		line_vb.push_back(start_pt);

		PDI->draw_line_strip(line_vb, color4f(1,1,0,1));
	}


	void nav_map_editor::import_heightmap(const nstring& img_file_name)
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return;

		nav_actor->import_heightmap(img_file_name);
	}


	bool nav_map_editor::empty() const
	{
		return (m_nav_map.expired() || m_nav_map_actor.expired());
	}

	void nav_map_editor::begin_navigate_map_editor( nlevel::ptr level )
	{
		if(!level || !level.get())
		{
			return;
		}

		const nstring nav_actor_name = _T("__navigate_map_actor");
		// 寻找合适的nav_actor
		nactor::ptr actor = level->find_actor(nav_actor_name);
		nav_map_actor::ptr nav_actor = boost::dynamic_pointer_cast<nav_map_actor>(actor);
		if( !nav_actor && !(nav_actor.get()) )
		{
			// 创建导航图预览nav_actor
			nav_actor = level->create_actor_t<nav_map_actor>(nav_actor_name);
		}
		// 重新创建nav_actor components
		nav_actor->create(level->get_navigation_map());

		m_nav_map_actor = nav_actor;

		navigation_map::ptr nav_map = nav_actor->get_navigation_map();
		m_nav_map = nav_map;

		// 重新创建导航图
		nav_actor->set_visible(true);

		nbrush_noise::instance()->reset_brush_noise(m_brush.noise_freq, m_brush.strength);
	}

	void nav_map_editor::end_navigate_map_editor()
	{
	}

	void nav_map_editor::set_nav_map_visible( bool value )
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return;

		nav_actor->set_visible(value);
	}

	bool nav_map_editor::get_nav_map_visible() const
	{
		if( empty() )
			return false;

		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return false;

		return nav_actor->get_visible();
	}

	bool nav_map_editor::rebuild_navigation_map(float max_height, float step_height, bool use_line_check )
	{
		if( empty() )
			return false;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return false;

		navigation_map::ptr nav_map = nav_actor->get_navigation_map();
		if(!nav_map)
			return false;

		nlevel* level = nav_actor->get_owner();
		if( level == NULL )
			return false;

		int hmap_w = nav_map->get_raw_width(); 
		int hmap_h = nav_map->get_raw_height(); 

		if(use_line_check)
		{
			for(int i=0; i<hmap_w; ++i)
			{
				for(int j=0; j<hmap_h; ++j)
				{
					// 进行摄像检测
					vector3 world_pos = nav_map->tile2world(i, j);

					ncheck_result hit;
					if(level->line_check(hit,vector3(world_pos.x,HALF_WORLD_MAX1,world_pos.z),vector3(world_pos.x,-HALF_WORLD_MAX1,world_pos.z),ELCT_World))
					{
						nav_map->set_height(i, j, hit.location.y);
					}
				}
			}
		}
		else
		{
			rebuild_nav_map_by_actor(nav_map, level);
		}

		// 重新生成可否行走区域
		rebuild_walkable_map(max_height, step_height, nav_map);


		nav_actor->post_heightmap_change(nrect(0,0,hmap_w,hmap_h));
		
		return true;
	}


	void nav_map_editor::move_navigation_map( const vector3& location, const vector3& scale )
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return;

		navigation_map::ptr nav_map = nav_actor->get_navigation_map();
		nav_map->set_location(location);
		nav_map->set_scale(scale);
		
		nav_actor->move(location, scale);
	}

	void nav_map_editor::resize_navigation_map( size_t width, size_t height )
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return;

		navigation_map::ptr nav_map = nav_actor->get_navigation_map();

		// 重新调整导航图大小
		nav_map->resize(width,height);

		nav_actor->create(nav_map);
	}

	void nav_map_editor::reset_navigation_map( size_t width, size_t height )
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return;

		navigation_map::ptr nav_map = nav_actor->get_navigation_map();

		nav_map->create(width, height, nav_map->get_location(), nav_map->get_scale());
		nav_actor->create(nav_map);
	}

	void nav_map_editor::rebuild_nav_map_by_actor( navigation_map::ptr nav_map, nlevel* level )
	{
		int hmap_w = nav_map->get_raw_width(); 
		int hmap_h = nav_map->get_raw_height(); 

		nactor::ptr actor = level->first_actor();
		while(actor != NULL )
		{
			nexus::nterrain_actor::ptr terrain_actor = boost::dynamic_pointer_cast<nterrain_actor>(actor);
			if(terrain_actor)
			{
				for (int i=0; i<hmap_w; ++i)
				{
					for (int j=0; j<hmap_h; ++j)
					{
						// 取地形高度图的高度
						vector3 world_pos = terrain_actor->tile2world(i, j);
						nav_map->set_height(i, j, world_pos.y);
					}
				}
			}

			actor = level->next_actor();
		}
	}

	const color4f default_nav_color = color4f(0.45f, 0.53f, 0.78f, 1.0f);
	const color4f& nav_map_editor::get_height_nav_color() const
	{
		if( empty() )
			return default_nav_color;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return default_nav_color;

		return nav_actor->get_height_nav_color();
	}

	void nav_map_editor::set_height_nav_color( const color4f& value )
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return;

		nav_actor->set_height_nav_color(value);

		navigation_map::ptr nav_map = nav_actor->get_navigation_map();
		if( nav_map )
		{
			nav_actor->post_heightmap_change(nrect(0,0,nav_map->get_raw_width(),nav_map->get_raw_height()));
		}
	}

	const color4f& nav_map_editor::get_walkable_nav_color() const
	{
		if( empty() )
			return default_nav_color;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return default_nav_color;

		return nav_actor->get_walkable_nav_color();
	}

	void nav_map_editor::set_walkable_nav_color( const color4f& value )
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return;

		nav_actor->set_walkable_nav_color(value);

		navigation_map::ptr nav_map = nav_actor->get_navigation_map();
		if( nav_map )
		{
			nav_actor->post_heightmap_change(nrect(0,0,nav_map->get_raw_width(),nav_map->get_raw_height()));
		}
	}

	float nav_map_editor::get_cull_dist() const
	{
		if( empty() )
			return 128.0f * 128.0f;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return 128.0f * 128.0f;

		return nav_actor->get_cull_dist();
	}

	void nav_map_editor::set_cull_dist( float value )
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return;

		nav_actor->set_cull_dist(value);
	}

	void nav_map_editor::rebuild_walkable_map( float &max_height, float &step_height, navigation_map::ptr nav_map )
	{
		int hmap_w = nav_map->get_raw_width(); 
		int hmap_h = nav_map->get_raw_height(); 

		// 计算导航图可行走区域
		max_height = abs(max_height);
		step_height = abs(step_height);
		for(int i=0; i<hmap_w; ++i)
		{
			for(int j=0; j<hmap_h; ++j)
			{
				// 超过高度范围，或者超过步阶
				float current_h = nav_map->get_tile_height(i,j);
				if( abs(current_h) > max_height
					|| abs(current_h - nav_map->get_tile_height(i-1,j-1)) > step_height
					|| abs(current_h - nav_map->get_tile_height(i-1,j+1)) > step_height 
					|| abs(current_h - nav_map->get_tile_height(i+1,j+1)) > step_height 
					|| abs(current_h - nav_map->get_tile_height(i+1,j-1)) > step_height 
					|| abs(current_h - nav_map->get_tile_height(i,j-1)) > step_height 
					|| abs(current_h - nav_map->get_tile_height(i,j+1)) > step_height 
					|| abs(current_h - nav_map->get_tile_height(i-1,j)) > step_height 
					|| abs(current_h - nav_map->get_tile_height(i+1,j)) > step_height 
					)
				{
					nav_map->set_tile_walkable(i, j, 0);
				}
				else
				{
					nav_map->set_tile_walkable(i, j, 1);
				}
			}
		}
	}

	bool nav_map_editor::get_walkable_map_visibale() const
	{
		if( empty() )
			return false;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return false;

		return nav_actor->get_walkable_map_visibale();
	}

	void nav_map_editor::set_walkable_map_visibale( bool value )
	{
		if( empty() )
			return;
		nav_map_actor::ptr nav_actor(m_nav_map_actor);
		if( !nav_actor )
			return;

		nav_actor->set_walkable_map_visibale(value);

		navigation_map::ptr nav_map = nav_actor->get_navigation_map();
		if( nav_map )
		{
			nav_actor->post_heightmap_change(nrect(0,0,nav_map->get_raw_width(),nav_map->get_raw_height()));
		}
	}
} // end of namespace nexus