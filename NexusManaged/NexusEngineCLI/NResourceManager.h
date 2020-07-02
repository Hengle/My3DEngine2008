/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NResourceStaticMesh.h"
#include "NResourceAnimMesh.h"
#include "NMaterialTemplate.h"
#include "ResourceIODef.h"
#include "NResourceStaticMesh.h"

namespace NexusEngine
{
	/**
	 * ��Դ������, ��װnexus::nresource_manager
	*/
	public ref class NResourceManager
	{
	private:
		static NResourceManager^ s_instance;
		NResourceManager(void);
		
	public:
		static property NResourceManager^ Instance
		{
			NResourceManager^ get()
			{
				return s_instance;
			}
		}

		static NResourceManager(void)
		{
			s_instance = gcnew NResourceManager;
		}

	public:
		//!	����һ���հ׵ľ�̬ģ����Դ
		NResourceStaticMesh^ NewStaticMesh(System::String^ resName);
		NResourceAnimMesh^ NewAnimMesh(System::String^ resName);

		//!	���ļ�ϵͳ���ز���ģ��ű�
		NMaterialTemplate^ LoadMaterialScript(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);

		//!	���ļ�ϵͳ���ؾ�̬ģ����Դ
		NResourceStaticMesh^ LoadStaticMesh(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);

		NResourceAnimMesh^ LoadAnimMesh(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);
	};
}//namespace NexusEngine