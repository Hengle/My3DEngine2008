#pragma once

namespace NexusEngine
{
	/**
	* ��native engineȫ�ַ����ķ�װ
	*/
	public ref class NativeEngineHelper
	{
	public:
		static float GetTimeSeconds();
	
		//! ����ָ���ַ�����CRCֵ
		static UInt32 CheckCRC(System::String^ nameStr);
	};
}
