#include "Player.h"



void Player::Controll()
{
   
    GamePad* GP=nullptr;
    GP = new GamePad();
    
    Item* item = nullptr;
    item = new Item();

    //パッドの更新
    GP->Update();
    item->Update();

    //操作 色の切り替え
    if (GP->iPad_A==1  && Switch == 0&&Old_iPad_A==0)
    {
        Switch = 1;
    }
    else if (GP->iPad_A ==1 && Switch == 1&&Old_iPad_A==0)
    {
        Switch = 0;
    }

  //  GP.vibration.wLeftMotorSpeed = 65535;
   
   // XInputSetState(0, &GP.vibration);

    //トリガー処理のための記憶
    Old_iPad_left = GP->iPad_left, Old_iPad_right = GP->iPad_right, iOld_Pad_up = GP->iPad_up, Old_iPad_down = GP->iPad_down;
    Old_iPad_leftshoulder = GP->iPad_leftshoulder, Old_iPad_rightshoulder = GP->iPad_rightshoulder;
    Old_iPad_A = GP->iPad_A, Old_iPad_B = GP->iPad_B, Old_iPad_X = GP->iPad_X, Old_iPad_Y = GP->iPad_Y;
 
    if (GP->state.Gamepad.sThumbLX != 0 || GP->state.Gamepad.sThumbLY != 0)//ゲームパッドアナログスティック入力時処理
    {
        Central_x += static_cast<FLOAT>(GP->state.Gamepad.sThumbLX / 32767.0  * (2.0f+item->LEG_->Effect));

        Central_y -= static_cast<FLOAT>(GP->state.Gamepad.sThumbLY / 32767.0  * (2.0f+item->LEG_->Effect));
    }

    //回転
    if (GP->iPad_leftshoulder == 1)
    {
       timer += 0.5;
    }

    if (GP->iPad_rightshoulder == 1)
    {
        timer -= 0.5;
    }


    //フラグ切り替えによる位置反転
    if (Switch == 0)
    {

        Player_RedX = sin(timer * 0.07) * Player_radius + Central_x;
        Player_RedY = cos(timer * 0.07) * Player_radius + Central_y;

        Player_BlueX = -sin(timer * 0.07) * Player_radius + Central_x;
        Player_BlueY = -cos(timer * 0.07) * Player_radius + Central_y;
    }
    else
    {
        Player_RedX = -sin(timer * 0.07) * Player_radius + Central_x;
        Player_RedY = -cos(timer * 0.07) * Player_radius + Central_y;

        Player_BlueX = sin(timer * 0.07) * Player_radius + Central_x;
        Player_BlueY = cos(timer * 0.07) * Player_radius + Central_y;
    }
   
}
