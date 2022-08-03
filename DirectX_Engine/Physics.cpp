#include "Physics.h"

void Physics::pendulum(float x, float y)
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
