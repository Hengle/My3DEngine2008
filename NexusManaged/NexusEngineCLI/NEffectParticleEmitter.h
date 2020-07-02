#pragma once
#include "NEffectElement.h"
#include "NDistributionAll.h"

namespace NexusEngine
{
	//���ӷ���������״
	public enum class EsfxEmitterShape
	{
		SphereShape			= ESFX_SphereShape,
		BoxShape			= ESFX_BoxShape,
		ConeShape			= ESFX_ConeShape,
	};

	//���ӵ��˶���ʽ
	public enum class EsfxMotionType
	{
		FreeMotion			= ESFX_FreeMotion,
		TrackMotion			= ESFX_TrackMotion,	
		ExplodeMotion		= ESFX_ExplodeMotion,
		AssembleMotion		= ESFX_AssembleMotion,
	};

	public ref class NEffectParticleEmitter : public NEffectElement
	{
	public:
		NEffectParticleEmitter(nexus::neffect_particle_emitter::ptr nativeObj);

		[Category("����"), Description("���ӵ���Ƭ����")]
		property enum class EsfxFaceType FaceType
		{
			enum class EsfxFaceType get()			{	return (enum class EsfxFaceType)NativePtr->m_config.m_face_type;}
			void set(enum class EsfxFaceType val)	{	NativePtr->m_config.m_face_type = (nexus::ESFX_FaceType)val;}
		}

		[Category("����"), Description("���ӵĳ�ʼλ��")]
		property Vector3 InitPos 
		{
			virtual Vector3 get() override
			{
				return Vector3::FromNative(NativePtr->m_init_pos);
			}
			virtual void set(Vector3 val) override
			{
				NativePtr->m_init_pos = val.ToNative();
				NativePtr->m_config.m_init_position=val.ToNative();
			}
		}

		[Category("����"), Description("���ӵĳ�ʼ�ٶȣ��Թ켣��Ϊ��������Ч")]
		property Vector3 InitSpeed 
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_config.m_init_speed);
			}
			void set(Vector3 val)
			{
				NativePtr->m_config.m_init_speed=val.ToNative();
			}
		}

		[Category("����"), Description("���ӵĳ�ʼ��ת")]
		property Vector3 InitRot
		{
			virtual Vector3 get() override
			{
				return Vector3::FromNative(NativePtr->m_init_rot);
			}
			virtual void set(Vector3 val) override
			{
				NativePtr->m_init_rot = val.ToNative();
				NativePtr->m_config.m_init_rot=val.ToNative();
			}
		}

		[Category("����"), Description("���ӵĳ�ʼ��ת")]
		property Vector3 InitRevolution
		{
			virtual Vector3 get() override
			{
				return Vector3::FromNative(NativePtr->m_init_revolution);
			}
			virtual void set(Vector3 val) override
			{
				NativePtr->m_init_revolution = val.ToNative();
				NativePtr->m_config.m_init_revo = val.ToNative();
			}
		}

		[Category("����"), Description("����������Ϊ����")]
		property enum class EsfxMotionType MotionType
		{
			enum class EsfxMotionType get()			{	return (enum class EsfxMotionType)NativePtr->m_config.m_motion;}
			void set(enum class EsfxMotionType val)	{	NativePtr->m_config.m_motion = (nexus::ESFX_MotionType)val;}
		}

		[Category("����"), Description("���Ӵ�С��ʱ��仯ֵ")]
		property NDistributionVector2^ DSize
		{
			NDistributionVector2^ get()
			{
				return m_size;
			}
			void set(NDistributionVector2^ val)
			{
				m_size = val;
				NativePtr->m_size = m_size->GetNativeSmartPtr();
				NativePtr->m_config.m_size= m_size->GetNativeSmartPtr();				
			}
		}

		[Category("����"), Description("���ӵ�ɫ����ʱ��仯ֵ")]
		property NDistributionColor^ Colors
		{
			NDistributionColor^ get()
			{
				return m_color;
			}
			void set(NDistributionColor^ val)
			{
				m_color = val;
				NativePtr->m_color = val->GetNativeSmartPtr();
				NativePtr->m_config.m_color = val->GetNativeSmartPtr();
			}
		}

		[Category("����"), Description("���ӵ��ƶ��ٶ���ʱ��仯ֵ")]
		property NDistributionVector3^ LinearVel
		{
			NDistributionVector3^ get()
			{
				return m_linear_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_vel = val->GetNativeSmartPtr();
				NativePtr->m_config.m_vel=val->GetNativeSmartPtr();
				m_linear_vel = val;
			}
		}

		[Category("����"), Description("���ӵĹ�ת�ٶ���ʱ��仯ֵ")]
		property NDistributionVector3^ RevolutionVel
		{
			NDistributionVector3^ get()
			{
				return m_revolution_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_revolution_vel= val->GetNativeSmartPtr();
				NativePtr->m_config.m_revolution_vel= val->GetNativeSmartPtr();
				m_revolution_vel = val;
			}
		}

		[Category("����"), Description("���ӵ���ת�ٶ���ʱ��仯ֵ")]
		property NDistributionVector3^ AngularVel
		{
			NDistributionVector3^ get()
			{
				return m_angular_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_rotate_vel= val->GetNativeSmartPtr();
				NativePtr->m_config.m_rotate_vel= val->GetNativeSmartPtr();
				m_angular_vel = val;
			}
		}

		[Category("����"), Description("���ӵ�������Ư��")]
		property float MaxDrift
		{
			float get()
			{
				return NativePtr->m_config.m_max_drift;
			}
			void set(float val)
			{
				NativePtr->m_config.m_max_drift= val;
			}
		}

		[Category("����"), Description("���ӵ���С���Ư��")]
		property float MinDrift
		{
			float get()
			{
				return NativePtr->m_config.m_min_drift;
			}
			void set(float val)
			{
				NativePtr->m_config.m_min_drift= val;
			}
		}

		[Category("����"), Description("�������ٶȣ�0��ʾ��������Ӱ��")]
		property float Gravity
		{
			virtual float get() override
			{
				return NativePtr->m_gravity;
			}
			virtual void set(float val) override
			{
				NativePtr->m_gravity = val;
				NativePtr->m_config.m_gravity=val;
			}
		}

		[Category("����"), Description("��������")]
		property float Damp
		{
			float get()
			{
				return NativePtr->m_config.m_damp;
			}
			void set(float val)
			{
				NativePtr->m_config.m_damp=val;
			}
		}

		[Category("����"), Description("���ӵ���������")]
		property float LifeTime
		{
			float get()
			{
				return NativePtr->m_config.m_particle_life;
			}
			void set(float val)
			{
				NativePtr->m_config.m_particle_life=val;
			}
		}

		[Category("���ӷ�����"), Description("ÿ�뷢�����������")]
		property int EmitCountPerSecond
		{
			int get()
			{
				return NativePtr->m_config.m_emit_speed;
			}
			void set(int val)
			{
				NativePtr->m_config.m_emit_speed=val;
			}
		}

		[Category("���ӷ�����"), Description("�����������")]
		property int ParticleCount
		{
			int get()
			{
				return NativePtr->m_config.m_max_particle_count;
			}
			void set(int val)
			{
				NativePtr->m_config.m_max_particle_count=val;
			}
		}

		[Category("���ӷ�����"), Description("��ʼʱһ�����ɵ���������")]
		property int InitParticleCount
		{
			int get()
			{
				return NativePtr->m_config.m_init_particle_count;
			}
			void set(int val)
			{
				NativePtr->m_config.m_init_particle_count=val;
			}
		}

		[Category("���ӷ�����"), Description("��������״����")]
		property Vector3 EmitterShapeParams
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_config.m_emitter_shape_parameter);
			}
			void set(Vector3 val)
			{
				NativePtr->m_config.m_emitter_shape_parameter= val.ToNative();
			}
		}

		[Category("���ӷ�����"), Description("���ӷ�������״")]
		property enum class EsfxEmitterShape EmitterShape
		{
			enum class EsfxEmitterShape get()			{	return (enum class EsfxEmitterShape)NativePtr->m_config.m_emitter_shape;}
			void set(enum class EsfxEmitterShape val)	{	NativePtr->m_config.m_emitter_shape = (nexus::ESFX_EmitterShape)val;}
		}

		[Category("���ӷ�����"), Description("���ӷ�������ʼλ��")]
		property Vector3 EmitterInitPos
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_config.m_emitter_init_pos);
			}
			void set(Vector3 val)
			{
				NativePtr->m_config.m_emitter_init_pos= val.ToNative();
			}
		}

		[Category("���ӷ�����"), Description("���ӷ��������ƶ��ٶ���ʱ��仯ֵ")]
		property NDistributionVector3^ EmitterLinearVel
		{
			NDistributionVector3^ get()
			{
				return m_emitter_linear;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_emitter_vel = val->GetNativeSmartPtr();
				m_emitter_linear = val;
			}
		}

		[Category("���ӷ�����"), Description("���ӷ������Ĺ�ת�ٶ���ʱ��仯ֵ")]
		property NDistributionVector3^ EmitterRevolutionVel
		{
			NDistributionVector3^ get()
			{
				return m_emitter_revolution;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_emitter_revo= val->GetNativeSmartPtr();
				m_emitter_revolution = val;
			}
		}

		[Category("���ӷ�����"), Description("���ӷ���������ת�ٶ���ʱ��仯ֵ")]
		property NDistributionVector3^ EmitterAngularVel
		{
			NDistributionVector3^ get()
			{
				return m_emitter_angular;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_emitter_rot= val->GetNativeSmartPtr();
				m_emitter_angular = val;
			}
		}

		virtual void Init(String^ name) override;
	protected:
		property neffect_particle_emitter* NativePtr
		{
			neffect_particle_emitter* get();
		}

	protected:
		//���ӳߴ�
		NDistributionVector2^	m_size;
		//�������ٶ�
		NDistributionVector3^	m_linear_vel;
		//���ӽ��ٶ�
		NDistributionVector3^	m_revolution_vel;
		//������ת���ٶ�
		NDistributionVector3^	m_angular_vel;
		//����ɫ��
		NDistributionColor^		m_color;
		//�������ٶ�
		NDistributionVector3^	m_emitter_linear;
		//��������ת���ٶ�
		NDistributionVector3^	m_emitter_revolution;
		//��������ת���ٶ�
		NDistributionVector3^	m_emitter_angular;
	};
}//namespace NexusEngine