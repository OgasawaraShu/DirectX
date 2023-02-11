#pragma once

#include "CollisionPrimitive.h"

class Collision
{
public:
	//���ƕ��ʂ̓����蔻��
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane&
		plane, DirectX::XMVECTOR* inter = nullptr);
	//���Ǝl�p����o���Ƃ��̓����蔻��
	static bool CheckSphere2Box(const Sphere& sphere, const Box&
		box, DirectX::XMVECTOR* inter = nullptr);
	//����VER
	static bool CheckSphere2Box(const Sphere& sphere, unsigned short attribute, const Box&
		box, DirectX::XMVECTOR* inter = nullptr);

	//���Ǝl�p(�ǂ̓����蔻��)
	static bool CheckSphere2Box2(const Sphere& sphere, const Wall&
		wall, DirectX::XMVECTOR* inter = nullptr);
	//����VER
	static bool CheckSphere2Box2(const Sphere& sphere, unsigned short attribute, const Wall&
		wall, DirectX::XMVECTOR* inter = nullptr);
	//�_�ƎO�p�`�̓����蔻��
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
		const Triangle& triangle, DirectX::XMVECTOR* closest);
	//���C�ƕ��ʂ̓����蔻��
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	//���C�ƎO�p�`�̓����蔻��
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	//���C�Ƌ��̓����蔻��
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	//���Ƌ��̓����蔻��
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
    //���ƎO�p�`�̓����蔻��
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);

};

