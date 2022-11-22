#include "SceneSelect.h"

SceneSelect::SceneSelect(int window_width, int window_height, Input* input)
{
	this->input = input;

	// 画面サイズに対する相対的なスケールに調整
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void SceneSelect::Initialize()
{
	
}

void SceneSelect::Update()
{
	//マウスの入力を取得
	MouseGet();

	//タイトルシーンアクション
	Title();
}

void SceneSelect::MouseGet()
{

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	GetCursorPos(&p);

	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(hwnd, &p);

	//マウスの座標を代入
	mouseX = p.x;
	mouseY = p.y;
}

void SceneSelect::Title()
{
	if (scene == 0)
	{
		if (mouseX >= 412 && mouseX <= 835 && mouseY >= 506 && mouseY <= 568)
		{
			OverRed = 1;
			if (input->TriggerMouseLeft())
			{
				changeFlag = true;
				
				push = 1;
			}
			else
			{
				push = 0;
			}
		}
		else if(mouseX >= 540 && mouseX <= 714 && mouseY >= 604 && mouseY <= 662)
		{
			OverRed = 2;
			if (input->TriggerMouseLeft())
			{
				scene = 100;
				push = 1;
			}
			else
			{
				push = 0;
			}
		}
		else
		{
			OverRed = 0;
		}

		if (changeFlag == true)
		{
			change += 0.005;

			if (change > 1)
			{
				scene = 1;
				
				changeFlag = false;
			}
		}
	}

	
}

void SceneSelect::Scene1()
{
	if (scene == 1)
	{

		if (Exit == true)
		{
			change += 0.005;

			if (change > 1)
			{
				scene = 99;
			}
		}
		else
		{
			change = 0;
		}
	}

	if (scene == 99)
	{
		if (Load == true)
		{
			change = 0;
			LoadTime++;

			if (LoadTime > 140)
			{
				if (DoorRotate > -180)
				{
					DoorRotate -= 1;
				}
			}

			if (LoadTime > 170)
			{
				Load = false;
				LoadTime = 0;
			}
		}
		else
		{
			change += 0.005;

			if (change > 1)
			{
				scene = 100;
				DoorRotate = 0;
			}
		}
	}
}
