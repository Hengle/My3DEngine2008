#pragma once
#include "game_canvas.h"

/**
 *	��ϷӦ����--����update����ͼ��
*/
class game_app
{
public:
	game_app(void);
	~game_app(void);

	static game_app* instance();

	//!	��ʼ���Լ���������Ϸ���
	void init();

	//!	ָ����ͼ����
	void bind_render_wnd(CWnd* wnd);

	//!	����������Ϸ�߼�
	void update(float delta_time);

	//!	ʹ����Ϸ�߼��ĵ�ǰ״̬������Ⱦ
	void render();

	//!	�رգ������Դ
	void close();

	//!	���ŵ�ͼ�ӿڵ���ʾ����
	void scale_view(int delta);

	const game_canvas& get_game_canvas() const { return m_canvas; }
 
	float get_game_time() const { return m_game_time; }
	void set_game_time(float game_time) { m_game_time = game_time; }
 
private:
	bool _load_att_data();

private:
	CWnd*		m_render_wnd;	// �󶨵���Ⱦ����
	game_canvas	m_canvas;		// ��Ⱦ�õĻ�������
	float		m_game_time;
};
