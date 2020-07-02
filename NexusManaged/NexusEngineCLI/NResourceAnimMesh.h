#pragma once
#include "NResource.h"
#include "NMtlBase.h"

namespace NexusEngine
{
	public ref class NResourceAnimMesh : public NResource
	{
	public:	
		//!	�Ӵ��̵���һ��ģ��,��Ϊĳ��LOD
		void ImportLOD(int lod, System::String^ fullPath);

		//!	����ĳ��LOD�Ĳ���
		void ImportSetMaterial(int lod, int sec, NMtlBase^ mtl);

		//!	���LOD����ĸ���
		int  GetNumLOD();

		//!	���ĳ��LOD��mesh��Scetion����
		int  GetNumSection(int lod);

		//!	���ĳ��LOD����Ϣ
		void GetLODInfo(int lod, [Out]int% numVert, [Out]int% numSec, [Out]int% numTri);

		//!	���ĳ��LOD��ĳ��Sectionʹ�õĲ���
		NMtlBase^ GetMaterial(int lod, int sec);

		int GetNumSequence();
		String^ GetSequenceInfo(int i, [Out]float% length);

		//-- Native Wrapper
	public:
		NResourceAnimMesh(nexus::nresource_anim_mesh::ptr nativeRes);
		nresource_anim_mesh::ptr GetSmartPtr();
	protected:
		property nresource_anim_mesh* NativePtr
		{
			nresource_anim_mesh* get();
		}
	};
}//namespace NexusEngine