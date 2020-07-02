/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NMaterialBase.h"
#include "NMaterialTemplate.h"
#include "MaterialParam.h"

namespace NexusEngine
{
	/**
	 *	ͨ�ò�����, ��װnexus::nmaterial�����
	*/
	public ref class NMaterial : public NMaterialBase
	{	
	public:
		NMaterial(String^ name);

		//! ʹ��ָ���Ĳ���ģ�崴��
		void Create(NMaterialTemplate^ temp);

		//! Loadָ���Ĳ���ģ��, ����������
		void Create(NResourceLoc tempLoc);

		//!	ȡ�ò��ʲ����ĸ���
		int GetNumParameter()	{	return NativePtr->get_num_parameter();}

		//!	ȡ��ĳ�����ʲ�������
		NMaterialParam GetParameter(int i);

		//!	���ò��ʲ�������, �ڲ�ʹ��"��������"������
		void SetParameter(NMaterialParam param);

		//-- Native Wrapper
	public:
		NMaterial(nexus::nmaterial::ptr nativeMtl);
		nexus::nmaterial::ptr GetSmartPtr();
	protected:
		property nmaterial* NativePtr
		{
			nmaterial* get() new;
		}
	};
}//namespace NexusEngine