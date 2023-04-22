#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "Math.h"
#include <vector>
#include <DirectXMath.h>

#define PI 3.1415926535897932384626433832795
#define LENGTH      200                 // 紐の長さ
#define CLENGTH     (LENGTH * 2 * PI)   // 紐を伸ばして一周させた場合に出来る円の円周の長さ
#define MASS        0.1346              //物の質量
#define G           9.81                // 重力加速度

class Physics
{
protected:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	void Initialize();

	void Pendulum(float x,float y);//振り子関数

	float Gravity(float x, float y);//重力

	void Jump(float y, float YGround);//JUMP

	float Screw(float y,float g);//回転運動
	//スプライン曲線
    static XMFLOAT3 splinePosition(const std::vector<XMFLOAT3>&points, size_t startIndex, float t);

	static float easingOut(const float& start, const float& end, const float t);

private:

	float acceleration_g = G / 60;//加速度
	float N = MASS * 0.08;//摩擦力
	float vel = -10;//下方向のベクトル
	float JumpVel = 2;

	double x1;     // 紐を伸ばして一周させた場合に出来る円周上の座標、０は紐が軸の真下にいる位置
	double angle;
	double speed; // xの速度

	float g1;
	float Time=0;
};

