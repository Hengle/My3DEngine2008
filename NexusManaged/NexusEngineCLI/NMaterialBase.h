/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NResource.h"

namespace NexusEngine
{
	/**
	 *	������ʻ���, ��װnexus::nmaterial_base�����
	*/
	public ref class NMaterialBase : public NativeObjectOwnedBySharedPtr
	{
	public:
		NMaterialBase(String^ name)	{}

		//!	ȡ�ò��ʵ�����
		String^ GetName();

		//!	ȡ�ò�����ʹ�õ�ģ���Resource Location
		NResourceLoc GetTemplateLoc();

		//-- Native Wrapper
	protected:
		NMaterialBase(nexus::nmaterial_base::ptr nativeMtl);		
		property nmaterial_base* NativePtr
		{
			nmaterial_base* get();
		}

	public:
		static NMaterialBase^ FromNativePtr(nexus::nmaterial_base::ptr nativeMtl);
		nexus::nmaterial_base::ptr GetSmartPtr();	
	};
}//namespace NexusEngine