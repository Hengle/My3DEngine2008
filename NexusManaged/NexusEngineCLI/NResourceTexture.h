/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NResource.h"

namespace NexusEngine
{
	/**
	*	��ͼ��Դ����, ��װnexus::nresource_texture
	*/
	public ref class NResourceTexture : public NResource
	{
	public:
		NResourceTexture(nresource_texture::ptr nativeRes);
	};

	/**
	* 2D��ͼ��Դ�� ��װnexus::nresource_texture_2d
	*/
	public ref class NResourceTexture2D : public NResourceTexture
	{
	public:
		explicit NResourceTexture2D(nexus::nresource_texture_2d::ptr nativeObj );

		virtual void LoadFromFile(NResourceLoc loc);

		Vector2 GetOriginalSize();

		property nexus::nrender_texture2D* Texture2D
		{
			nexus::nrender_texture2D* get();
		}
	};
}//namespace NexusEngine