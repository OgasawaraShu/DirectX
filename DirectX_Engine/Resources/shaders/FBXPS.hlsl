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
 //�e�N�X�`���}�b�s���O
  float4 texcolor = tex.Sample(smp,input.uv);
  // ����x
  const float shininess = 4.0f;
  // ���_���王�_�ւ̕����x�N�g��
  float3 eyedir = normalize(cameraPos - input.worldpos.xyz);


  float3 light = normalize(float3(1, -1, 1));

  float brightness=0.0f;
  //���C�g
  //���s
  for (int i = 0; i < DIRLIGHT_NUM; i++) {
	  if (dirLights[i].active) {
		  float dotlightnormal = saturate(dot(dirLights[i].lightv, input.normal));
		  float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
		  float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
		  float3 diffuse = diffuse * m_diffuse;
		  brightness += (dotlightnormal + 0.1f) * dirLights[i].lightcolor;//0.1f��diffuse
	  }
  }

  // �_����
  for (i = 0; i < POINTLIGHT_NUM; i++) {
	  if (pointLights[i].active) {
		  // ���C�g�ւ̕����x�N�g��
		  float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
		  float d = length(lightv);
		  lightv = normalize(lightv);

		  // ���������W��
		  float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);

		  // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
		  float3 dotlightnormal = dot(lightv, input.normal);
		  // ���ˌ��x�N�g��
		  float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
		  // �g�U���ˌ�
		  float3 diffuse = dotlightnormal * m_diffuse;
		  // ���ʔ��ˌ�
		  float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

		  // �S�ĉ��Z����
		  //shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;

		  brightness += atten*(dotlightnormal + 0.1f) * pointLights[i].lightcolor;//0.1f��diffuse
	  }
  }

  // �X�|�b�g���C�g
  for (i = 0; i < SPOTLIGHT_NUM; i++) {
	  if (spotLights[i].active) {
		  // ���C�g�ւ̕����x�N�g��
		  float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
		  float d = length(lightv);
		  lightv = normalize(lightv);

		  // ���������W��
		  float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));

		  // �p�x����
		  float cos = dot(lightv, spotLights[i].lightv);
		  // �����J�n�p�x����A�����I���p�x�ɂ����Č���
		  // �����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
		  float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
		  // �p�x��������Z
		  atten *= angleatten;

		  // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
		  float3 dotlightnormal = dot(lightv, input.normal);
		  // ���ˌ��x�N�g��
		  float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
		  // �g�U���ˌ�
		  float3 diffuse = dotlightnormal * m_diffuse;
		  // ���ʔ��ˌ�
		  float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

		  // �S�ĉ��Z����
		  //shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
		  brightness += atten*(dotlightnormal + 0.1f) * spotLights[i].lightcolor;//0.1f��diffuse
	  }
  }

  float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);

  output.target0= shadecolor * texcolor;
 
  return  output.target0;
}