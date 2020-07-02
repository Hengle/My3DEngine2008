#pragma once
#include "NativeObject.h"

namespace NexusEngine
{
	ref class NRenderFont;
	ref class NUICanvas;

	/**
	* ��װNative class nexus::nrender_resource_manager
	*/
	public ref class NRenderResourceManager
	{
	public:	
		// ����һ���µ�����
		NRenderFont^ NewRenderFont();

		// ����һ���µ�UI Canvas
		NUICanvas^ NewUICanvas();

	public:
		explicit NRenderResourceManager(INativeObj^ nativeObj);

		property nrender_resource_manager* NativePtr
		{
			nrender_resource_manager* get();
		}

	private:
		INativeObj^		m_nativeObj;
	};

} // end of namespace NexusEngine
