#pragma once
#include "math/Vector3.h"
#include "math/Ray.h"

namespace NexusEngine
{
	public ref class NCamera
	{
	public:
		NCamera(void);
		~NCamera(void);

		void SetLookat(Vector3 eye, Vector3 at, Vector3 up);
		void SetPerspective(float FOV, int viewportW, int viewportH, float zNear, float zFar);
		void SetOrtho(float w, float h, float zNear, float zFar);

		Vector3 ScreenToWorld(int screenX, int screenY);
		System::Drawing::Point WorldToScreen(Vector3 p);
		Ray GetMouseRay(int cursorX, int cursorY);

		property Vector3 Forward
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->get_view_z());
			}
		}


		property nexus::ncamera* NativePtr
		{
			nexus::ncamera* get();
		}
	private:
		nexus::ncamera*	m_nativeObj;
	};
}//namespace NexusEngine