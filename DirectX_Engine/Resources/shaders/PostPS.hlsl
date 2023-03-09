#include "Post.hlsli"




Texture2D<float4> tex0: register(t0);//0番目に設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);//1番目に設定されたテクスチャ

SamplerState smp : register(s0);//0番目に設定されたサンプラー

float WhiteNoise(float2 coord) {
	return frac(sin(dot(coord, float2(8.7819, 3.255))) * 437.645);
}





float4 main(VSOutput input) : SV_TARGET
{

	//
	if (Post == true)
	{
	float density = 200;
	float2 samplePoint = input.uv;
	float4 colortex0 = tex0.Sample(smp, floor(input.uv * density) / density);
	float4 Tex1 = tex1.Sample(smp, input.uv);
	float vignette = length(float2(0.6, 0.6) - input.uv);
	vignette = clamp(vignette - 0.2, 0, 1);
	colortex0.rgb -= vignette;
	return colortex0;
	}
	else
	{
		float4 texcolor = tex0.Sample(smp,input.uv);
		return float4(texcolor.rgb, 1);
 }






}

