#pragma once
#include "NEffectElement.h"
#include "NDistributionAll.h"
#include "SubUV.h"

namespace NexusEngine
{
	//���������
	public enum class EsfxGridType
	{
		PlaneGrid			= neffect_grid_plane,
		SphereGrid			= neffect_grid_sphere_billboard,	
		CylinderFrid		= neffent_grid_cylinder_billboard,
	};

	ref class NEffectGrid :
		public NEffectElement
	{
	public:
		NEffectGrid(nexus::neffect_grid::ptr nativeObj);

		[Category("Grid")]
		property enum class EsfxGridType GridType
		{
			enum class EsfxGridType get()			{	return (enum class EsfxGridType)NativePtr->m_type;}
			void set(enum class EsfxGridType val)	{	NativePtr->m_type = (nexus::ESFX_GridType)val;}
		}

		[Category("Grid")]
		property int LengthDiv
		{
			int get()			{	return NativePtr->m_length_div;}
			void set(int val)	{	NativePtr->m_length_div = val;}
		}

		[Category("Grid")]
		property int WidthDiv
		{
			int get()			{	return NativePtr->m_width_div;}
			void set(int val)	{	NativePtr->m_width_div = val;}
		}

		[Category("Grid")]
		property float WaveSpeed
		{
			float get()			{	return NativePtr->m_wave_speed;}
			void set(float val)	{	NativePtr->m_wave_speed = val;}
		}

		[Category("Grid")]
		property NDistributionVector3^ WavePower
		{
			NDistributionVector3^ get()			{	return m_wave_power;}
			void set(NDistributionVector3^ val)	
			{	
				m_wave_power=val;
				NativePtr->m_wave_power = val->GetNativeSmartPtr();
			}
		}

		[Category("Grid")]
		property float WaveAttenuation
		{
			float get()			{	return NativePtr->m_attenuation;}
			void set(float val)	{	NativePtr->m_attenuation = val;}
		}

		[Category("Grid"), Description("size by time")]
		property NDistributionVector2^ DSize
		{
			NDistributionVector2^ get()
			{
				return m_size;
			}
			void set(NDistributionVector2^ val)
			{
				NativePtr->m_size = val->GetNativeSmartPtr();
				m_size = val;
			}
		}

		[Category("Grid")]
		property NDistributionColor^ Colors
		{
			NDistributionColor^ get()
			{
				return m_color;
			}
			void set(NDistributionColor^ val)
			{
				NativePtr->m_color = val->GetNativeSmartPtr();
				m_color = val;
			}
		}

		[Category("Grid")]
		property NDistributionVector3^ LinearVel
		{
			NDistributionVector3^ get()
			{
				return m_linear_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_vel = val->GetNativeSmartPtr();
				m_linear_vel = val;
			}
		}

		[Category("Grid")]
		property NDistributionVector3^ RevolutionVel
		{
			NDistributionVector3^ get()
			{
				return m_revolution_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_revolution_vel= val->GetNativeSmartPtr();
				m_revolution_vel = val;
			}
		}

		[Category("Grid")]
		property NDistributionVector3^ AngularVel
		{
			NDistributionVector3^ get()
			{
				return m_angular_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_rotate_vel= val->GetNativeSmartPtr();
				m_angular_vel = val;
			}
		}

		virtual void Init(String^ name) override;
	protected:
		property neffect_grid* NativePtr
		{
			neffect_grid* get()
			{
				return ndynamic_cast<neffect_grid>( m_smart_ptr->get() );
			}
		}
	protected:
		//���ӳߴ�
		NDistributionVector2^	m_size;
		//����ɫ��
		NDistributionColor^		m_color;
		//�������ٶ�
		NDistributionVector3^	m_linear_vel;
		//���ӽ��ٶ�
		NDistributionVector3^	m_revolution_vel;
		//������ת���ٶ�
		NDistributionVector3^	m_angular_vel;
		//����ǿ��
		NDistributionVector3^	m_wave_power;
	};

}
