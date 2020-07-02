#ifndef _NEXUS_ANIM_CONTROLLER_H_
#define _NEXUS_ANIM_CONTROLLER_H_
#include "ncore.h"
#include "anim_define.h"

namespace nexus
{
	class nAPI nanim_controller :
		public nobject
	{
	public:
		nanim_controller(void)			{		}
		virtual ~nanim_controller(void)	{		}	

		virtual void tick_anim(float delta_time) = 0;

		// ���õ�ǰ���Ŷ�������
		virtual void set_anim(const nstring& sequence) = 0;

		// ����ָ������������ǰ����ָ�����������ƣ��ο�set_anim
		// param loop : �Ƿ�ѭ������
		// param rate : ��������
		// param start_time : ��ʼʱ��
		virtual void play_anim(bool loop, float rate, float start_time) = 0;

		// ֹͣ���Ŷ���
		virtual void stop_anim() = 0;

		// ���õ�ǰanim sequenceλ��
		virtual void set_position(float new_time, bool fire_notifies) = 0;

		// ��ȡ��ǰanim sequenceʣ��ʱ��
		virtual float get_time_left() const = 0;
		// ��ȡ��ǰanim sequence�ܳ���
		virtual float get_anim_length() const = 0;
		// ��ȡ��ǰanim sequence��֡��
		virtual size_t get_anim_frame() const = 0;
		// ��ȡ��ǰanim sequence
		virtual const TCHAR* get_anim_playing() const = 0;

		// advance animation
		virtual void advance_by(float move_deta, float delta_time, bool fire_notifies) = 0; 

		virtual void issue_notifies(float delta_time) = 0;

		virtual void on_anim_end(const nstring& sequence, float played_time) = 0;

		nDECLARE_VIRTUAL_CLASS(nanim_controller)
	};
}//namespace nexus

#endif //_NEXUS_ANIM_CONTROLLER_H_