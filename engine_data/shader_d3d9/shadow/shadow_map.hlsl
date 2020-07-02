
//-- shadow map
float4x4 g_shadowMat:SHADOW_Mat;
texture	 g_shadowBuffer:SHADOW_Buffer;

sampler shadowSampler = sampler_state
			{
				Texture = <g_shadowBuffer>;
				MipFilter = None;
				MinFilter = LINEAR;
				MagFilter = LINEAR;
				AddressU  = CLAMP;
				AddressV  = CLAMP;
			};

// ����Shadow Map��UV����
float4	shadowVS(float3 vertPos)
{
	float4 shadowUV = mul(float4(vertPos,1),g_shadowMat);
	return shadowUV;
}

// ����ShadowMap
float4 shadowPS(float4 shadowUV)
{
	float4 shadow = tex2Dproj(shadowSampler, shadowUV);
	return shadow;
}