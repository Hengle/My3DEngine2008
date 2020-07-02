#pragma once
#include "ntest_case.h"
#include <vector>

class TestActor:public nactor
{
private:
	vector3		m_pos;		//���ֵ�λ��
	vector3		m_dir;		//��ǰ�ƶ�����ÿ3��仯һ��
	float		m_time;		//�����ϴη���仯��ʱ��
	float		m_start_time;	//��ʼʱ��
	vector4		m_screen_pos;	//��������Ļ�ϵ�λ�ú����
public:
	TestActor(const nstring& name_str);
	virtual ~TestActor();
	virtual void update(float delta_time, const nviewport& view);
	virtual void move(const vector3& pos, const vector3& rot, const vector3& scale);
	//�����ƶ����ƶ���λ��
	virtual void move(const vector3& pos);
	virtual	const vector4&	get_screen_position();
	nDECLARE_NAMED_CLASS(TestActor)
};

class shadow_test :
	public nrender_test
{
public:
	shadow_test(void);
	virtual ~shadow_test(void);

	virtual void open(perspective_camera_controller* cam);
	virtual void tick(float run_time);
	virtual void debug_canvas(nui_canvas* canvas);
	virtual void _on_device_lost(int param);
	virtual bool _on_device_reset(int param);
private:
	vector<nactor::ptr>	m_man;
	nrender_font*		m_font;
	nresource_texture_2d::ptr	m_test_icon;
	frect				m_icon_rect;
	nDECLARE_CLASS(shadow_test)
};
