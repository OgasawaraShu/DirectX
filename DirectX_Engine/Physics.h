#pragma once
#include "Math.h"
#include <DirectXMath.h>

#define PI 3.1415926535897932384626433832795
#define LENGTH      200                 // �R�̒���
#define CLENGTH     (LENGTH * 2 * PI)   // �R��L�΂��Ĉ���������ꍇ�ɏo����~�̉~���̒���
#define MASS        0.1346              //���̎���
#define G           9.81                // �d�͉����x

class Physics
{
public:
	void Initialize();

	void Pendulum(float x,float y);//�U��q�֐�

	void Gravity(float x, float y);

	float Screw(float y,float g);
private:

	float acceleration_g = G / 60;//�����x
	float N = MASS * 0.08;//���C��
	float vel = -10;//�������̃x�N�g��

	double x1;     // �R��L�΂��Ĉ���������ꍇ�ɏo����~����̍��W�A�O�͕R�����̐^���ɂ���ʒu
	double angle;
	double speed; // x�̑��x

	float g1;

	//G1 += vel;

};

