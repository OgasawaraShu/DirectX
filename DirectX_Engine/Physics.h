#pragma once
#include "Math.h"
#include <DirectXMath.h>

#define PI 3.1415926535897932384626433832795
#define LENGTH      200                 // •R‚Ì’·‚³
#define CLENGTH     (LENGTH * 2 * PI)   // •R‚ğL‚Î‚µ‚Äˆêü‚³‚¹‚½ê‡‚Éo—ˆ‚é‰~‚Ì‰~ü‚Ì’·‚³
#define MASS        0.1346              //•¨‚Ì¿—Ê
#define G           9.81                // d—Í‰Á‘¬“x

class Physics
{
public:
	void Initialize();

	void Pendulum(float x,float y);//U‚èqŠÖ”

	float Gravity(float x, float y);//d—Í

	void Jump(float y, float YGround);//JUMP

	float Screw(float y,float g);//‰ñ“]‰^“®
private:

	float acceleration_g = G / 60;//‰Á‘¬“x
	float N = MASS * 0.08;//–€C—Í
	float vel = -10;//‰º•ûŒü‚ÌƒxƒNƒgƒ‹
	float JumpVel = 2;

	double x1;     // •R‚ğL‚Î‚µ‚Äˆêü‚³‚¹‚½ê‡‚Éo—ˆ‚é‰~üã‚ÌÀ•WA‚O‚Í•R‚ª²‚Ì^‰º‚É‚¢‚éˆÊ’u
	double angle;
	double speed; // x‚Ì‘¬“x

	float g1;
	float Time=0;

	//G1 += vel;

};

