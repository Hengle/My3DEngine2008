/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NexusException.h"

namespace NexusEngine
{
	/**
	 * ����Native���뷢�����쳣
	*/
	public ref class NexusNativeException : public NexusException
	{
	public:
		NexusNativeException(System::String^ info);
		NexusNativeException(const TCHAR* szInfo);
	};

#define BEGIN_NATIVE_GUARD try{
#define END_NATIVE_GUARD } catch(nexus::nexception& ne)\
	{throw gcnew NexusNativeException(ne.what().c_str());}
}//namespace NexusEngine