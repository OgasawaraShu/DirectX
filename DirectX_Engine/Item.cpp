#include "Item.h"
#include <vcruntime_string.h>

//Player player;

void Item::Intialize()//初期化
{
    memset(LEG_, 0, sizeof(LEG_));

    //構造体初期化代入
    for (int i = 0; i++; i < 10)
    {
        LEG_[i] = { 0,0,0,200,200};
    }

    LEG_[0].X = 300;
    LEG_[0].Y = 500;

    memset(ARM_, 0, sizeof(ARM_));

    //構造体初期化代入
    for (int i = 0; i++; i < 10)
    {
        ARM_[i] = { 0,0,0,0,0 };
    }

    memset(STRONG_, 0, sizeof(STRONG_));

    //構造体初期化代入
    for (int i = 0; i++; i < 10)
    {
        STRONG_[i] = { 0,0,0,0,0 };
    }

    memset(STUDY_, 0, sizeof(STUDY_));

    //構造体初期化代入
    for (int i = 0; i++; i < 10)
    {
        STUDY_[i] = { 0,0,0,0,0 };
    }

    /*
    memset(DESTOROY_, 0, sizeof(DESTOROY_));

    //構造体初期化代入
    for (int i = 0; i++; i < 10)
    {
        DESTOROY_[i] = { 0,0,0,0};
    }
    */
}

void Item::Update()//更新
{
    //if(LEG_->Flag==1)
    Leg_SpeedUp();

    if(ARM_->Flag==1)
    Arm_SpeedUp();

    if(STRONG_->Flag==1)
    StrongArm();

    if(STUDY_->Flag==1)
    StudyUp();

   // player.Controll();
   // if(DESTOROY_->Flag==1)
   // Destroy();
}

void Item::Leg_SpeedUp()//足速度アップ
{
    if (LEG_->Flag == 1)
    {
        LEG_->Limit++;
        LEG_->Effect = 5;
    }

    if (LEG_->Limit >= 600)
    {
        LEG_->Flag = 0;
        LEG_->Limit = 0;
    }
}

void Item::Arm_SpeedUp()//腕速度アップ
{
   
}

void Item::StrongArm()//敵を一発で倒す
{
    
}

void Item::StudyUp()//取得経験値アップ
{
  
}

void Item::Destroy()//画面内の敵を倒す
{
  
}
