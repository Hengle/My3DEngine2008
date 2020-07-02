#ifndef _NEXUS_ENGINE_NAV_MAP_EDITOR_H_
#define _NEXUS_ENGINE_NAV_MAP_EDITOR_H_
#include "neditor_engine.h"
#include "../navigation_map/nav_map_actor.h"
#include "mouse_helper.h"
#include "nterrain_editor.h"

namespace nexus
{
	/**
	*	���α༭��
	*	@remarks �ṩ�༭���ܽӿڣ��ṩ����༭�����õİ�װ�ӿ�
	*/
	class nEDAPI nav_map_editor
	{
	public:
		nav_map_editor(void);
		~nav_map_editor(void);

		// ��ʼ����ͼ�༭��ģʽ
		void begin_navigate_map_editor(nlevel::ptr level);
		// ��������ͼ�༭��ģʽ
		void end_navigate_map_editor();

		// ��ʾ�������ص���ͼ
		void set_nav_map_visible(bool value);
		bool get_nav_map_visible() const;

		// �������ɵ���ͼ
		// max_height: ָ������ܴﵽ�ĸ߶�
		// step_height: ָ������
		// use_line_check: trueʹ�����߼��ȷ������ͼ���Ӹ߶ȣ�false������κ͵���ĸ߶�
		bool rebuild_navigation_map(float max_height, float step_height, bool use_line_check);

		// ���õ���ͼ�Ĵ�С������
		void move_navigation_map(const vector3& location, const vector3& scale);
		// ���µ�������ͼ�Ĵ�С�����ұ��ָ߶Ȳ���
		void resize_navigation_map(size_t width, size_t height);
		// �������õ���ͼ�Ĵ�С�����ҽ��߶�����Ϊ0
		void reset_navigation_map(size_t width, size_t height);

		// ��nav_map_actor�Ƿ���Ч
		bool empty() const;

		void close()
		{
			m_nav_map_actor.reset();
		}

		// ����߶�ͼ���ᵼ���������ݵĶ�ʧ
		void import_heightmap(const nstring& img_file_name);

		void set_brush(const nterrain_brush& brush)		{	m_brush = brush; }
		void draw_widgets(nrender_primitive_draw_interface* PDI);

		void on_mouse_left_down(const npoint& pt);
		void on_mouse_left_up();
		void on_mouse_right_down();
		void on_mouse_right_up();
		void on_mouse_wheel(int delta);
		void on_mouse_move(const npoint& pt, bool ctrl_down, const ncamera* cam);

		// ���òü�����
		float get_cull_dist() const;
		void set_cull_dist(float value);

		// ���õ���ͼ������Ⱦ��ɫ
		const color4f& get_height_nav_color() const;
		void set_height_nav_color(const color4f& value);

		const color4f& get_walkable_nav_color() const;
		void set_walkable_nav_color(const color4f& value);

		bool get_walkable_map_visibale() const;
		void set_walkable_map_visibale(bool value);
	protected:
		void draw_brush_cursor(nrender_primitive_draw_interface* PDI);
		void draw_terrain_circle(int centre_x, int centre_z, int radius, nrender_primitive_draw_interface* PDI);
		void draw_terrain_quad(int left,int right,int top,int bottom, nrender_primitive_draw_interface* PDI);

		void brush_paint(bool neg);

		void rebuild_nav_map_by_actor( navigation_map::ptr nav_map, nlevel* level );

		// ���¼�������������
		// max_height: ָ������ܴﵽ�ĸ߶�
		// step_height: ָ������
		void rebuild_walkable_map( float &max_height, float &step_height, navigation_map::ptr nav_map );

	protected:
		nterrain_brush					m_brush;

		boost::weak_ptr<nav_map_actor>	m_nav_map_actor;
		boost::weak_ptr<navigation_map>	m_nav_map;

		//-- mouse input
		mouse_drag	m_left_drag;
		mouse_drag	m_right_drag;

		npoint	m_cursor_pos;
	};


} // end of namespace nexus

#endif // end of _NEXUS_ENGINE_NAV_MAP_EDITOR_H_