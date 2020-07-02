#pragma once
using namespace nexus;

/*
	��ͷ����
	.��ͷһֱ��������;
	.����Ҽ��϶�, ��ת��ͷ, ��ʹ�����ǳ����ھ�ͷ����һ��;
	.�������϶�, ��ת��ͷ, ��Ӱ���ɫ;
*/
class player_camera_ctrl : public camera_controller
{
public:
	player_camera_ctrl(void);
	~player_camera_ctrl(void);

	virtual void update_camera(ncamera* cam)
	{
		lock_player_pos();
		m_compute.update_camera_view(cam);
	}

	void process_input_message(const MSG& msg);

	void bind_player(nactor::ptr player);
	void close()
	{
		m_player.reset();
	}

protected:
	virtual void on_mouse_left_down(const npoint& pt);
	virtual void on_mouse_left_up();
	virtual void on_mouse_right_down(const npoint& pt);
	virtual void on_mouse_right_up();
	virtual void on_mouse_wheel(int delta);
	virtual void on_mouse_move(const npoint& pt, bool ctrl_down);

	void mouse_left_drag(const npoint& pt, bool ctrl_down);
	void mouse_right_drag(const npoint& pt, bool ctrl_down);

	void lock_player_pos();

private:
	camera_YPR		m_compute;
	nactor::ptr		m_player;
};
