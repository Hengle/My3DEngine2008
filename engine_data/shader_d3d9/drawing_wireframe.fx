#include "vertex_factory.hlsl"


void vsMain(vfInput vert,	
			out float4 outPos : POSITION
			)
{
	outPos = vfTransformPos(vert);
}

float4 psMain() : COLOR
{
	return float4(1,1,0.9,1);
}

technique techDefault
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = False;
		ZFunc = LESSEQUAL;
		DepthBias = -0.0002;	// �������Ϊ��������������ߵ�Ч��
		SlopeScaleDepthBias = 1.9;
		
		FillMode = WireFrame;
		CullMode = None;
		ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}