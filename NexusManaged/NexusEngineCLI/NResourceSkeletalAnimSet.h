#pragma once
#include "NResource.h"
#include "NMtlBase.h"


namespace NexusEngine
{
	ref class NSkeletalAnimSequence;

	public ref class NResourceSkeletalAnimSet : public NResource
	{
	public:
		//!	�Ӵ��̵����������
		void ImportSkeletalAnimSequence(System::String^ fullPath);

		//! �õ�����֡������
		String^ GetSequenceName(UInt32 index);

		NSkeletalAnimSequence^ FindSequence(UInt32 index);
		NSkeletalAnimSequence^ FindSequence(String^ sequenceName);

		// �ܼ��ж��ٶ�������
		property UInt32 SequenceCount
		{
			UInt32 get()
			{
				return NativePtr->get_num_anim();
			}
		}

		int GetBoneCount();
		String^ GetBoneName(int i);
		String^ GetBoneParentName(int i);

		//-- Native Wrapper
	public:
		NResourceSkeletalAnimSet(nexus::nresource_skeletal_anim_set::ptr nativeRes);
		nresource_skeletal_anim_set::ptr GetSmartPtr();
	protected:
		property nresource_skeletal_anim_set* NativePtr
		{
			nresource_skeletal_anim_set* get();
		}
	};
}
