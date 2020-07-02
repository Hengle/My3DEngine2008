/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"

namespace NexusEngine
{
	ref class NActor;

	/**
	 *	Actor�������, ��װnexus::nactor_componet����ӿ�
	*/
	public ref class NActorComponent : public NativeObjectOwnedBySharedPtr
	{
	public:
		//!	�ڲ��ӿ� : ����������actor
		void _SetOwner(NActor^ owner);

		//!	ȡ�������ĸ�Actor�Ե�
		NActor^ GetOwner();

		//!	ȡ���������
		[System::ComponentModel::CategoryAttribute("ActorComponent")]
		property System::String^ Name
		{
			System::String^ get();
		}

		[System::ComponentModel::CategoryAttribute("ActorComponent")]
		property System::String^ NativeClassName
		{
			System::String^ get();
		}

	protected:
		System::WeakReference^	m_owner;

		//-- Native Wrapper
	protected:
		NActorComponent(nexus::nactor_component::ptr nativeComp);
		
		property nactor_component* NativePtr
		{
			nactor_component* get();
		}
	public:
		static NActorComponent^ FromNativePtr(nexus::nactor_component::ptr nativeComp, System::String^ nativeClassName);
	};
}//namespace NexusEngine