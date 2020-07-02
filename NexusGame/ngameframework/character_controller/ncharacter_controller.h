#ifndef _NEXUS_GAMEFRAMEWORK_CHARACTER_CONTROLLER_H_
#define _NEXUS_GAMEFRAMEWORK_CHARACTER_CONTROLLER_H_
#include "ncore.h"
#include "ntype_def.h"
#include "nmovement_define.h"
#include "../world/ngame_level.h"
#include "../savedmove/nsavedmove.h"

namespace gameframework
{
	class ndebug_draw_interface;
	
	class character_evnet_arg : public nevent_arg
	{
	public:
		ncharacter_controller* m_character;

		explicit character_evnet_arg(ncharacter_controller* _character) : m_character(_character) {};
	};

	class character_hit_evnet_arg : public character_evnet_arg
	{
	public:
		ncheck_result*		m_hit;

		explicit character_hit_evnet_arg(ncharacter_controller* _character, ncheck_result* hit) 
			: character_evnet_arg(_character),m_hit(hit)
		{};
	};



	/**
	* �����ڵ���ͼ�Ľ�ɫ���ƻ��࣬���ඨ���˽�ɫ�˶���صĲ����ͽ�ɫ�˶�����
	* ��Ϸ�߼�����Լ̳и���ʵ�ֲ�ͬ���ԵĽ�ɫ����
	*/
	class nGameFrameworkAPI ncharacter_controller 
		: public nexus::nobject
	{
	public:
		typedef boost::shared_ptr<ncharacter_controller> ptr;

		// �¼�����
		typedef nevent_handler<void, character_evnet_arg*> character_event;
		typedef nevent_handler<void, character_hit_evnet_arg*> character_hit_event;


		ncharacter_controller();
		virtual ~ncharacter_controller();
	
		// ��ʼ��
		virtual bool init() { return true; }
		
		// ����ʱ����
		virtual void destroy() {}

		// update,��Ҫ��ÿ��Tick�е���
		virtual void update(float delta_time);

		// ����savedmove��һ��ֻ����һ�μ���
		void set_saved_move(nsavedmove* savedmove_ptr);

		// �ƶ���ɫ
		// level: ���ĸ��������˶���������ɫ�볡������ײ
		// delta_time: ʱ����sΪ��λ
		virtual void move(ngame_level* level, float delta_time);

		// ������ĳ����
		// param dest_pos: ��Ҫ��������Ŀ���
		// param place_nav_above: �Ƿ���Ҫ����ɫ���õ�����ͼ������
		virtual bool transport(ngame_level* level, const vector3& dest_pos, bool place_nav_above);

		// debug draw interface
		virtual void draw_debug_info(ndebug_draw_interface* ddi);

		// ��ǰ�Ƿ�������Ծ
		virtual bool can_jump () const;

		// �л�����Ծ,ִ����Ծ����
		virtual void jump();

		// ��ǰ�Ƿ��������
		virtual bool can_fly() const;
		// �л�������״̬
		virtual void fly();

		//-- Ŀ����Խӿ�
		bool reached_destination(const vector3& test_position, const vector3&  destination)
		{
			return reach_threshold_test(test_position, destination, 40, 40, 40);
		}

		// ���ݷ�ֵ������û�е���Ŀ���
		bool reach_threshold_test(const vector3&  test_position, const vector3&  destination, float upThresholdAdjust, float downThresholdAdjust, float thresholdAdjust);

		// ���ý�ɫλ��
		void set_position(const vector3& position);
		// ��ȡ��ǰ��ɫλ��
		const vector3& get_position() const;

		// ���ý�ɫ����
		void set_rotation(const vector3& rot);
		// ��ȡ��ǰ��ɫ����
		const vector3& get_rotation() const;

		// ���ý�ɫ�ļ��ٶ�
		void set_acceleration(const vector3& accel)
		{
			m_acceleration = vec_normalize(accel);
		}
		// ��ȡ��ɫ��ǰ���ٶ�
		const vector3& get_acceleration() const
		{
			return m_acceleration;
		}

		// ���ý�ɫ�ļ��ٶ�
		void set_velocity(const vector3& velocity)
		{
			m_velocity = velocity;
		}
		// ��ȡ��ɫ��ǰ���ٶ�
		const vector3& get_velocity() const
		{
			return m_velocity;
		}

		EMovementType get_current_movement_type() const { return m_current_movement_type; }

		// �ı䵱ǰ�ƶ�����
		virtual void set_current_movement_type(EMovementType move_type);

		// ����/��
		bool get_walk() const {  return m_walk; }
		void set_walk(bool value) { m_walk = value; }
		
		// �۾��ĸ߶�
		float get_eyes_height() const { return m_eyes_height; }
		void set_eyes_height(float value) { m_eyes_height = value; } 

		float get_jump_speed() const {  return m_jump_speed; }
		void set_jump_speed(float value) { m_jump_speed = value; }

		float get_speedup() const {  return m_speedup; }
		void set_speedup(float value) { m_speedup = value; }

		float get_max_ground_speed() const {  return m_max_ground_speed; }
		void set_max_ground_speed(float value) { m_max_ground_speed = value; }

		float get_max_fly_speed() const {  return m_max_fly_speed; }
		void set_max_fly_speed(float value) { m_max_fly_speed = value; }

		uint64 get_character_id() const {  return m_character_id; }
		void set_character_id(uint64 value) { m_character_id = value; }

		// ���(�ƶ�ʱ��������)���ڲ�����
		void _on_land();

	protected:
		// ���㵱ǰ���еĺ���͸����˶�״̬���ص�����ٶ�
		// param force: �������к���
		// param hor_max_speed: ˮƽ����ٶ�
		// param vert_max_speed: ��ֱ����ٶ�
		virtual void composite_force(vector3 &force, float &hor_max_speed, float &vert_max_speed);

		// ��ÿ�ε�����move����֮����ã��������ظú�����ʵ������ͬ���ȹ���
		virtual void post_move() {}

		// �����������Զ�����ƶ���������
		virtual void custom_move(vector3& /*force*/, float& /*hor_max_speed*/, float& /*vert_max_speed*/) {}

		// ��������ٶ������ʣ�����[0,1]
		virtual float maxspeed_modifier();

		// �ƶ���ʽ�����ı�Ļص�
		virtual void notify_ground();
		virtual void notify_jump();
		virtual void notify_fly();
		virtual void notify_custom();
		virtual void notify_hit(ncheck_result* hit);

	public:
		// �����¼����ⲿʹ�÷�ʽ����,�ο�/event_delegate/sample.txt��
		// test_delegate_class test_class;
		// ground_event += new character_event::T<test_delegate_class>(&test_class,&test_delegate_class::do_somthing);
		// ��ɫ�л���EMove_Ground�ƶ�״̬�¼�
		character_event		ground_event;
		// ��ɫ�л���EMove_Jump�ƶ�״̬�¼�
		character_event		jump_event;
		// ��ɫ�л���EMove_Fly�ƶ�״̬�¼�
		character_event		fly_event;
		// ��ɫ�л���EMove_Custom�ƶ�״̬�¼�
		character_event		custom_event;

		// �ƶ��������赲
		character_hit_event	hit_event;

	private:
		// ��ɫID
		uint64		m_character_id;

		// ��ɫλ��
		vector3		m_location;
		// ��ɫ����
		vector3		m_rotation;
		// ��ǰ�ٶȣ����ڲ����㲻���ڱ仯
		vector3		m_velocity;
		// ��ɫ���ٶ�,ֻ���ⲿ���øı�
		vector3		m_acceleration;
		// ��ǰ�ƶ�����
		EMovementType m_current_movement_type;

		// ����/�ܣ� true���� false����
		bool		m_walk;

		//-- ��ɫ�˶���������
		// ��ɫ������������¶�,~= cos( *��)
		float		m_slope_limit;
		// ����
		float		m_step_height;

		// ������
		float		m_accel_rate;

		// �����ٶ�
		float		m_jump_speed;

		// �ڵ������ܲ�������ٶ�
		float		m_max_ground_speed;
		// �ߵ�����ٶ� = �ܵ�����ٶ� * m_walking_modifier
		float		m_walking_modifier;
		float		m_max_fly_speed;
		// ����ָ�����ⲿ��������ٶ�������
		float		m_speedup;

		// �۾��ĸ߶�
		float		m_eyes_height;

		// �����ж��Ƿ���Ҫ�ƶ�ͬ��
		nsavedmove* m_saved_move;
	};
}

#endif // end of _NEXUS_GAMEFRAMEWORK_CHARACTER_CONTROLLER_H_