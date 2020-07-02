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

	public enum class ETextureType
	{
		Texture2D = 0,
		CubeMap = 1,
	};

	//!	���ʲ���
	public value struct NMaterialParam
	{
		String^ name;					// ��������
		EMaterialParameter type;		// ��������, �������Ͳ�ͬ, �����ֵ�ֱ���Ч

		int				 intVal;
		Vector4			 floatVal;
		NResourceLoc	 textureLoc;
		ETextureType textureType;

		void FromNative(const nexus::material_parameter& np);
	};
}//namespace NexusEngine