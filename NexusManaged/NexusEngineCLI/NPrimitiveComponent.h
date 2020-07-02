/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NActorComponent.h"
#include "math/BoundingBox.h"

namespace NexusEngine
{
	/**
	 *	��������(����Ϊ����Ⱦ��)����Ļ���
	*/
	public ref class NPrimitiveComponent : public NActorComponent
	{
	public:
		NPrimitiveComponent(nexus::nprimitive_component::ptr nativeComp);

		BoundingBox GetBoundingBox();

	protected:
		property nprimitive_component* NativePtr
		{
			nprimitive_component* get();
		}
	};
}//namespace NexusEngine