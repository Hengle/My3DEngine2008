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
	 *	����ģ����Դ, ��װnexus::nmaterial_template
	*/
	public ref class NMaterialTemplate : public NResource
	{
	public:
		NMaterialTemplate(nexus::nmaterial_template::ptr nativeTemp);

		property nexus::nmaterial_template::ptr NativePtr
		{
			nexus::nmaterial_template::ptr get();
		}
	};
}//namespace NexusEngine