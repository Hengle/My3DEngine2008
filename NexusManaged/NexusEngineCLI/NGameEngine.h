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

		//!	ִ��ÿ֡��Update��Render
		virtual void FrameTick(float deltaTime, NViewport^ vp);

	protected:
		property ngame_engine* NativePtr
		{
			ngame_engine* get();
		}
	};
}//namespace NexusEngine