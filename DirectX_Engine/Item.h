#pragma once

//#include "Player.h"
//#include "Enemy.h"

class Item
{
private:

public:
	//変数
	typedef struct LEG//足移動速度アイテム
	{
		int Flag;

		int Limit;

		float Effect;

		float X;

		float Y;
	}Leg;
	Leg LEG_[10];


	typedef struct ARM//腕回転速度アイテム
	{
		int Flag;

		int Limit;

		float Effect;

		float X;

		float Y;
	}Arm;
	Arm ARM_[10];


	typedef struct STRONG//敵を一発アウト
	{
		int Flag;

		int Limit;

		float Effect;

		float X;

		float Y;
	}Strong;
	Strong STRONG_[10];


	typedef struct STUDY//経験値アップ
	{
		int Flag;

		int Limit;

		float Effect;

		float X;

		float Y;
	}Study;
	Study STUDY_[10];

	/*
	typedef struct DESTROY//殲滅アイテム
	{
		int Flag;

		int Limit;

		float X;

		float Y;
	}Destroy;
	Destroy DESTOROY_[10];
	*/
	//関数
	void Intialize();//初期化	

	void Update();//更新



	void Leg_SpeedUp();//移動速度アップ

	void Arm_SpeedUp();//腕回転速度アップ

	void StrongArm();//ザコを一発で倒す

	void StudyUp();//取得経験値アップ

	void Destroy();//画面内の敵を倒す
};

