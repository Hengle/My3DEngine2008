#pragma once
#include "NMeshComponent.h"
#include "NResourceSkeletalMesh.h"

namespace NexusEngine
{
	ref class NSkeletalAnimPlayer;

	public ref class NSkeletalMeshComponent : public NMeshComponent
	{
	public:		
		//!	ָ����������Դ
		void ResetResource(NResourceSkeletalMesh^ resMesh, bool cpuSkin);

		//!	ָ����������Դ, �ڲ�ִ��Load
		void ResetResource(NResourceLoc resMeshLoc, bool cpuSkin);

		//! ����AnimControllerΪclass nskel_anim_play
		NSkeletalAnimPlayer^ ResetSkeletalAnimPlayer();

		//-- Native Wrapper
	public:
		NSkeletalMeshComponent(nexus::nskeletal_mesh_component::ptr nativeComp);
	private:
		property nskeletal_mesh_component* NativePtr
		{
			nskeletal_mesh_component* get();
		}

	};
}