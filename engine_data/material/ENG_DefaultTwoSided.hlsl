#define NEXUS_MATERIAL	// ʹ�ô˺�֪ͨinclude��material.hlsl���ļ�

#define MTL_TWO_SIDED	// ��pre pass��Ⱦ�У����������˫�����ʹ�����

#define MTL_LIGHT 0	// �����Ƿ���ܹ���
#define MTL_NUM_TEXCOORD 0

struct mtlParameters
{
	float dummy : TEXCOORD0;
};


#include "mtl_const.hlsl"