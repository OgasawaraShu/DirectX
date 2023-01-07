cbuffer cbuff0 : register(b0)
{
	float4 color;
	matrix mat;
	float Time;
	float Bar;
	bool Post;
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};


