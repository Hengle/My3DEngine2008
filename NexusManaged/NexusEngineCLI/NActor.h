/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NRenderElement.h"
#include "NMath.h"

namespace NexusEngine
{
	ref class NActorComponent;
	ref class NLevel;

	/**
	 * nexus::nactor�İ�װ��
	*/
	public ref class NActor : public NativeObjectOwnedBySharedPtr
	{
	public:
		//! ��actor����һ�����, ���������Ҫ����Actor����Ψһ��
		NActorComponent^ CreateComponent(System::String^ compName, System::String^ nativeClassName);

		//! ɾ�����в���
		bool	RemoveAllComponents()
		{
			return NativePtr->remove_all_component();
		}

		//! �������в����ı任����
		void UpdateComponentsTransform()
		{
			NativePtr->update_cmp_transform();
		}

		//�ж��Ƿ���ָ��Actor������ͬ
		bool IsSameTypeOf(NActor^ actor);

		//! ȡ��Actor������, Actor NameҪ����ĳ��Level����Ψһ��
		[Category("Actor")]
		property System::String^ Name
		{
			System::String^ get()
			{
				return gcnew System::String(NativePtr->get_name().name_str.c_str());
			}
			void set(System::String^ val);
		}

		//! ȡ��Actor�ж��ٸ����
		[Category("Actor")]
		property int NumComponents
		{
			int get()
			{
				return m_compList->Count;
			}
		}

		//!	Actor�Ƿ���Դ���
		[Category("Level Design")]
		property bool Serializable
		{
			bool get()
			{
				return NativePtr->get_serializable();
			}
			void set(bool val)
			{
				NativePtr->set_serializable(val);
			}
		}

		//! Actor�ɼ���������
		[Category("Actor")]
		property bool Visible
		{
			bool get()
			{
				return NativePtr->get_visible();
			}
			void set(bool val)
			{
				NativePtr->set_visible(val);
			}
		}

		//! Actor����(�༭���޷��ƶ�)
		[Category("Level Design")]
		property bool Frozen
		{
			bool get()
			{
				return NativePtr->get_frozen();
			}
			void set(bool val)
			{
				NativePtr->set_frozen(val);
			}
		}

		[Category("Movement")]
		property bool Static
		{
			bool get()
			{
				return NativePtr->get_static();
			}
			void set(bool value)
			{
				NativePtr->set_static(value);
			}
		}

		[Category("Movement")]
		property bool Kinematic
		{
			bool get()
			{
				return NativePtr->is_kinematic();
			}
			void set(bool value)
			{
				NativePtr->set_kinematic(value);
			}
		}

		//!	�õ�ĳ���������
		NActorComponent^ GetComponent(int i)
		{
			return m_compList[i];
		}

		[Browsable(false)]
		property ObjectSpace SpaceData
		{
			ObjectSpace get();
			void set(ObjectSpace newSpace);
		}

		//!	Actor��λ��
		[Category("Actor")]
		property Vector3 Location
		{
			Vector3 get()
			{
				const nexus::vector3& nloc = NativePtr->get_space().location;
				return Vector3(nloc.x, nloc.y, nloc.z);
			}

			void set(Vector3 val)
			{
				const object_space& os = NativePtr->get_space();
				nexus::vector3* nval = reinterpret_cast<nexus::vector3*>(&val);
				if(os.location != *nval)
				{
					NativePtr->move(*nval, os.rotation, os.scale);
				}
			}
		}

		//!	Actor����ת,��λ: �Ƕ�
		[Category("Actor")]
		property Vector3 Rotation
		{
			Vector3 get()
			{
				const nexus::vector3& nrot = NativePtr->get_space().rotation;
				return Vector3(nRadToDeg(nrot.x), 
					nRadToDeg(nrot.y), 
					nRadToDeg(nrot.z)
					);
			}

			void set(Vector3 val)
			{
				const object_space& os = NativePtr->get_space();				
				NativePtr->move(os.location, 
					nexus::vector3(nDegToRad(val.x), nDegToRad(val.y), nDegToRad(val.z)), 
					os.scale);
			}
		}

		//!	Actor����ת,��λ: ����Ϊ��λ
		[Category("Actor")]
		property Vector3 RadianRotation
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->get_space().rotation);
			}

			void set(Vector3 val)
			{
				const object_space& os = NativePtr->get_space();
				vector3 nativeRotation = val.ToNative();
				if(nativeRotation != os.rotation)
				{
					NativePtr->move(os.location, nativeRotation, os.scale);
				}
			}
		}

		//!	Actor������
		[Category("Actor")]
		property Vector3 Scale
		{
			Vector3 get()
			{
				const nexus::vector3& ns = NativePtr->get_space().scale;
				return Vector3(ns.x, ns.y, ns.z);
			}

			void set(Vector3 val)
			{
				const object_space& os = NativePtr->get_space();
				nexus::vector3* nval = reinterpret_cast<nexus::vector3*>(&val);
				if(*nval != os.scale)
				{
					NativePtr->move(os.location, os.rotation, *nval);
				}
			}
		}

		[Category("Level Design")]
		property String^ Layer
		{
			String^ get()
			{
				return gcnew System::String(NativePtr->get_layer_name().c_str());
			}
			void set(String^ layer_name);
		}

		//!	ʹ��PDI�ӿڻ���Actor��Bounding box
		void DrawBounds(NRenderPrimitiveDrawInterface^ PDI, Color4f color);

		void _SetOwner(NLevel^ lv)
		{
			m_owner = lv;
		}

		void LookAt(Vector3 Position)
		{
			nexus::vector3* position = reinterpret_cast<nexus::vector3*>(&Position);
			NativePtr->look_at(*position);
		}

		[Browsable(false)]
		property bool Destroyed
		{
			bool get() {	return m_destroyed;}
			void set(bool val)	{	m_destroyed = val;}
		}
	protected:
		//!	Native�������İ�װ���󼯺�
		System::Collections::Generic::List<NActorComponent^>^	m_compList;

		NLevel^ m_owner;
		bool	m_destroyed;

		//-- Native Wrapper 
	protected:
		//������ȱʡ�Ĺ��캯����֤�����Լ�����
		NActor(){};		
		NActor(nexus::nactor::ptr nativeActor);		

	public:
		//clone�ӿڵ������ǿ�����ÿ�����͵�NActor���ظýӿ��Ժ����ֱ�ӷ����������͵�NActor�������Ͳ�����FromNativePtr�и����������ж���
		virtual NActor^	Clone(nexus::nactor::ptr nativeActor);
		static NActor^ FromNativePtr(nexus::nactor::ptr nativeActor, System::String^ nativeClassName);
		void NativeActorLoaded();

		[Browsable(false)]
		property nactor* NativePtr
		{
			nactor* get();
		}

		[Browsable(false)]
		property nactor::ptr SharedPtr
		{
			nactor::ptr get();
		}
	};
}//namespace NexusEngine