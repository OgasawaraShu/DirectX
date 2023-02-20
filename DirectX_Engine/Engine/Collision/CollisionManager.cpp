#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
using namespace DirectX;


CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::CheckAllCollisions()
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	// �S�Ă̑g�ݍ��킹�ɂ��đ�����`�F�b�N
	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA) {
		itB = itA;
		++itB;
		for (; itB != colliders.end(); ++itB) {
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			//�������ꏏ�Ȃ�X�L�b�v
			if (colA->attribute == colB->attribute)
			{
				continue;
			}

			// �Ƃ��ɋ�
			if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Sphere* SphereB = dynamic_cast<Sphere*>(colB);
				DirectX::XMVECTOR inter;
				if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			//���ƕ���
			else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_PLANE) {
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Plane* PlaneB = dynamic_cast<Plane*>(colB);
				DirectX::XMVECTOR inter;
				if (Collision::CheckSphere2Plane(*SphereA, *PlaneB, &inter)) {
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			//���Ǝl�p(�����l�p����o����)
			else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_BOX) {
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Box* BoxB = dynamic_cast<Box*>(colB);
				DirectX::XMVECTOR inter;
				if (Collision::CheckSphere2Box(*SphereA, *BoxB, &inter)) {
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			//���Ǝl�p(�����l�p���瓖��������)
			
			else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_WALL) {
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Wall* WallB = dynamic_cast<Wall*>(colB);
				DirectX::XMVECTOR inter;
				if (Collision::CheckSphere2Box2(*SphereA, *WallB, &inter)) {
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			else 	if (colA->GetShapeType() == COLLISIONSHAPE_WALL &&
				colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
				Sphere* SphereA = dynamic_cast<Sphere*>(colB);
				Wall* WallB = dynamic_cast<Wall*>(colA);
				DirectX::XMVECTOR inter;
				if (Collision::CheckSphere2Box2(*SphereA, *WallB, &inter)) {
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			

		}
	}
}


bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
{
	return Raycast(ray, 0xffff, hitInfo, maxDistance);
}

bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
{
	bool result = false;
	std::forward_list<BaseCollider*>::iterator it;
	std::forward_list<BaseCollider*>::iterator it_hit;
	float distance = maxDistance;
	XMVECTOR inter;

	// �S�ẴR���C�_�[�Ƒ�����`�F�b�N
	it = colliders.begin();
	for (; it != colliders.end(); ++it) {
		BaseCollider* colA = *it;

		// ����������Ȃ���΃X�L�b�v
		if (!(colA->attribute & attribute)) {
			continue;
		}

		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE) {
			Sphere* sphere = dynamic_cast<Sphere*>(colA);

			float tempDistance;
			XMVECTOR tempInter;

			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}

		//�l�p(�����l�p����o����)
		
		else if (colA->GetShapeType() == COLLISIONSHAPE_PLANE) {
			Plane* plane = dynamic_cast<Plane*>(colA);

			float tempDistance;
			XMVECTOR tempInter;

			if (!Collision::CheckRay2Plane(ray, *plane, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}

	//���̎擾
	if (result && hitInfo) {
		hitInfo->distance = distance;
		hitInfo->inter = inter;
		hitInfo->collider = *it_hit;
		hitInfo->fbx = hitInfo->collider->GetObject3d();
	}

	return result;
}

bool CollisionManager::Spherecast(const Sphere& sphere, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
{
	bool result = false;
	std::forward_list<BaseCollider*>::iterator it;
	std::forward_list<BaseCollider*>::iterator it_hit;
	float distance = maxDistance;
	XMVECTOR inter;

	// �S�ẴR���C�_�[�Ƒ�����`�F�b�N
	it = colliders.begin();
	for (; it != colliders.end(); ++it) {
		BaseCollider* colA = *it;

		// ����������Ȃ���΃X�L�b�v
		if (!(colA->attribute & attribute)) {
			continue;
		}

	
		//���ƕ�
		if (colA->GetShapeType() == COLLISIONSHAPE_WALL) {
			Wall* wall = dynamic_cast<Wall*>(colA);

			float tempDistance{};
			XMVECTOR tempInter;

			if (!Collision::CheckSphere2Box2(sphere, *wall,&tempInter)) continue;
			
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}

	}

	if (result && hitInfo) {
		hitInfo->distance = distance;
		hitInfo->inter = inter;
		hitInfo->collider = *it_hit;
		hitInfo->fbx = hitInfo->collider->GetObject3d();
	}

	return result;
}

bool CollisionManager::Spherewall(const Sphere& sphere, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
{
	bool result = false;
	std::forward_list<BaseCollider*>::iterator it;
	std::forward_list<BaseCollider*>::iterator it_hit;
	float distance = maxDistance;
	XMVECTOR inter;

	// �S�ẴR���C�_�[�Ƒ�����`�F�b�N
	it = colliders.begin();
	for (; it != colliders.end(); ++it) {
		BaseCollider* colA = *it;

		// ����������Ȃ���΃X�L�b�v
		if (!(colA->attribute & attribute)) {
			continue;
		}


		//���ƕ�
		if (colA->GetShapeType() == COLLISIONSHAPE_WALL) {
			Wall* wall = dynamic_cast<Wall*>(colA);

			float tempDistance{};
			XMVECTOR tempInter;

			if (!Collision::CheckSphere2Box2(sphere, *wall, &tempInter)) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}

	}

	if (result && hitInfo) {
		hitInfo->distance = distance;
		hitInfo->inter = inter;
		hitInfo->collider = *it_hit;
		hitInfo->fbx = hitInfo->collider->GetObject3d();
	}

	return result;
}

