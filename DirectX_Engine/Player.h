#pragma once
//#pragma comment (lib, "xinput.lib")
//#include <xinput.h>
#include <DirectXMath.h>
#include "GamePad.h"
#include"Item.h"
#include"Input.h"


class Player
{
private:
   
public:
    //変数

   //センター座標
    float Central_x = 0;
    float Central_y = 0;

    //回転する円の大きさ
    float Player_radius = 200;
    //回転するための値
    float timer = 0;
    //切り替え変数
    int Switch = 0;

    //赤球座標
    float Player_RedX = 0;
    float Player_RedY = 0;

    float Red_R = 50;
    float Blue_R = 30;

    //蒼球座標
    float Player_BlueX = 0;
    float Player_BlueY = 0;

    //球のフラグ関連
    float Red_Lv = 1;
    float Blue_Lv = 1;

    int Radius = 100;



    //トリガー処理のための変数
    int Old_iPad_left = 0, Old_iPad_right = 0, iOld_Pad_up = 0, Old_iPad_down = 0;
    int Old_iPad_leftshoulder = 0, Old_iPad_rightshoulder = 0;
    int Old_iPad_A = 0, Old_iPad_B = 0, Old_iPad_X = 0, Old_iPad_Y = 0;


    //関数
    void Controll();//位置反転

};

