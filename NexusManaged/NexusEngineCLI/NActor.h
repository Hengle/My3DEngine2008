/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NRenderElement.h"

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

		//! ȡ��Actor������, Actor NameҪ����ĳ��Level����Ψһ��
		[System::ComponentModel::CategoryAttribute("Actor")]
		property System::String^ Name
		{
			System::String^ get()
			{
				return gcnew System::String(NativePtr->get_name().name_str.c_str());
			}
			void set(System::String^ val);
		}

		//! ȡ��Actor�ж��ٸ����
		[System::ComponentModel::CategoryAttribute("Actor")]
		property int NumComponents
		{
			int get()
			{
				return m_compList->Count;
			}
		}

		//!	Actor�Ƿ���Դ���
		[System::ComponentModel::CategoryAttribute("Level Design")]
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
		[System::ComponentModel::CategoryAttribute("Actor")]
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
		[System::ComponentModel::CategoryAttribute("Level Design")]
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

		//!	�õ�ĳ���������
		NActorComponent^ GetComponent(int i)
		{
			return m_compList[i];
		}

		//!	Actor��λ��
		[System::ComponentModel::CategoryAttribute("Actor")]
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
				NativePtr->move(*nval, os.rotation, os.scale);
			}
		}

		//!	Actor����ת,��λ: �Ƕ�
		[System::ComponentModel::CategoryAttribute("Actor")]
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

		//!	Actor������
		[System::ComponentModel::CategoryAttribute("Actor")]
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
				NativePtr->move(os.location, os.rotation, *nval);
			}
		}

		//!	ʹ��PDI�ӿڻ���Actor��Bounding box
		void DrawBounds(NRenderPrimitiveDrawInterface^ PDI, Color4f color);

		void _SetOwner(NLevel^ lv)
		{
			m_owner = lv;
		}
	protected:
		//!	Native�������İ�װ���󼯺�
		System::Collections::Generic::List<NActorComponent^>^	m_compList;

		NLevel^ m_owner;

		//-- Native Wrapper 
	protected:		
		NActor(nexus::nactor::ptr nativeActor);		

	public:
		static NActor^ FromNativePtr(nexus::nactor::ptr nativeActor, System::String^ nativeClassName);
		void NativeActorLoaded();

		[System::ComponentModel::BrowsableAttribute(false)]
		property nactor* NativePtr
		{
			nactor* get();
		}
	};
}//namespace NexusEngine