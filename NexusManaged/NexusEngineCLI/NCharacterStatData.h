#pragma once
#include "stat/nobj_data.h"

namespace NexusEngine
{
	// gameframework::nobj_data Interop��װ
	// ʹ����Ҫע��:
	// 1.�������ʵ���ʵ�ʳ�ʼ���ʵ��Ĵ�СInit
	// 2.�������ݳ�Ա��ʹ��Get*()���� ͬʱע����������
	// 3.��������һ��ʹ��OnRecv*()����
	public ref class NCharacterStatData
	{
	public:
		//-- �¼�����
		event EventHandler<EventArgs^>^ StatInited;
		event EventHandler<EventArgs^>^ StatUpdated;

		NCharacterStatData(void);
		~NCharacterStatData(void);

		bool Init(UInt16 valueCount);

		void OnRecvInitStat(IntPtr data);
		void OnRecvStatUpdate(IntPtr data);
		void OnRecvRemoteStatUpdate(IntPtr data);

		Int32 GetInt32Value(UInt16 index);
		UInt32 GetUInt32Value(UInt16 index);
		UInt64 GetUInt64Value(UInt16 index);
		float GetFloatValue(UInt16 index);
		Byte GetByteValue(UInt16 index, Byte offest);
		UInt16 GetInt16Value(UInt16 index, Byte offest);

		bool HasFlag(UInt16 index, UInt32 flag);

		property nobj_data* NativePtr
		{
			nobj_data* get()
			{
				return m_nativeData;
			}
		}

	private:
		nobj_data* m_nativeData;

	};
}
