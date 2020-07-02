/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NResource.h"
#include "NMath.h"

namespace NexusEngine
{
	public enum class EMaterialParameter
	{
		None	= EMPT_None,
		Float	= EMPT_Float,
		Float2	= EMPT_Float2,
		Float3	= EMPT_Float3,
		Float4	= EMPT_Float4,
		Texture	= EMPT_Texture,
		Int		= EMPT_Int,
	};

	//!	���ʲ���
	public value struct NMaterialParam
	{
		String^ name;					// ��������
		EMaterialParameter type;		// ��������, �������Ͳ�ͬ, �����ֵ�ֱ���Ч

		int				 intVal;
		Vector4			 floatVal;
		NResourceLoc	 textureLoc;

		void FromNative(const nexus::material_parameter_value& np);
	};
}//namespace NexusEngine