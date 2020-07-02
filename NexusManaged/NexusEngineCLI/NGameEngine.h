/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NEngine.h"
#include "NViewport.h"

namespace NexusEngine
{
	//! ��Ϸ����ʱ��ʹ�õ�������
	public ref class NGameEngine : public NEngine
	{
	public:
		NGameEngine(void);

		//!	ִ��ÿ֡��Update
		virtual void Update(float deltaTime, NViewport^ vp);
		//!	ִ��ÿ֡��Render
		virtual void Render(NViewport^ vp);

		//!	���ʰȡ�ؿ��е�Actor
		NActor^ PickLevelActor(NViewport^ vp, int mouseX, int mouseY);

		virtual void ScreenShot(System::String^ fileName, UINT32 width, UINT32 height);
		virtual void ViewportShot(System::String^ fileName);

	protected:
		property ngame_engine* NativePtr
		{
			ngame_engine* get();
		}
	};
}//namespace NexusEngine