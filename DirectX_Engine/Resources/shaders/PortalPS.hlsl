#include "Portal.hlsli"

/**/
Texture2D<float4> tex:register(t0);

SamplerState smp:register(s0);


struct PSOutput
{
	float4 target0:SV_TARGET0;
	float4 target1:SV_TARGET1;

};



//
float wave(float4 uv, float2 emitter, float speed, float phase) {
	float dst = distance(uv, emitter);
	return pow((0.5 + 0.5 * sin(dst * phase - scale * speed)), 2.0);
}


void Twirl(float2 UV, float2 Center, float Strength, float2 Offset, out float2 Out)
{
	float2 delta = UV - Center;
	float angle = Strength * length(delta);
	float x = cos(angle) * delta.x - sin(angle) * delta.y;
	float y = sin(angle) * delta.x + cos(angle) * delta.y;
	Out = float2(x + Center.x + Offset.x, y + Center.y + Offset.y);
}



float4 main(VSOutput input):SV_TARGET
{
  PSOutput output;


if (Flag == false)
{
	// テクスチャがリピートじゃないなら *0.5+0.5 とかする
	input.uv = normalize(input.uv - float2(0.5, 0.375));
	// 時間でアニメーション
	input.uv += scale;
}

  float4 texcolor = tex.Sample(smp,input.uv);
  float3 light = normalize(float3(1, -1, 1));
  float diffuse = saturate(dot(-light, input.normal));
  float brightness = diffuse + 0.3f;
  float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
  output.target0= texcolor;
  float4 LightColor = output.target0;
//周りを暗く
  float vignette = length(float2(0.5, 0.5) - input.uv);
vignette = clamp(vignette - 0.2, 0, 1);

if (Color == 1)
{
	LightColor.r -= vignette;
	LightColor.g -= vignette;
}
else
{
	LightColor.b -= vignette;
	LightColor.g -= vignette*0.35;
}
  return LightColor;
}