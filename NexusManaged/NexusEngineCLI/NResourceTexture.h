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
}//namespace NexusEngine