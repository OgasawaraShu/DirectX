#include "FBX.hlsli"

/**/
Texture2D<float4> tex:register(t0);

SamplerState smp:register(s0);


struct PSOutput
{
	float4 target0:SV_TARGET0;
	float4 target1:SV_TARGET1;

};



float4 main(VSOutput input):SV_TARGET
{
  PSOutput output;
  float4 texcolor = tex.Sample(smp,input.uv);

 // float2 offset = float2(0.6, 0);

//  float4 Outlinetexcolor = tex.Sample(smp, input.uv+offset);


  float3 light = normalize(float3(1, -1, 1));

  //ライト
  float diffuse = saturate(dot(-light, input.normal));
  float brightness = diffuse + 0.3f;
  float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);


  float4 blendcolor = shadecolor * texcolor;
 // float4 Outlineblendcolor = shadecolor * Outlinetexcolor;


  output.target0= shadecolor * texcolor;
  output.target1= float4(0.7*(shadecolor * texcolor).rgb,1);
 // output.target1= shadecolor*Outlinetexcolor;





  //return output.target0*output.target1;


  //トゥーンシェーダ
  //float3 ramp = tex2D(_RampTex, fixed2(diff, diff)).rgb;
  float intensity =
	  saturate(dot(normalize(input.normal),-light));
  
  float4 LightColor = output.target0;
  float4 DarkColor = output.target1;

  float  LightToon = step(0.3, intensity);
  float  DarkToon = 1 - step(0.3, intensity);

  //	return step(0.4, intensity) * LightColor+(1 - step(0.4, intensity)) * DarkColor;




// return LightToon * LightColor + DarkToon * DarkColor;
  return LightColor;
  //return shadecolor * texcolor;
}