#pragma once
#include "NResource.h"
#include "NMtlBase.h"


namespace NexusEngine
{
	public ref class NResourceSkeletalMesh : public NResource
	{
	public:
		//!	�Ӵ��̵������ģ��
		void ImportSkeletalMeshLOD(int lod, System::String^ fullPath);
		//!	����ĳ��LOD�Ĳ���
		void ImportSetMaterial(int lod, int mtl_id, NMtlBase^ mtl);
	
		//!	���ĳ��LOD��ĳ��Sectionʹ�õĲ���
		NMtlBase^ GetMaterial(int lod, int mtl_id);

		//!	���LOD����ĸ���
		int  GetNumLOD();

		//!	���ĳ��LOD Mesh�м���Section
		int GetNumSection(int lod);

		//!	���ĳ��LOD����Ϣ
		void GetLODInfo(int lod, [Out]int% numVert, [Out]int% numSec, [Out]int% numTri);

		property UInt32 LODCount
		{
			UInt32 get()
			{
				return NativePtr->get_num_lod();
			}
		}

		//-- Native Wrapper
	public:
		NResourceSkeletalMesh(nexus::nresource_skeletal_mesh::ptr nativeRes);
		nresource_skeletal_mesh::ptr GetSmartPtr();
	protected:
		property nresource_skeletal_mesh* NativePtr
		{
			nresource_skeletal_mesh* get();
		}
	};
}
