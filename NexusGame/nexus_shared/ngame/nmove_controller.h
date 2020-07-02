/**
 *	nexus ngame - nmove_controller
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NMOVE_CONTROLLER_H_
#define _NGAME_NMOVE_CONTROLLER_H_

#include "ncommon.h"
#include "ngameframework.h"

namespace nexus {

	struct ncheck_result;
	class nworld_object;

	class nmove_controller : public gameframework::ncharacter_controller
	{
	public:
		nmove_controller();
		virtual ~nmove_controller();

		void set_controlled(nworld_object* obj_ptr) { m_obj_ptr = obj_ptr; }
		// �Ƿ��ܷ���
		virtual bool can_fly() const;

		// ��ÿ�ε�����move����֮����ã��������ظú�����ʵ������ͬ���ȹ���
		virtual void post_move();

		// ��������ٶ������ʣ�����[0,1]
		virtual float maxspeed_modifier();

		// �ƶ���ʽ�����ı�Ļص�
		virtual void notify_ground();
		virtual void notify_jump();
		virtual void notify_fly();
		virtual void notify_custom();
		virtual void notify_hit(ncheck_result* hit);

	private:
		nworld_object*			m_obj_ptr;
	};

} // namespace nexus

#endif // _NGAME_NMOVE_CONTROLLER_H_
