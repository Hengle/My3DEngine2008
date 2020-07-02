#pragma once

#include "ngameframework.h"
#include "ncommon.h"
#include "nobject_fields.h"

class game_canvas;

using namespace gameframework;

/**
 *	��Ϸ��ͼ�пɽ�������Ļ���
*/
class game_unit
{
public:
	typedef boost::shared_ptr<game_unit> ptr;

	game_unit(void);
	virtual ~game_unit(void);

	//!	ȡ����������ϵ����
	const vector3& get_pos() const { return m_controller_ptr->get_position(); }
	void set_pos(const vector3& pos) { m_controller_ptr->set_position(pos); }

	const vector3& get_rotation() const { return m_controller_ptr->get_rotation(); }
	void set_rotation(const vector3& rot) { m_controller_ptr->set_rotation(rot); }

	//!	ȡ�ø�������ϵ����
	npoint get_tile_pos() const;

	uint16 move(const vector3& pos, const vector3& rotation, const vector3& acceleration, float time_stamp, uint8 flag);

	//!	��������״̬
	virtual void update(float delta_time);

	//!	�ڻ����ϻ����Լ�
	virtual void draw(game_canvas* cvs);

	uint64 get_id() const { return m_unit_id; }
	void set_id(uint64 id) { m_unit_id = id; }

	nobj_data& get_data() { return m_data; }

	bool has_flag(uint32 flag) const	{ return m_data.has_flag(EUF_UnitFlags, flag); }

protected:
	COLORREF	m_draw_color;	// �������Ͳ�ͬ���������ò�ͬ����ʾ��ɫֵ

	uint64						m_unit_id;
	ncharacter_controller::ptr	m_controller_ptr;

	nobj_data					m_data;
};
