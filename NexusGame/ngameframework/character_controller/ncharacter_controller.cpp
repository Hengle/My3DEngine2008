#include "stdafx.h"
#include "ncharacter_controller.h"
#include "../utility/ndebug_draw_interface.h"
#include "timer/nprofile.h"

namespace gameframework
{

	#define PROFILE_CC 0

	ncharacter_controller::ncharacter_controller()
	{
		m_walk = false;
		m_slope_limit = 0.707f;   // ~= cos(45��)
		m_step_height = 80.0f;
		m_accel_rate = 4096.0f;
		m_jump_speed = 1450.0f;
		m_max_ground_speed = 550.0f;
		m_walking_modifier = 0.4f;
		m_max_fly_speed = 1650.0f;
		m_speedup = 1.0f;
		m_current_movement_type = EMove_Ground;

		m_eyes_height = 80.f;

		m_saved_move = NULL;
	}

	ncharacter_controller::~ncharacter_controller()
	{

	}

	bool ncharacter_controller::reach_threshold_test( const vector3& test_position, const vector3& destination, float upThresholdAdjust, float downThresholdAdjust, float thresholdAdjust )
	{
		vector3 deltaDist = destination - test_position;

		if (m_current_movement_type == EMove_Fly)
		{
			upThresholdAdjust = 2.0f * upThresholdAdjust;
			downThresholdAdjust = 2.0f * downThresholdAdjust;
			thresholdAdjust = 2.0f * thresholdAdjust;
		}

		float diff = deltaDist.y;
		deltaDist.y = 0.0f;
		if (vec_length_sq(deltaDist) > thresholdAdjust * thresholdAdjust)
			return false;

		if ((diff > 0.0f) ? (abs(diff) > upThresholdAdjust) : (abs(diff) > downThresholdAdjust))
		{
			return false;
		}
		return true;
	}


	void ncharacter_controller::set_position( const vector3& position )
	{
		m_location = position;
	}

	const vector3& ncharacter_controller::get_position() const
	{
		return m_location;
	}

	void ncharacter_controller::set_rotation(const vector3& rot)
	{
		m_rotation = rot;
	}

	const vector3& ncharacter_controller::get_rotation() const
	{
		return m_rotation;
	}

	void ncharacter_controller::draw_debug_info( ndebug_draw_interface* ddi )
	{
		// ���Ƶ�ǰλ��
		ddi->draw_line( m_location, (m_location + vector3(0,1,0) * 200), color4f(0,1,0,1));
		ddi->draw_line( m_location, (m_location + vector3(-1,0,0) * 50), color4f(0,1,0,1));
		ddi->draw_line( m_location, (m_location + vector3(1,0,0) * 50), color4f(0,1,0,1));
		ddi->draw_line( m_location, (m_location + vector3(0,0,-1) * 50), color4f(0,1,0,1));
		ddi->draw_line( m_location, (m_location + vector3(0,0,1) * 50), color4f(0,1,0,1));

		// ���Ƽ��ٶ�
		ddi->draw_line( m_location, (m_location + m_acceleration * 100), color4f(0,0,1,1));
	
		// �����ٶ�
		ddi->draw_line( m_location, (m_location + m_velocity), color4f(1,0,0,1));

		// 
		TCHAR str_info[256];
		swprintf( str_info, _T("location: x= %.2f y= %.2f z= %.2f"), m_location.x, m_location.y, m_location.z);
		ddi->draw_debug_string(5,100, str_info,color4ub(0,255,0,255));
		
		swprintf( str_info, _T("velocity: x= %.2f y= %.2f z= %.2f"), m_velocity.x, m_velocity.y, m_velocity.z);
		ddi->draw_debug_string(5,114, str_info,color4ub(0,255,0,255));

		swprintf( str_info, _T("movement_type = %d"), m_current_movement_type);
		ddi->draw_debug_string(5,128, str_info,color4ub(0,255,0,255));
	}

	void ncharacter_controller::move( ngame_level* level, float delta_time )
	{
		if(delta_time <= 0.0f)
			return;

		// ����ٶ����ű���
		const float maxspeedmod = maxspeed_modifier();
		// ��������
		const float maxaccelrate = maxspeedmod * m_accel_rate;

		// ���㵱ǰ���ܺ���
		vector3 force = vector3::zero;
		float hor_max_speed = 0.f;		// ˮƽ����ٶ�
		float vert_max_speed = 0.f;		// ��ֱ����ٶ�
		composite_force(force, hor_max_speed, vert_max_speed);
		hor_max_speed *= maxspeedmod;

		// vertical pass
		float vert_accel = 0.f;
		if( is_zero(force.y) && !is_zero(m_velocity.y))
		{
			vert_accel = 0.f;
			m_velocity.y = 0.f;
		}
		else
		{
			vert_accel = maxaccelrate * force.y;
		}
		vert_accel = clamp(vert_accel, -maxaccelrate, maxaccelrate);
		m_velocity.y += vert_accel * delta_time;
		m_velocity.y = clamp(m_velocity.y, -vert_max_speed, vert_max_speed);

		// horizontal pass
		vector2 hor_vel = vector2(m_velocity.x, m_velocity.z);
		vector2 hor_accel_dir = vec_normalize(vector2(force.x, force.z));
		vector2 hor_accel = hor_accel_dir * maxaccelrate;
		if( vec_length_sq(hor_accel) <= nEPS && vec_length_sq(hor_vel) > nEPS  )
		{
			// ���ٶ�Ϊ�㣬�ٶȲ�Ϊ��ʱ����ͣ��
			vector2 old_vel = hor_vel;
			hor_vel -= vec_normalize(hor_vel) * maxaccelrate * delta_time;
			if (vec_dot(old_vel, hor_vel) < 0)
			{
				hor_vel = vector2(0.f,0.f);
			}
		}
		else
		{
			// final velocity = initial velocity + acceleration * time
			hor_vel += hor_accel * delta_time;
		}

		// �����ٶ�������ٶȷ�Χ��
		if (vec_length_sq(hor_vel) > hor_max_speed * hor_max_speed)
		{
			hor_vel = vec_normalize(hor_vel) * hor_max_speed;
		}
		
		m_velocity.x = hor_vel.x;
		m_velocity.z = hor_vel.y;

		// ����λ��
		vector3 disp = m_velocity * delta_time;

#ifdef PROFILE_CC
		START_PROFILE_COUNTER(_T("try_move_character"));
#endif	

		ncheck_result hit;
		if(!level->try_move_character(this, m_location, disp,m_step_height, hit))
		{
			if(hit.dist > nEPS)
			{
				notify_hit(&hit);
			}
		}

#ifdef PROFILE_CC
		END_PROFILE_COUNTER(_T("try_move_character"));
#endif	

		// �ƶ�����ִ�����֮����� ������������ʵ��ͬ�����߼�
		post_move();
	}

	bool ncharacter_controller::transport( ngame_level* level, const vector3& dest_pos,bool place_nav_above)
	{
		vector3 final_loc = dest_pos;
		if( level->try_transport(final_loc,place_nav_above) )
		{
			set_position(final_loc);
			return true;
		}
		return false;
	}

	float ncharacter_controller::maxspeed_modifier()
	{
		float ret = m_speedup;
		if (m_walk)
		{
			ret *= m_walking_modifier;
		}
		return ret;
	}

	bool ncharacter_controller::can_jump() const
	{
		return (m_current_movement_type==EMove_Ground);
	}

	bool ncharacter_controller::can_fly() const
	{
		return true;
	}

	void ncharacter_controller::fly()
	{
		if (!can_fly())
			return;

		set_current_movement_type(EMove_Fly);
	}

	void ncharacter_controller::jump()
	{
		if (!can_jump())
			return;

		set_current_movement_type(EMove_Jump);
	}

	void ncharacter_controller::set_current_movement_type( EMovementType move_type )
	{
		EMovementType old_move_type = m_current_movement_type;
		
		m_current_movement_type=move_type;
	
		if(old_move_type != m_current_movement_type)
		{
			if(m_current_movement_type == EMove_Ground)
			{
				notify_ground();
			}
			else if(m_current_movement_type == EMove_Jump)
			{
				m_velocity.y = m_jump_speed;
				notify_jump();
			}
			else if(m_current_movement_type == EMove_Fly)
			{
				notify_fly();
			}
			else if(m_current_movement_type == EMove_Custom)
			{
				notify_custom();
			}
		}
	}


	void ncharacter_controller::_on_land()
	{
		if(m_current_movement_type == EMove_Jump)
		{
			set_current_movement_type(EMove_Ground);
		}
	}

	void ncharacter_controller::notify_ground()
	{
		character_evnet_arg arg(this);
		ground_event(&arg);
	}

	void ncharacter_controller::notify_jump()
	{
		character_evnet_arg arg(this);
		ground_event(&arg);
	}

	void ncharacter_controller::notify_fly()
	{
		character_evnet_arg arg(this);
		ground_event(&arg);
	}

	void ncharacter_controller::notify_custom()
	{
		character_evnet_arg arg(this);
		ground_event(&arg);
	}


	void ncharacter_controller::notify_hit( ncheck_result* hit )
	{
		character_hit_evnet_arg arg(this, hit);
		hit_event(&arg);
	}

	void ncharacter_controller::composite_force( vector3 &force, float &hor_max_speed, float &vert_max_speed )
	{
		const vector3 gravity = vector3(0.f, -1.0f, 0.f);
		switch (m_current_movement_type)
		{
		case EMove_Ground: 
			{
				force += m_acceleration + gravity;
				hor_max_speed = m_max_ground_speed;
				vert_max_speed = m_jump_speed;
			}
			break;
		case EMove_Jump:
			{
				force += m_acceleration + gravity;
				hor_max_speed = m_max_ground_speed;
				vert_max_speed = m_jump_speed;
			}
			break;
		case EMove_Fly:
			{
				force += m_acceleration;
				hor_max_speed = m_max_fly_speed;
				vert_max_speed = m_max_fly_speed;
			}
			break;
		case EMove_Custom:
			custom_move(force, hor_max_speed,vert_max_speed);
			break;
		}
	}

	void ncharacter_controller::update( float delta_time )
	{
		// ��saved move�����Ƿ���Ҫreplication
		if(m_saved_move != NULL)
		{
			m_saved_move->update(delta_time, this);
		}
	}

	void ncharacter_controller::set_saved_move( nsavedmove* savedmove_ptr )
	{
		m_saved_move = savedmove_ptr;
		m_saved_move->init(this);
	}
} // end of namespace gameframework