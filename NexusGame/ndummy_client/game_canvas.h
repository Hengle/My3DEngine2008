#pragma once

#include "ncore.h"

/**
 *	��ͼ��Ұ--������ǰ������ʾ��ͼ�ķ�Χ��һЩ����
*/
struct map_view
{
	int		tile_size;	// ����ͼһ�����ӵ����ش�С
	//int		left,		// �������Ͻǣ�0��0�������Ӧ�ĵ�ͼ��������
	//		top;
	int		wnd_w,		// ���ڵĴ�С����λ������
			wnd_h;

	vector3		loaction;

	COLORREF	clear_color;	// ���ɫ
	bool		show_grid;		// �Ƿ���ʾ��ͼ�����߿�
	COLORREF	grid_color;		// �����߿����ɫ
	
	map_view()
	{
		tile_size = 18;
		//left = top = 0;
		wnd_w = wnd_h = 100;

		loaction.x = 0;
		loaction.y = 0;
		loaction.z = 0;

		clear_color = RGB(230,230,230);
		show_grid = true;		
		grid_color = RGB(0, 28, 88);
	}


	npoint world2pixel(const vector3& pos) const
	{
		npoint pt;
		pt.x = int((pos.x - loaction.x) / tile_size + (wnd_w / 2));
		pt.y = int((loaction.z - pos.z) / tile_size + (wnd_h / 2));
		return pt;
	}

	vector3 pixel2world(npoint pt) const
	{
		vector3 pos;
		pos.x = (pt.x - (wnd_w / 2)) * tile_size + loaction.x;
		pos.z = ((wnd_h / 2) - pt.y) * tile_size + loaction.z;
		return pos;
	}
};

class game_unit;

/**
 *	��Ϸ��Ⱦ�õĻ���
 *	@remakrs ��װ����Ӧ�ò��GDI��ͼ�ӿڣ�����һ��Double buffer
*/
class game_canvas
{
public:
	game_canvas(void);
	~game_canvas(void);

	//!	��ʼ���������ڲ�buffer����
	void init(CWnd* wnd);

	//!	���ڲ�buffer���Ƶ�������
	void flip(CWnd* wnd);

	//!	�رգ�����ڲ�����
	void close();

	//!	���ݱ���������������ͼ���꣬ʹ�������ʼ���������
	void update_view(const vector3& pos);

	//!	�������л�ͼ
	void clear();

	//!	���߽ӿ�
	void draw_line(int start_x, int start_y, int end_x, int end_y, COLORREF clr);

	//!	����һ��ʵ�ĵľ���
	void draw_rect(const nrect& rc, COLORREF clr);

	const map_view& get_view() const	{	return m_view;}

	int get_scale() const { return m_view.tile_size; }
	void set_scale(int scale) { m_view.tile_size = scale; }

private:
	map_view	m_view;		//	�ӿ���������

	CBitmap		m_mem_bmp;	//	˫�����õ�bitmap����
	CBitmap*	m_old_bmp;	//	�����ϵ�mem dcʹ�õ�bitmap���󣬹��ָ���
	CDC			m_mem_dc;	//	˫����ʹ�õ�DC����
};
