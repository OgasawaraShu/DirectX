#pragma once
#include "input.h"
#include "GamePad.h"

class PortalGun
{
public://メンバ関数
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="window_width">画面幅</param>
    /// <param name="window_height">画面高さ</param>
    /// <param name="input">入力</param>
    PortalGun(Input* input,GamePad*gamepad);

    void Initialize();//初期化
    void Update();//更新


    void keyAction();//キー入力によるアクション

    //setter
    void setter_blue(float x,float y,float z,float r);

    float get_blueZ();//blueの球の座標を渡す
    float get_flagZ();
   
private://メンバ変数

    struct Gun//ポータルを射出する銃
    {
        float x;
        float y;
        float z;
        float flag;
    }GUN;

    struct Blue
    {
        float x;
        float y;
        float z;
        float r;
    }BLUE_BULLET;

    struct Red
    {
        float x;
        float y;
        float z;
        float r;
    }RED_BULLET;

    int Flag = 0;//射出フラグ

    // 入力クラスのポインタ
    Input* input;
    //Padクラスのポインタ
    GamePad* gamepad;
    //速度
   // Vector3 velecity_;
};

