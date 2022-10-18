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
				scene = 1;
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
	}
}
