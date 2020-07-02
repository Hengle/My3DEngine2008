#pragma once
#include "NEffectElement.h"
#include "NDistributionAll.h"

namespace NexusEngine
{
	public ref class NEffectCylinder : public NEffectElement
	{
	public:
		NEffectCylinder(nexus::neffect_cylinder::ptr nativeObj);

		[Category("Cylinder"), Description("dividing")]
		property int DivCount
		{
			int  get()			{	return NativePtr->m_div; }
			void set(int val)	{	NativePtr->m_div = val; }
		}

		[Category("Cylinder"), Description("UV scaling factor")]
		property Vector2 UVScale
		{
			Vector2  get()			{	return Vector2::FromNative(NativePtr->m_uv_scale); }
			void set(Vector2 val)	{	NativePtr->m_uv_scale = val.ToNative(); }
		}

		[Category("Cylinder"), Description("circle count")]
		property NDistributionFloat^ CircleCount
		{
			NDistributionFloat^  get()			{	return m_num_circle; }
			void set(NDistributionFloat^ val)	
			{	
				m_num_circle=val;
				NativePtr->m_num_circle = val->GetNativeSmartPtr(); 
			}
		}

		[Category("Cylinder"), Description("height")]
		property NDistributionFloat^ Height
		{
			NDistributionFloat^  get()			{	return m_height; }
			void set(NDistributionFloat^ val)	
			{	
				m_height=val;
				NativePtr->m_height = val->GetNativeSmartPtr(); 
			}
		}

		[Category("Cylinder"), Description("top radius")]
		property NDistributionFloat^ TopRadius
		{
			NDistributionFloat^  get()			{	return m_top_radius; }
			void set(NDistributionFloat^ val)	
			{	
				m_top_radius=val;
				NativePtr->m_top_radius = val->GetNativeSmartPtr(); 
			}
		}

		[Category("Cylinder")]
		property NDistributionFloat^ BottomRadius
		{
			NDistributionFloat^  get()			{	return m_bottom_radius; }
			void set(NDistributionFloat^ val)	
			{	
				m_bottom_radius=val;
				NativePtr->m_bottom_radius = val->GetNativeSmartPtr(); 
			}
		}

		[Category("Cylinder")]
		property NDistributionFloat^ TopBottomStep
		{
			NDistributionFloat^  get()			{	return m_div_top_radius_inc; }
			void set(NDistributionFloat^ val)	
			{	
				m_div_top_radius_inc=val;
				NativePtr->m_div_top_radius_inc = val->GetNativeSmartPtr(); 
			}
		}

		[Category("Cylinder")]
		property NDistributionFloat^ BottomRadiusStep
		{
			NDistributionFloat^  get()			{	return m_div_bottom_radius_inc; }
			void set(NDistributionFloat^ val)	
			{	
				m_div_bottom_radius_inc=val;
				NativePtr->m_div_bottom_radius_inc = val->GetNativeSmartPtr(); 
			}
		}

		[Category("Cylinder")]
		property NDistributionFloat^ TopShrink
		{
			NDistributionFloat^  get()			{	return m_div_top_radius_shrink; }
			void set(NDistributionFloat^ val)	
			{	
				m_div_top_radius_shrink=val;
				NativePtr->m_div_top_radius_shrink = val->GetNativeSmartPtr(); 
			}
		}

		[Category("Cylinder")]
		property NDistributionFloat^ BotoomShrink
		{
			NDistributionFloat^  get()			{	return m_div_bottom_radius_shrink; }
			void set(NDistributionFloat^ val)	
			{	
				m_div_bottom_radius_shrink=val;
				NativePtr->m_div_bottom_radius_shrink = val->GetNativeSmartPtr(); 
			}
		}

		[Category("Cylinder")]
		property NDistributionFloat^ TopRise
		{
			NDistributionFloat^  get()			{	return m_div_top_rise; }
			void set(NDistributionFloat^ val)	
			{	
				m_div_top_rise=val;
				NativePtr->m_div_top_rise = val->GetNativeSmartPtr(); 
			}
		}

		[Category("Cylinder")]
		property NDistributionFloat^ BottomRise
		{
			NDistributionFloat^  get()			{	return m_div_bottom_rise; }
			void set(NDistributionFloat^ val)	
			{	
				m_div_bottom_rise=val;
				NativePtr->m_div_bottom_rise = val->GetNativeSmartPtr(); 
			}
		}

		[Category("Cylinder")]
		property NDistributionColor^ TopColor
		{
			NDistributionColor^  get()			{	return m_top_color; }
			void set(NDistributionColor^ val)	
			{	
				m_top_color=val;
				NativePtr->m_top_color = val->GetNativeSmartPtr(); 
			}
		}

		[Category("Cylinder")]
		property NDistributionColor^ BottomColor
		{
			NDistributionColor^  get()			{	return m_bottom_color; }
			void set(NDistributionColor^ val)	
			{	
				m_bottom_color=val;
				NativePtr->m_bottom_color = val->GetNativeSmartPtr(); 
			}
		}

		virtual void Init(String^ name) override;

	protected:

		property neffect_cylinder* NativePtr
		{
			neffect_cylinder* get(){return ndynamic_cast<neffect_cylinder>( m_smart_ptr->get() );}
		}
	private:
		//-----------------------------------------------------------------------------------
		NDistributionFloat^	m_num_circle;				// Ȧ��
		NDistributionFloat^	m_height;					// �߶ȣ���ֱ���򳤶ȣ�
		NDistributionFloat^	m_top_radius;				// �����뾶
		NDistributionFloat^	m_bottom_radius;			// �ײ��뾶
		NDistributionFloat^	m_div_top_radius_inc;		// �ָ�ʱ���뾶�����ٶ�
		NDistributionFloat^	m_div_bottom_radius_inc;	// �ָ�ʱ�װ뾶�����ٶ�
		NDistributionFloat^	m_div_top_radius_shrink;	// �ָ�ʱ��ÿ��һ���㣬���뾶�����̶�,�������������,��â�ǵ�
		NDistributionFloat^	m_div_bottom_radius_shrink;
		NDistributionFloat^	m_div_top_rise;				// ÿ�ηָ��߶������ٶ�
		NDistributionFloat^	m_div_bottom_rise;			// ÿ�ηָ����߶������ٶ�
		NDistributionColor^	m_top_color;				// ����������ɫ
		NDistributionColor^	m_bottom_color;				// �ײ�������ɫ
	};
};
