#include "Physics.h"


template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;
using XMVECTOR = DirectX::XMVECTOR;

void Physics::Pendulum(float x, float y)
{

	// 公式に従って速度を加算
	// MASSの値を小さくするとゆっくり動く
	speed += -MASS * G * sin(x1 / LENGTH);

	// 速度に従って円上の座標を変更
	x1 += speed;

	// 軸を原点とした場合のぶら下がっている物の座標を算出
	// このままだと－45～45度の振り子になるので
	// 時計回りに90度（PI/2.0）回転
	angle = x1 / LENGTH + PI / 2.0;

	// 求めた角度から軸を原点とした円周上の座標を取得
	x = cos(angle) * LENGTH;
	y = sin(angle) * LENGTH;
}

float Physics::Gravity(float x, float y)
{
	//重力をつけてy軸で落下
	g1 += acceleration_g;
	y -= g1;

	return y;
}

void Physics::Jump(float y, float YGround)
{

	JumpVel -= acceleration_g;
	y += JumpVel;
}

float Physics::Screw(float y, float g)
{
	float v = 0.0;

	//y軸を長さ　重力(落ちてるときにある力)をパワーとして投げる力を出力
	v = y * g;
	//vはｘもしくはｚ軸に飛ばす力
	return v;
}


XMFLOAT3 Physics::splinePosition(const std::vector<XMFLOAT3>& points, size_t startIndex, float t)
{
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n];
	if (startIndex < 1)return points[1];

	XMFLOAT3 p0 = points[startIndex - 1];
	XMFLOAT3 p1 = points[startIndex];
	XMFLOAT3 p2 = points[startIndex + 1];
	XMFLOAT3 p3 = points[startIndex + 2];

	XMFLOAT3 position;

	//X座標
	position.x= 0.5 * ((2.0f * p1.x) + ((-1 * p0.x) + p2.x) * t +
		((2 * p0.x) - (5 * p1.x) + (4 * p2.x) - p3.x) * t * t +
		((-1 * p0.x) + (3 * p1.x) - (3 * p2.x) + p3.x) * t * t * t);

	//Y座標
	position.y = 0.5 * ((2.0f * p1.y) + ((-1 * p0.y) + p2.y) * t +
		((2 * p0.y) - (5 * p1.y) + (4 * p2.y) - p3.y) * t * t +
		((-1 * p0.y) + (3 * p1.y) - (3 * p2.y) + p3.y) * t * t * t);

	//Z座標
	position.z = 0.5 * ((2.0f * p1.z) + ((-1 * p0.z) + p2.z) * t +
		((2 * p0.z) - (5 * p1.z) + (4 * p2.z) - p3.z) * t * t +
		((-1 * p0.z) + (3 * p1.z) - (3 * p2.z) + p3.z) * t * t * t);

	return position;
}

float Physics::easingOut(const float& start, const float& end, const float t)
{
	return start * (1.0f - t * (2 - t)) + end * (t * (2 - t));
}



void Physics::Initialize()
{
	//重力を下向き設定
	g1 += vel;
}
