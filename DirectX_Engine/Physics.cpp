#include "Physics.h"

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

	//y = 0.5 * G * Time * Time - JumpVel * Time + YGround;
	//Time += 1;
	//return y;
}

float Physics::Screw(float y, float g)
{
	float v = 0.0;

	//y���𒷂��@�d��(�����Ă�Ƃ��ɂ����)���p���[�Ƃ��ē�����͂��o��
	v = y * g;
	//v�͂��������͂����ɔ�΂���
	return v;
}



void Physics::Initialize()
{
	//�d�͂��������ݒ�
	g1 += vel;
}
