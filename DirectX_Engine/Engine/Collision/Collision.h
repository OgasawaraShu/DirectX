#pragma once

#include "CollisionPrimitive.h"

class Collision
{
public:
	//球と平面の当たり判定
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane&
		plane, DirectX::XMVECTOR* inter = nullptr);
	//球と四角から出たときの当たり判定
	static bool CheckSphere2Box(const Sphere& sphere, const Box&
		box, DirectX::XMVECTOR* inter = nullptr);
	//属性VER
	static bool CheckSphere2Box(const Sphere& sphere, unsigned short attribute, const Box&
		box, DirectX::XMVECTOR* inter = nullptr);

	//球と四角(壁の当たり判定)
	static bool CheckSphere2Box2(const Sphere& sphere, const Wall&
		wall, DirectX::XMVECTOR* inter = nullptr);
	//属性VER
	static bool CheckSphere2Box2(const Sphere& sphere, unsigned short attribute, const Wall&
		wall, DirectX::XMVECTOR* inter = nullptr);
	//点と三角形の当たり判定
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
		const Triangle& triangle, DirectX::XMVECTOR* closest);
	//レイと平面の当たり判定
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	//レイと三角形の当たり判定
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	//レイと球の当たり判定
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	//球と球の当たり判定
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
    //球と三角形の当たり判定
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);

};

