#include "Physics.h"


template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;
using XMVECTOR = DirectX::XMVECTOR;

void Physics::Pendulum(float x, float y)
{

	// �����ɏ]���đ��x�����Z
	// MASS�̒l������������Ƃ�����蓮��
	speed += -MASS * G * sin(x1 / LENGTH);

	// ���x�ɏ]���ĉ~��̍��W��ύX
	x1 += speed;

	// �������_�Ƃ����ꍇ�̂Ԃ牺�����Ă��镨�̍��W���Z�o
	// ���̂܂܂��Ɓ|45�`45�x�̐U��q�ɂȂ�̂�
	// ���v����90�x�iPI/2.0�j��]
	angle = x1 / LENGTH + PI / 2.0;

	// ���߂��p�x���玲�����_�Ƃ����~����̍��W���擾
	x = cos(angle) * LENGTH;
	y = sin(angle) * LENGTH;
}

float Physics::Gravity(float x, float y)
{
	//�d�͂�����y���ŗ���
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

	//y���𒷂��@�d��(�����Ă�Ƃ��ɂ����)���p���[�Ƃ��ē�����͂��o��
	v = y * g;
	//v�͂��������͂����ɔ�΂���
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

	//X���W
	position.x= 0.5 * ((2.0f * p1.x) + ((-1 * p0.x) + p2.x) * t +
		((2 * p0.x) - (5 * p1.x) + (4 * p2.x) - p3.x) * t * t +
		((-1 * p0.x) + (3 * p1.x) - (3 * p2.x) + p3.x) * t * t * t);

	//Y���W
	position.y = 0.5 * ((2.0f * p1.y) + ((-1 * p0.y) + p2.y) * t +
		((2 * p0.y) - (5 * p1.y) + (4 * p2.y) - p3.y) * t * t +
		((-1 * p0.y) + (3 * p1.y) - (3 * p2.y) + p3.y) * t * t * t);

	//Z���W
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
	//�d�͂��������ݒ�
	g1 += vel;
}
