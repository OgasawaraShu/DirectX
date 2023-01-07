cbuffer cbuff0 : register(b0)
{
	bool Post;
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};