/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once

namespace NexusEngine
{
	/**
	 *	�ṩһ�������쳣�Ļ���
	*/
	public ref class NexusException : public System::Exception
	{
	public:
		NexusException(System::String^ info);
	};
}//namespace NexusEngine