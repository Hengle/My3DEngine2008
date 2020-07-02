/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NPrimitiveComponent.h"
#include "NMaterial.h"

namespace NexusEngine
{
	/**
	 *	Mesh������Ļ���,��װnexus::nmesh_component����
	*/
	public ref class NMeshComponent : public NPrimitiveComponent
	{
	public:
		//!	ָ���������resource�����������Ĳ���
		void ReplaceMaterial(int lod, int mtlID, NMaterial^ mtl);

		//-- Native wrapper
	public:
		NMeshComponent(nexus::nmesh_component::ptr nativeComp);
	protected:
		property nmesh_component* NativePtr
		{
			nmesh_component* get();
		}		
	};
}//namespace NexusEngine