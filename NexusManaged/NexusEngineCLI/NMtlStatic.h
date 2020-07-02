#pragma once
#include "nmtlcommon.h"
#include "NMath.h"
#include "MaterialParam.h"

namespace NexusEngine
{
	public ref class NMtlStatic : public NMtlCommon
	{
	public:
		NMtlStatic(String^ nameStr);
		virtual ~NMtlStatic(void);

		void SetFloatParameter(String^ paramName, Vector4 val);

		//! ����Ĭ�ϵ�tech��Ȼ�󴴽�һ��Tech���󶨵�Lighting��Unlit��Ⱦ����
		void DefaultCreate(NResourceLoc shaderLoc);

		//!	ȡ�ò��ʲ����ĸ���
		int GetNumParameter();

		//!	ȡ��ĳ�����ʲ�������
		NMaterialParam GetParameter(int i);

		//!	���ò��ʲ�������, �ڲ�ʹ��"��������"������
		void SetParameter(NMaterialParam param);

		//-- Native Wrapper
	public:
		NMtlStatic(nexus::nmtl_static::ptr nativeMtl);		
	
	protected:		
		property nmtl_static* NativePtr
		{
			nmtl_static* get();
		}
	};
}//namespace NexusEngine