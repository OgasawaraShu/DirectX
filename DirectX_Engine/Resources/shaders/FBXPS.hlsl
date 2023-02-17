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
 //テクスチャマッピング
  float4 texcolor = tex.Sample(smp,input.uv);
  // 光沢度
  const float shininess = 4.0f;
  // 頂点から視点への方向ベクトル
  float3 eyedir = normalize(cameraPos - input.worldpos.xyz);


  float3 light = normalize(float3(1, -1, 1));

  float brightness=0.0f;
  //ライト
  //平行
  for (int i = 0; i < DIRLIGHT_NUM; i++) {
	  if (dirLights[i].active) {
		  float dotlightnormal = saturate(dot(dirLights[i].lightv, input.normal));
		  float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
		  float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
		  float3 diffuse = diffuse * m_diffuse;
		  brightness += (dotlightnormal + 0.1f) * dirLights[i].lightcolor;//0.1fはdiffuse
	  }
  }

  // 点光源
  for (i = 0; i < POINTLIGHT_NUM; i++) {
	  if (pointLights[i].active) {
		  // ライトへの方向ベクトル
		  float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
		  float d = length(lightv);
		  lightv = normalize(lightv);

		  // 距離減衰係数
		  float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);

		  // ライトに向かうベクトルと法線の内積
		  float3 dotlightnormal = dot(lightv, input.normal);
		  // 反射光ベクトル
		  float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
		  // 拡散反射光
		  float3 diffuse = dotlightnormal * m_diffuse;
		  // 鏡面反射光
		  float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

		  // 全て加算する
		  //shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;

		  brightness += atten*(dotlightnormal + 0.1f) * pointLights[i].lightcolor;//0.1fはdiffuse
	  }
  }

  // スポットライト
  for (i = 0; i < SPOTLIGHT_NUM; i++) {
	  if (spotLights[i].active) {
		  // ライトへの方向ベクトル
		  float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
		  float d = length(lightv);
		  lightv = normalize(lightv);

		  // 距離減衰係数
		  float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));

		  // 角度減衰
		  float cos = dot(lightv, spotLights[i].lightv);
		  // 減衰開始角度から、減衰終了角度にかけて減衰
		  // 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
		  float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
		  // 角度減衰を乗算
		  atten *= angleatten;

		  // ライトに向かうベクトルと法線の内積
		  float3 dotlightnormal = dot(lightv, input.normal);
		  // 反射光ベクトル
		  float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
		  // 拡散反射光
		  float3 diffuse = dotlightnormal * m_diffuse;
		  // 鏡面反射光
		  float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

		  // 全て加算する
		  //shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
		  brightness += atten*(dotlightnormal + 0.1f) * spotLights[i].lightcolor;//0.1fはdiffuse
	  }
  }

  float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);

  output.target0= shadecolor * texcolor;
 
  return  output.target0;
}