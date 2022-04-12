#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//#include "Player.h"

class Enemy
{
public:

	typedef struct Enemy1//ザコ1
	{
		float X;//ｘ座標
		float Y;//ｙ座標

		float R;//半径
		int Flag;//フラグ
		int Color_Flag;//色のフラグ

		//int id;//配列の要素

		int Timer;//フラグを立たせるための時間

		int SpeedX;
		int SpeedY;

		float Angle;

		//struct Enemy1* next;
		//struct Enemy1* before;//前のデータがある場所
	}ENEMY1;

	ENEMY1 Enemy1[100];


	
	//ENEMY1* head = NULL; // リストの先頭
	//ENEMY1* tail = NULL; // リストの末尾

	
	//関数
//	ENEMY1* createCell(void);//要素の作成
	//void create(float x,float y,float r,int flag,int cr_flag,int id);//最後尾にリストを追加
	//void editCell(float x, float y, float r, int flag, int cr_flag, int id);//中身を編集


	//ここから配列Ver
	void Intialize();//初期化	

	void Update();//更新

	void Generation();//オブジェクトの生成

	void Flagged();//フラグが立つ

	void Move();//敵の動作

	void Rear();//後処理
};

