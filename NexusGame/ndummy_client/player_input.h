#pragma once

/**
 *	������Ϸ�еļ����������
*/
class player_input
{
public:
	player_input(void);
	~player_input(void);

	static player_input* instance();

	//!	���������������¼�
	void on_mouse_left_down(const npoint& pt);

	//!	����������̧���¼�
	void on_mouse_left_up();

	//!	��������Ҽ������¼�
	void on_mouse_right_down(const npoint& pt);

	//!	��������Ҽ�̧���¼�
	void on_mouse_right_up();

	//!	���������ֲ���
	void on_mouse_wheel(int delta);

	//!	��������ƶ�����
	void on_mouse_move(const npoint& pt, bool ctrl_down);

	//!	��������²���
	void on_key_down(int key);
};
