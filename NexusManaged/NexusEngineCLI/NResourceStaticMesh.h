/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NResource.h"
#include "NMaterialBase.h"

namespace NexusEngine
{
	/**
	 *	��̬ģ����Դ
	*/
	public ref class NResourceStaticMesh : public NResource
	{
	public:	
		//!	�Ӵ��̵���һ��ģ��,��Ϊĳ��LOD
		void ImportLOD(int lod, System::String^ fullPath);

		//!	����ĳ��LOD�Ĳ���
		void ImportSetMaterial(int lod, int sec, NMaterialBase^ mtl);

		//!	���LOD����ĸ���
		int  GetNumLOD();

		//!	���ĳ��LOD��mesh��Scetion����
		int  GetNumSection(int lod);

		//!	���ĳ��LOD����Ϣ
		void GetLODInfo(int lod, [Out]int% numVert, [Out]int% numSec, [Out]int% numTri);

		//!	���ĳ��LOD��ĳ��Sectionʹ�õĲ���
		NMaterialBase^ GetMaterial(int lod, int sec);

		//-- Native Wrapper
	public:
		NResourceStaticMesh(nresource_static_mesh::ptr nativeRes);
		nresource_static_mesh::ptr GetSmartPtr();
	protected:
		property nresource_static_mesh* NativePtr
		{
			nresource_static_mesh* get();
		}
	};
}//namespace NexusEngine