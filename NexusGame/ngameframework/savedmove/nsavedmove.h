#ifndef _NEXUS_GAMEFRAMEWORK_SAVEDMOVE_H_
#define _NEXUS_GAMEFRAMEWORK_SAVEDMOVE_H_
#include "ncore.h"
#include "../character_controller/nmovement_define.h"
#include "replicate_move_callback.h"

namespace gameframework
{
	class ncharacter_controller;

	/**
	* class nsavedmove
	* @remark	��Ҫ��;Ϊ�ж��Ƿ���Ҫͬ���ƶ��ṩһ��ͳһ�Ĳ���
				��¼������һ���NPC���ʱ���ڵ��ƶ���
				�����ƶ���Ϣ��ͬ�����ԣ�������ѹ��
				Ϊ�˸��õĽ�ʡ�ƶ�ͬ����Ϣ��
	*/
	class nGameFrameworkAPI nsavedmove
	{
	public:
		typedef uint8	move_flags;
		typedef boost::shared_ptr<nsavedmove> ptr;

		enum move_flag_type
		{
			EMFT_Walk		= 1<<0,
			EMFT_Jump		= 1<<1,
			EMFT_Fly		= 1<<2,
			// ע��PressJump�����������ƽ�ɫ��Ծ�Ĳ�����EMFT_Jumpֻ�Ǳ�ʶ��ǰ��״̬ΪJUMP
			// ������Ϊ���粻�ȶ����µ�Զ�̽�ɫ�����Ծ����
			EMFT_PressJump	= 1<<3,

			EMFT_Custom		= 1<<7,
		};

		nsavedmove(replicate_move_callback_ptr replicate);
		virtual ~nsavedmove();

		// init
		virtual void init(ncharacter_controller* controller);
		
		// update
		virtual void update(float delta_time, ncharacter_controller* controller);

		// ѹ��/��ѹ���ƶ������������ø���ɫ������
		// �������ƶ�����ѹ��������m_walk��jump, fly��
		// param: custom_flag �����ⲿ����һЩ�Զ���ı�ǣ��ú�����ʹ��ֻ����m_walk��jump, fly��������־λ����
		static move_flags compressed_controller_flags(ncharacter_controller* cc, move_flags custom_flag);
		static void flags_to_controller(move_flags flags, ncharacter_controller* cc);

		const vector3& get_saved_acceleration() const
		{
			return m_saved_acceleration;
		}

		const vector3& get_saved_velocity() const
		{
			return m_saved_velocity;
		}

		const vector3& get_saved_location()const
		{
			return m_saved_location;
		}

		const vector3& get_saved_rotation() const
		{
			return m_saved_rotation;
		}

		bool get_saved_walk() const
		{
			return m_walk;
		}

		EMovementType get_saved_movement_type() const
		{
			return m_movement_type;
		}


	protected:
		// �����ƶ�ͬ��
		virtual void replicate_move();

		// �ж��Ƿ�����Ҫ�ƶ�
		virtual bool is_important_movement(ncharacter_controller* controller);

	private:
		void reset_move_parm(ncharacter_controller* controller);

	protected:
		// ���ͬ�����ʱ��(��λ: s)��ͬ������������ֵ�Ǳ���ͬ��һ��
		float m_max_response_time;

		// ��¼�����ƶ���ʱ���
		float m_timestamp;
		// ����/�ܣ�true:�� false:��
		bool m_walk;
		// �ƶ�����
		EMovementType m_movement_type;
		// λ��
		vector3 m_saved_location;
		// �ٶ�
		vector3 m_saved_velocity;
		// ���ٶ�
		vector3 m_saved_acceleration;
		// ����,����Ϊ��λ
		vector3 m_saved_rotation;
		// ���ٶȷ�ֵ
		float m_acceldotthreshold;
	
		// ͳ��replicate move����
		unsigned int m_num_replicates;

		// replicate�ص�����
		replicate_move_callback_ptr m_replicate_move_callback;
	};
}

#endif // end of _NEXUS_GAMEFRAMEWORK_SAVEDMOVE_H_