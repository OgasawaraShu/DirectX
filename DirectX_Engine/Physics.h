#pragma once
#include "Math.h"
#include <DirectXMath.h>

#define PI 3.1415926535897932384626433832795
#define LENGTH      200                 // 紐の長さ
#define CLENGTH     (LENGTH * 2 * PI)   // 紐を伸ばして一周させた場合に出来る円の円周の長さ
#define MASS        0.1346              //物の質量
#define G           9.81                // 重力加速度

class Physics
{
public:
	void Initialize();

	void Pendulum(float x,float y);//振り子関数

	void Gravity(float x, float y);

	float Screw(float y,float g);
private:

	float acceleration_g = G / 60;//加速度
	float N = MASS * 0.08;//摩擦力
	float vel = -10;//下方向のベクトル

	double x1;     // 紐を伸ばして一周させた場合に出来る円周上の座標、０は紐が軸の真下にいる位置
	double angle;
	double speed; // xの速度

	float g1;

	//G1 += vel;

};

