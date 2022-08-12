#include "Physics.h"

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

	//y = 0.5 * G * Time * Time - JumpVel * Time + YGround;
	//Time += 1;
	//return y;
}

float Physics::Screw(float y, float g)
{
	float v = 0.0;

	//y軸を長さ　重力(落ちてるときにある力)をパワーとして投げる力を出力
	v = y * g;
	//vはｘもしくはｚ軸に飛ばす力
	return v;
}



void Physics::Initialize()
{
	//重力を下向き設定
	g1 += vel;
}
