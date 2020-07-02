/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NPrimitiveComponent.h"
#include "NResource.h"

namespace NexusEngine
{
	/**
	 *	SpeedTree���, ����һ��Speed Tree Instance, ��װnexus::nspeed_tree_component�����
	*/
	public ref class NSpeedTreeComponent : public NPrimitiveComponent
	{
	public:
		NSpeedTreeComponent(nexus::nspeed_tree_component::ptr nativeComp);

		//!	�����ڲ���Speed Tree Instance
		void Create(NResourceLoc loc);

	private:
		property nspeed_tree_component* NativePtr
		{
			nspeed_tree_component* get();
		}
	};
}//namespace NexusEngine