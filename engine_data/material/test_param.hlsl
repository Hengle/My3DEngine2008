#define NEXUS_MATERIAL	// ʹ�ô˺�֪ͨinclude��material.hlsl���ļ�

#define MTL_LIGHT 0	// �����Ƿ���ܹ���
#define MTL_NUM_TEXCOORD 0

float g_f1:MTL_MyFloat;
float2 g_f2:MTL_MyVector2;
float3 g_f3:MTL_MyVector3;
float4 g_f4:MTL_MyVector4;
texture g_tex:MTL_MyTexture;

struct mtlParameters
{
	float dummy : TEXCOORD0;
};

#define MTL_HAVE_DIFFUSE
float4 mtlGetDiffuseColor(mtlParameters mtlParam)
{
	return g_f4;
}

#include "mtl_const.hlsl"