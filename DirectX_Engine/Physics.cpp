#include "Physics.h"

void Physics::pendulum(float x, float y)
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
