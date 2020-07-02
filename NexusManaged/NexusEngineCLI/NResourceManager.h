/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NResourceStaticMesh.h"
#include "NResourceAnimMesh.h"
#include "NMtlTechShader.h"
#include "ResourceIODef.h"
#include "NResourceStaticMesh.h"
#include "NResourceTexture.h"
#include "NResourceTextureAtlas.h"
#include "NResourceSpecialEffect.h"
#include "NResourceSkeletalAnimSet.h"
#include "NResourceSkeletalMesh.h"

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
		NResourceSpecialEffect^ NewSpecialEffect(System::String^ resName);

		NResourceSkeletalAnimSet^ NewSkeletalAnimSet(System::String^ resName);
		NResourceSkeletalMesh^ NewSkeletalMesh(System::String^ resName);

		//!	���ļ�ϵͳ���ز���ģ��ű�
		NMtlTechShader^ LoadMtlShader(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);

		//!	���ļ�ϵͳ���ؾ�̬ģ����Դ
		NResourceStaticMesh^ LoadStaticMesh(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);

		NResourceAnimMesh^ LoadAnimMesh(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);
		
		NResourceSkeletalAnimSet^ LoadSkeletalAnimSet(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);
		NResourceSkeletalMesh^ LoadSkeletalMesh(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);

		//! ����һ��2D����
		NResourceTexture2D^ LoadResourceTexture2D(NResourceLoc loc, EResourceIOMode mode,EResourceIOPriority priority);
	

		NResourceTextureAtlas^ LoadTextureAtlas(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);
	};
}//namespace NexusEngine