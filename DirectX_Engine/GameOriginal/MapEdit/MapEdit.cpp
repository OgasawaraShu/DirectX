#include "MapEdit.h"
#include "../../Engine/Collision/Wallcolider.h"
#include "../../Engine/Collision/SphereCollider.h"
#include "../../Engine/Collision/CollisionManager.h"
#include "../../Engine/Collision/BaseCollider.h"
#include "../../Engine/Collision/CollisionAttribute.h"
#include "../../Engine/Collision/Collision.h"
#include <imgui.h>
#include "stdio.h"
#include <cassert>
#include <sstream>
#include <iomanip>

#include <fstream>

Camera* MapEdit::M_camera = nullptr;
MapEdit::MapEdit(int window_width, int window_height, Input* input)
	:ObjFbx(input)
{
	this->input = input;
	this->Window_Width = window_width;
	this->Window_Height = window_height;

	// 画面サイズに対する相対的なスケールに調整
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void MapEdit::MapInitialize()
{

}

void MapEdit::CreateObj(ID3D12GraphicsCommandList* cmdList)
{
	//選択をしちゃいけいないフラグを戻す
	Select_obj_flag = false;
	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();


	//マウス座標取得
	GetCursorPos(&mousePosition);

	//クライアント座標に変換
	ScreenToClient(hwnd, &mousePosition);

	//long型をfloat型に変換
	mouse.x = mousePosition.x;
	mouse.y = mousePosition.y;


	//キーを押して生成
	//imgui関連
	ImGui::Begin("Option");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(300, 200));

	if (ImGui::TreeNode("Create")) {
		//フェンスモデル生成
		if (ImGui::Button("Fence Create"))
		{
			//生成
			std::unique_ptr<MapEdit>fence = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//初期化
			fence->Initialize();
			//ここでモデルの形状をセット
			fence->SetModel(fence_model);
			//コリジョンのサイズ登録(初期は０)
			fence->SetColisionSize({ 10,10,2 });
			//スケールのサイズ
			fence->SetScale({ 0.1,0.1,0.1 });
			//ポジションの初期位置設定
			fence->SetPosition({ 0,0,0 });
			//モデルを指定
			fence->SetFbxmodelType(Fence_model_num);
			//ver指定
			fence->SetFbxVer(null_ver);
			//登録
			Fences.push_back(std::move(fence));

			//当たり判定確認用OBJ生成
			CreateCollisionBox();
		}

		//ドラムモデル生成
		if (ImGui::Button("Drum Create"))
		{
			//生成
			std::unique_ptr<MapEdit>drum = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//初期化
			drum->Initialize();
			//ここでモデルの形状をセット
			drum->SetModel(drum_model);
			//コリジョンのサイズ登録(初期は０)
			drum->SetColisionSize({ 6,6,6 });
			//スケールのサイズ
			drum->SetScale({ 0.02,0.02,0.02 });
			//ポジションの初期位置設定
			drum->SetPosition({ 0,10,0 });
			//モデルを指定
			drum->SetFbxmodelType(Drum_model_num);
			//ver指定
			drum->SetFbxVer(null_ver);
			//登録
			Drums.push_back(std::move(drum));

			//当たり判定確認用OBJ生成
			CreateCollisionBox();
		}

		//壁モデル生成
		if (ImGui::Button("Wall Create"))
		{
			//生成
			std::unique_ptr<MapEdit>wall = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//初期化
			wall->Initialize();
			//ここでモデルの形状をセット
			wall->SetModel(wall_model);
			//コリジョンのサイズ登録(初期は０)
			wall->SetColisionSize({ 0,0,0 });
			//モデルを指定
			wall->SetFbxmodelType(Wall_model_num);
			//ver指定
			wall->SetFbxVer(null_ver);
			//登録
			Walls.push_back(std::move(wall));

			//当たり判定確認用OBJ生成
			CreateCollisionBox();
		}


		//床モデル生成
		if (ImGui::Button("Floor Create"))
		{
			//生成
			std::unique_ptr<MapEdit>floor = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//初期化
			floor->Initialize();
			//ここでモデルの形状をセット
			floor->SetModel(floor_model);
			//コリジョンのサイズ登録(初期は０)
			floor->SetColisionSize({ 0,0,0 });
			//スケールのサイズ
			floor->SetScale({ 1,0.05,1 });
			//モデルを指定
			floor->SetFbxmodelType(Floor_model_num);
			//ver指定
			floor->SetFbxVer(null_ver);
			//登録
			Floors.push_back(std::move(floor));

			//当たり判定確認用OBJ生成
			CreateCollisionBox();
		}

		//出口モデル生成
		if (ImGui::Button("Exit Create"))
		{
			//生成
			std::unique_ptr<MapEdit>exit = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//初期化
			exit->Initialize();
			//ここでモデルの形状をセット
			exit->SetModel(exit_model);
			//コリジョンのサイズ登録(初期は０)
			exit->SetColisionSize({ 10,10,10 });
			//スケールのサイズ
			exit->SetScale({ 0.1,0.1,0.1 });
			//モデルを指定
			exit->SetFbxmodelType(Exit_model_num);
			//ver指定
			exit->SetFbxVer(exit_ver);
			//登録
			Exits.push_back(std::move(exit));

			//当たり判定確認用OBJ生成
			CreateCollisionBox();
		}

		//Gunモデル生成
		if (ImGui::Button("Gun Create"))
		{
			//生成
			std::unique_ptr<MapEdit>gun = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//初期化
			gun->Initialize();
			//ここでモデルの形状をセット
			gun->SetModel(gun_model);
			//コリジョンのサイズ登録(初期は０)
			gun->SetColisionSize({ 1,1,1});
			//スケールのサイズ
			gun->SetScale({ 0.012,0.012,0.012 });
			//モデルを指定
			gun->SetFbxmodelType(Gun_model_num);
			//ver指定
			gun->SetFbxVer(gun_ver);
			//登録
			Guns.push_back(std::move(gun));

			//当たり判定確認用OBJ生成
			CreateCollisionBox();
		}


		//箱モデル生成
		if (ImGui::Button("Box Create"))
		{
			//生成
			std::unique_ptr<MapEdit>box = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//初期化
			box->Initialize();
			//ここでモデルの形状をセット
			box->SetModel(box_model);
			//コリジョンのサイズ登録(初期は０)
			box->SetColisionSize({ 10,10,10 });
			//スケールのサイズ
			box->SetScale({ 3.5,3.5,3.5 });
			//モデルを指定
			box->SetFbxmodelType(Box_model_num);
			//ver指定
			box->SetFbxVer(obj_ver);
			//登録
			Boxs.push_back(std::move(box));

			//当たり判定確認用OBJ生成
			CreateCollisionBox();
		}


		ImGui::TreePop();
	}

	//ボタン押されたらコリジョンを可視化するかしないか変えられる
	if (ImGui::Button("Collision Draw"))
	{
		if (Colision_draw_flag == false)
		{
			Colision_draw_flag = true;
		}
		else
		{
			Colision_draw_flag = false;
		}
	}
	//ボタンが押されてコピー元の中に情報が入っているのならペーストして生成する
	if (ImGui::Button("Paste")||(input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_V)))
	{
		PasteInfo();
	}


	//データ保存（仮）
	if (ImGui::Button("Save"))
	{
		//情報を記録
		for (std::unique_ptr<MapEdit>& fence_ : Fences) {
			fence_->MapSave(fence_);
		}


		for (std::unique_ptr<MapEdit>& drum_ : Drums) {
			drum_->MapSave(drum_);
		}


		for (std::unique_ptr<MapEdit>& wall_ : Walls) {
			wall_->MapSave(wall_);
		}


		for (std::unique_ptr<MapEdit>& floor_ : Floors) {
			floor_->MapSave(floor_);
		}

		for (std::unique_ptr<MapEdit>& exit_ : Exits) {
			exit_->MapSave(exit_);
		}

		for (std::unique_ptr<MapEdit>& gun_ : Guns) {
			gun_->MapSave(gun_);
		}

		for (std::unique_ptr<MapEdit>& box_ : Boxs) {
			box_->MapSave(box_);
		}

		for (std::unique_ptr<MapEdit>& spawn_ : Respawns) {
			spawn_->MapSave(spawn_);
		}
	}

	ImGui::End();

	ArowCreate();
	

	//PIckUpされたObjの内容を表示	
	ImGui::Begin("PickUp Obj");
	ImGui::SetWindowPos(ImVec2(0, 400));
	ImGui::SetWindowSize(ImVec2(300, 200));

    Arow_draw_flag = false;
	
	for (std::unique_ptr<MapEdit>& fence_ : Fences) {
		if (fence_->GetSelectFlag() == true)
		{
			fence_->AdjustmentObj(fence_);

			Arow_draw_flag = true;

			Arow_pos = fence_->GetPosition();
			//矢印による位置の移動
			ArowUpdate(fence_,mouse);

			
			//押されたらモデルの情報を一時保存(copy)
			if (ImGui::Button("Copy")||(input->PushKey(DIK_LCONTROL)&&input->PushKey(DIK_C)))
			{
				CopyModelInfo(fence_->GetFbxmodelType(), fence_->GetVer(), fence_->GetPosition(), fence_->GetScale(), fence_->GetRotation(), fence_->GetColisionSize());
			}
		}
	}

	for (std::unique_ptr<MapEdit>& drum_ : Drums) {
		if (drum_->GetSelectFlag() == true)
		{
			drum_->AdjustmentObj(drum_);

			Arow_draw_flag = true;

			//矢印の位置変数代入
			Arow_pos = drum_->GetPosition();

			//矢印による位置の移動
			ArowUpdate(drum_, mouse);


			//押されたらモデルの情報を一時保存(copy)
			if (ImGui::Button("Copy") || (input->PushKey(DIK_LCONTROL) && input->PushKey(DIK_C)))
			{
				CopyModelInfo(drum_->GetFbxmodelType(), drum_->GetVer(), drum_->GetPosition(), drum_->GetScale(), drum_->GetRotation(), drum_->GetColisionSize());
			}
		}
	}

	for (std::unique_ptr<MapEdit>& walls_ : Walls) {
		if (walls_->GetSelectFlag() == true)
		{
			walls_->AdjustmentObj(walls_);

			Arow_draw_flag = true;


			//矢印の位置変数代入
			Arow_pos = walls_->GetPosition();

			//矢印による位置の移動
			ArowUpdate(walls_, mouse);


			//押されたらモデルの情報を一時保存(copy)
			if (ImGui::Button("Copy") || (input->PushKey(DIK_LCONTROL) && input->PushKey(DIK_C)))
			{
				CopyModelInfo(walls_->GetFbxmodelType(), walls_->GetVer(), walls_->GetPosition(), walls_->GetScale(), walls_->GetRotation(), walls_->GetColisionSize());
			}
		}
	}

	for (std::unique_ptr<MapEdit>& floor_ : Floors) {
		if (floor_->GetSelectFlag() == true)
		{
			floor_->AdjustmentObj(floor_);

			Arow_draw_flag = true;


			//矢印の位置変数代入
			Arow_pos = floor_->GetPosition();

			//矢印による位置の移動
			ArowUpdate(floor_, mouse);



			//押されたらモデルの情報を一時保存(copy)
			if (ImGui::Button("Copy") || (input->PushKey(DIK_LCONTROL) && input->PushKey(DIK_C)))
			{
				CopyModelInfo(floor_->GetFbxmodelType(), floor_->GetVer(), floor_->GetPosition(), floor_->GetScale(), floor_->GetRotation(), floor_->GetColisionSize());
			}
		}
	}

	for (std::unique_ptr<MapEdit>& exit_ : Exits) {
		if (exit_->GetSelectFlag() == true)
		{
			exit_->AdjustmentObj(exit_);

			Arow_draw_flag = true;


			//矢印の位置変数代入
			Arow_pos = exit_->GetPosition();

			//矢印による位置の移動
			ArowUpdate(exit_, mouse);

		}
	}


	for (std::unique_ptr<MapEdit>& gun_ : Guns) {
		if (gun_->GetSelectFlag() == true)
		{
			gun_->AdjustmentObj(gun_);

			Arow_draw_flag = true;


			//矢印の位置変数代入
			Arow_pos = gun_->GetPosition();

			//矢印による位置の移動
			ArowUpdate(gun_, mouse);



			//押されたらモデルの情報を一時保存(copy)
			if (ImGui::Button("Copy") || (input->PushKey(DIK_LCONTROL) && input->PushKey(DIK_C)))
			{
				CopyModelInfo(gun_->GetFbxmodelType(), gun_->GetVer(), gun_->GetPosition(), gun_->GetScale(), gun_->GetRotation(), gun_->GetColisionSize());
			}
		}
	}


	for (std::unique_ptr<MapEdit>& box_ : Boxs) {
		if (box_->GetSelectFlag() == true)
		{
			box_->AdjustmentObj(box_);

			Arow_draw_flag = true;


			//矢印の位置変数代入
			Arow_pos = box_->GetPosition();

			//矢印による位置の移動
			ArowUpdate(box_, mouse);



			//押されたらモデルの情報を一時保存(copy)
			if (ImGui::Button("Copy") || (input->PushKey(DIK_LCONTROL) && input->PushKey(DIK_C)))
			{
				CopyModelInfo(box_->GetFbxmodelType(), box_->GetVer(), box_->GetPosition(), box_->GetScale(), box_->GetRotation(), box_->GetColisionSize());
			}
		}
	}


	for (std::unique_ptr<MapEdit>& spawn_ : Respawns) {
		if (spawn_->GetSelectFlag() == true)
		{
			spawn_->AdjustmentObj(spawn_);
		}
	}
	ImGui::End();

	
	//各OBJの情報の調整処理
	//更新
	//imgui関連
	ImGui::Begin("Obj_Data");
	ImGui::SetWindowPos(ImVec2(0, 200));
	ImGui::SetWindowSize(ImVec2(300, 200));


	//Objのカウント用
	int Fence_count = 1;
	int Drum_count = 1;
	int Wall_count = 1;
	int Floor_count = 1;
	int Exit_count = 1;
	int Gun_count = 1;
	int Box_count = 1;

	//当たり判定可視化用代入変数
	int Collision_count = 0;
	XMFLOAT3 pos[100] = {};
	XMFLOAT3 rotate[100] = {};
	XMFLOAT3 colision_size[100] = {};


	for (std::unique_ptr<MapEdit>& fence_ : Fences) {

		//削除フラグが立っているのならOBJを削除
		Fences.remove_if([](std::unique_ptr<MapEdit>& fence_) {
			return fence_->GetDeadFlag();
			});

		//OBJの名前
		char str[1024] = { 'F','e','n','c','e','_' };
		//番号割りあて
		char str2[1024];
		//intをstringに変換
		sprintf_s(str2, "%d", Fence_count);
		//文字列を結合
		strcat_s(str, str2);
		//ツリーに格納
		if (ImGui::TreeNode(str)) {
			fence_->AdjustmentObj(fence_);
			ImGui::TreePop();
		}
		//Fbxの更新処理
		fence_->Update();
		//OBJの単独PICKUP
		fence_->ObjSelect(fence_, mouse);

		//for文で１増やす
		Fence_count += 1;

		pos[Collision_count] = { fence_->GetPosition() };
		rotate[Collision_count] = { fence_->GetRotation() };
		colision_size[Collision_count] = { fence_->GetColisionSize() };
		Collision_count += 1;
	}


	for (std::unique_ptr<MapEdit>& drum_ : Drums) {


		//削除フラグが立っているのならOBJを削除
		Drums.remove_if([](std::unique_ptr<MapEdit>& drum_) {
			return drum_->GetDeadFlag();
			});

		//OBJの名前
		char str[1024] = { 'D','r','u','m','_' };
		//番号割りあて
		char str2[1024];
		//intをstringに変換
		sprintf_s(str2, "%d", Drum_count);
		//文字列を結合
		strcat_s(str, str2);
		//ツリーに格納
		if (ImGui::TreeNode(str)) {
			drum_->AdjustmentObj(drum_);
			ImGui::TreePop();
		}
		//Fbxの更新処理
		drum_->Update();
		//OBJの単独PICKUP
		drum_->ObjSelect(drum_, mouse);


		//for文で１増やす
		Drum_count += 1;

		pos[Collision_count] = { drum_->GetPosition() };
		rotate[Collision_count] = { drum_->GetRotation() };
		colision_size[Collision_count] = { drum_->GetColisionSize() };
		Collision_count += 1;
	}

	for (std::unique_ptr<MapEdit>& wall_ : Walls) {


		//削除フラグが立っているのならOBJを削除
		Walls.remove_if([](std::unique_ptr<MapEdit>& wall_) {
			return wall_->GetDeadFlag();
			});

		//OBJの名前
		char str[1024] = { 'W','a','l','l','_' };
		//番号割りあて
		char str2[1024];
		//intをstringに変換
		sprintf_s(str2, "%d", Wall_count);
		//文字列を結合
		strcat_s(str, str2);
		//ツリーに格納
		if (ImGui::TreeNode(str)) {
			wall_->AdjustmentObj(wall_);
			ImGui::TreePop();
		}
		//Fbxの更新処理
		wall_->Update();
		//OBJの単独PICKUP
		wall_->ObjSelect(wall_, mouse);


		//for文で１増やす
		Wall_count += 1;

		pos[Collision_count] = { wall_->GetPosition() };
		rotate[Collision_count] = { wall_->GetRotation() };
		colision_size[Collision_count] = { wall_->GetColisionSize() };
		Collision_count += 1;
	}

	for (std::unique_ptr<MapEdit>& floor_ : Floors) {


		//削除フラグが立っているのならOBJを削除
		Floors.remove_if([](std::unique_ptr<MapEdit>& floor_) {
			return floor_->GetDeadFlag();
			});

		//OBJの名前
		char str[1024] = { 'F','l','o','o','r','_' };
		//番号割りあて
		char str2[1024];
		//intをstringに変換
		sprintf_s(str2, "%d", Floor_count);
		//文字列を結合
		strcat_s(str, str2);
		//ツリーに格納
		if (ImGui::TreeNode(str)) {
			floor_->AdjustmentObj(floor_)
				;
			ImGui::TreePop();
		}
		//Fbxの更新処理
		floor_->Update();
		//OBJの単独PICKUP
		floor_->ObjSelect(floor_, mouse);


		//for文で１増やす
		Floor_count += 1;

		pos[Collision_count] = { floor_->GetPosition() };
		rotate[Collision_count] = { floor_->GetRotation() };
		colision_size[Collision_count] = { floor_->GetColisionSize() };
		Collision_count += 1;
	}


	for (std::unique_ptr<MapEdit>& gun_ : Guns) {


		//削除フラグが立っているのならOBJを削除
		Guns.remove_if([](std::unique_ptr<MapEdit>& gun_) {
			return gun_->GetDeadFlag();
			});

		//OBJの名前
		char str[1024] = { 'G','u','n','_' };
		//番号割りあて
		char str2[1024];
		//intをstringに変換
		sprintf_s(str2, "%d", Gun_count);
		//文字列を結合
		strcat_s(str, str2);
		//ツリーに格納
		if (ImGui::TreeNode(str)) {
			gun_->AdjustmentObj(gun_)
				;
			ImGui::TreePop();
		}
		//Fbxの更新処理
		gun_->Update();
		//OBJの単独PICKUP
		gun_->ObjSelect(gun_, mouse);


		//for文で１増やす
		Gun_count += 1;

		pos[Collision_count] = { gun_->GetPosition() };
		rotate[Collision_count] = { gun_->GetRotation() };
		colision_size[Collision_count] = { gun_->GetColisionSize() };
		Collision_count += 1;
	}


	for (std::unique_ptr<MapEdit>& box_ : Boxs) {


		//削除フラグが立っているのならOBJを削除
		Boxs.remove_if([](std::unique_ptr<MapEdit>& box_) {
			return box_->GetDeadFlag();
			});

		//OBJの名前
		char str[1024] = { 'B','o','x','_' };
		//番号割りあて
		char str2[1024];
		//intをstringに変換
		sprintf_s(str2, "%d", Box_count);
		//文字列を結合
		strcat_s(str, str2);
		//ツリーに格納
		if (ImGui::TreeNode(str)) {
			box_->AdjustmentObj(box_)
				;
			ImGui::TreePop();
		}
		//Fbxの更新処理
		box_->Update();
		//OBJの単独PICKUP
		box_->ObjSelect(box_, mouse);


		//for文で１増やす
		Box_count += 1;

		pos[Collision_count] = { box_->GetPosition() };
		rotate[Collision_count] = { box_->GetRotation() };
		colision_size[Collision_count] = { box_->GetColisionSize() };
		Collision_count += 1;
	}

	for (std::unique_ptr<MapEdit>& exit_ : Exits) {


		//削除フラグが立っているのならOBJを削除
		Exits.remove_if([](std::unique_ptr<MapEdit>& exit_) {
			return exit_->GetDeadFlag();
			});

		//OBJの名前
		char str[1024] = { 'E','x','i','t','_' };
		//番号割りあて
		char str2[1024];
		//intをstringに変換
		sprintf_s(str2, "%d", Exit_count);
		//文字列を結合
		strcat_s(str, str2);
		//ツリーに格納
		if (ImGui::TreeNode(str)) {
			exit_->AdjustmentObj(exit_);
			ImGui::TreePop();
		}
		//Fbxの更新処理
		exit_->Update();
		//OBJの単独PICKUP
		exit_->ObjSelect(exit_, mouse);


		//for文で１増やす
		Exit_count += 1;

		pos[Collision_count] = { exit_->GetPosition() };
		rotate[Collision_count] = { exit_->GetRotation() };
		colision_size[Collision_count] = { exit_->GetColisionSize() };
		Collision_count += 1;
	}

	CreateSpawn();
	//imguiの終わり
	ImGui::End();

	Collision_count = 0;//配列要素数カウントを0にする
	for (std::unique_ptr<MapEdit>& collision_ : Collisions) {
		//各情報をコリジョンに代入する
		UpdateCollisionBox(colision_size[Collision_count], pos[Collision_count], rotate[Collision_count], collision_);
		Collision_count += 1;
	}

	

	//描画

	//フラグが立っていたらコリジョンを可視化する
	if (Colision_draw_flag == true)
	{
		for (std::unique_ptr<MapEdit>& collision_ : Collisions) {
			collision_->Draw2(cmdList);
		}
	}

	for (std::unique_ptr<MapEdit>& fence_ : Fences) {
		fence_->Draw2(cmdList);
	}

	for (std::unique_ptr<MapEdit>& drum_ : Drums) {
		drum_->Draw2(cmdList);
	}

	for (std::unique_ptr<MapEdit>& wall_ : Walls) {
		wall_->Draw2(cmdList);
	}

	for (std::unique_ptr<MapEdit>& floor_ : Floors) {
		floor_->Draw2(cmdList);
	}

	for (std::unique_ptr<MapEdit>& exit_ : Exits) {
		exit_->Draw2(cmdList);
	}


	for (std::unique_ptr<MapEdit>& guns_ : Guns) {
		guns_->Draw2(cmdList);
	}


	for (std::unique_ptr<MapEdit>& box_ : Boxs) {
		box_->Draw2(cmdList);
	}

	for (std::unique_ptr<MapEdit>& spawn_ : Respawns) {
		spawn_->Draw2(cmdList);
	}

	if (Arow_draw_flag == true)
	{
		for (std::unique_ptr<MapEdit>& arow_ : Arows) {
			arow_->Draw2(cmdList);
		}
	}
}

void MapEdit::WriteSet(int type,int ver, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize)
{
	FILE* fp1;
	//テキスト暗号化した状態で保存
	fp1 = _fsopen("GameOriginal/MapEdit/STAGE2.txt", "a", _SH_DENYNO);

	if (fp1 == NULL) {
		assert(0);
	}
	else {
		//fileがnullじゃないのなら各情報を入れる
		fprintf(fp1, "\r\n");//改行
		fprintf(fp1, "TYPE,%d\r\n", type);//モデルタイプ
		fprintf(fp1, "POSITION,%f,%f,%f\r\n", pos.x, pos.y, pos.z);//pos
		fprintf(fp1, "SCALE,%f,%f,%f\r\n", scale.x, scale.y, scale.z);//scale
		fprintf(fp1, "ROTATE,%f,%f,%f\r\n", rotate.x, rotate.y, rotate.z);//rotate
		fprintf(fp1, "VERSION,%d\r\n", ver);//属性
		fprintf(fp1, "COLISIONSIZE,%f,%f,%f", ColisionSize.x, ColisionSize.y, ColisionSize.z);//rotate
		fclose(fp1);
	}
}

void MapEdit::AdjustmentObj(std::unique_ptr<MapEdit>& obj_)
{
	//imgui用の変数作成
	static XMFLOAT3 slider_position;
	static XMFLOAT3 slider_scale;
	static XMFLOAT3 slider_rotate;
	static XMFLOAT3 slider_colisionsize;
	//imguiの変数に現在のOBJのデータ代入
	slider_position = obj_->GetPosition();
	slider_scale = obj_->GetScale();
	slider_rotate = obj_->GetRotation();
	slider_colisionsize = obj_->GetColisionSize();

	//Objの情報調整
	//position
	ImGui::InputFloat3("POSITION",&slider_position.x);
	//scale
	ImGui::InputFloat3("SCALE", &slider_scale.x);
	//rotation
	ImGui::InputFloat3("ROTATION", &slider_rotate.x);
	//colision
	ImGui::InputFloat3("COLISION", &slider_colisionsize.x);

	


	//押されたらOBJを削除
	if (ImGui::Button("Delete"))
	{
		obj_->SetDeadFlag(true);
	}

	
	//変更した値を渡す
	obj_->SetPosition(slider_position);
	obj_->SetScale(slider_scale);
	obj_->SetRotate(slider_rotate);
	obj_->SetColisionSize(slider_colisionsize);	
}

void MapEdit::MapSave(std::unique_ptr<MapEdit>& obj_)
{
	//情報を登録(txtに書き込み)
	//modelnum,pos,scale,rotation,colisionsize順
	WriteSet(obj_->GetFbxmodelType(), obj_->GetVer(),obj_->GetPosition(), obj_->GetScale(), obj_->GetRotation(), obj_->GetColisionSize());
}

void MapEdit::LoadSet(ID3D12GraphicsCommandList* cmdList)
{
	//１行分の文字列を入れる変数
	std::string line;

	//各情報宣言
	int type;
	int ver;
	XMFLOAT3 pos{};
	XMFLOAT3 scale{};
	XMFLOAT3 rotate{};
	XMFLOAT3 ColisionSize{};

	//コマンド実行ループ
	while (getline(MapCommands, line)) {
		//1行分の文字列をっストリーム変換して解析
		std::istringstream line_stream(line);

		std::string word;

		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//skip
			continue;
		}

		if (word.find("\r\n") == 0) {
			//skip
			continue;
		}
		//ポジション

		if (word.find("TYPE") == 0)
		{
			getline(line_stream, word, ',');
			type = (float)std::atof(word.c_str());
		}
		else if (word.find("VERSION") == 0)
		{
			getline(line_stream, word, ',');
			ver = (float)std::atof(word.c_str());
		}
		else if (word.find("POSITION") == 0) {
			//pos.x
			getline(line_stream, word, ',');
			pos.x = (float)std::atof(word.c_str());

			//pos.y
			getline(line_stream, word, ',');
			pos.y = (float)std::atof(word.c_str());

			//pos.z
			getline(line_stream, word, ',');
			pos.z = (float)std::atof(word.c_str());

		}
		else if (word.find("SCALE") == 0) {
			//scale.x
			getline(line_stream, word, ',');
			scale.x = (float)std::atof(word.c_str());

			//scale.y
			getline(line_stream, word, ',');
			scale.y = (float)std::atof(word.c_str());

			//scale.z
			getline(line_stream, word, ',');
			scale.z = (float)std::atof(word.c_str());

			

		}
		else if (word.find("ROTATE") == 0) {
			//rotate.x
			getline(line_stream, word, ',');
			rotate.x = (float)std::atof(word.c_str());

			//rotate.y
			getline(line_stream, word, ',');
			rotate.y = (float)std::atof(word.c_str());

			//rotate.z
			getline(line_stream, word, ',');
			rotate.z = (float)std::atof(word.c_str());

	

		}
		else if (word.find("COLISIONSIZE") == 0) {
			//colision.x
			getline(line_stream, word, ',');
			ColisionSize.x = (float)std::atof(word.c_str());

			//colision.y
			getline(line_stream, word, ',');
			ColisionSize.y = (float)std::atof(word.c_str());

			//colision.z
			getline(line_stream, word, ',');
			ColisionSize.z = (float)std::atof(word.c_str());



			//集めた情報をここでOBJの中に収納していく
			//ここでOBJを生成する（モデルや座標の情報をwhile文で取るためここでは入れ物だけ）
            //生成

			//タイプが０だとスポーン地点なので飛ばす
			if (type != 0)
			{
				std::unique_ptr<MapEdit>mapObj = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
				//初期化
				mapObj->Initialize();
				//type1ならフェンスモデルをセット
				if (type == 1)
				{
					//ここでモデルの形状をセット
					mapObj->SetModel(fence_model);
					//モデルを指定
					mapObj->SetFbxmodelType(Fence_model_num);
				}
				else if (type == 2)
				{
					//ここでモデルの形状をセット
					mapObj->SetModel(wall_model);
					//モデルを指定
					mapObj->SetFbxmodelType(Wall_model_num);
				}
				else if (type == 3)
				{
					//ここでモデルの形状をセット
					mapObj->SetModel(floor_model);
					//モデルを指定
					mapObj->SetFbxmodelType(Floor_model_num);
				}
				else if (type == 4)
				{
					//ここでモデルの形状をセット
					mapObj->SetModel(drum_model);
					//モデルを指定
					mapObj->SetFbxmodelType(Drum_model_num);
				}
				else if (type == 5)
				{
					//ここでモデルの形状をセット
					mapObj->SetModel(exit_model);
					//モデルを指定
					mapObj->SetFbxmodelType(Exit_model_num);
				}
				else if (type == 6)
				{
					//ここでモデルの形状をセット
					mapObj->SetModel(gun_model);
					//モデルを指定
					mapObj->SetFbxmodelType(Gun_model_num);
				}
				else if (type == 7)
				{
					//ここでモデルの形状をセット
					mapObj->SetModel(box_model);
					//モデルを指定
					mapObj->SetFbxmodelType(Box_model_num);
				}




				//座標をセット
				mapObj->SetPosition(pos);
				//スケールをセット
				mapObj->SetScale(scale);
				//回転をセット
				mapObj->SetRotate(rotate);
				//colisionsize登録
				mapObj->SetColisionSize(ColisionSize);
		
				//verで属性を決める
				if (ver == 0)
				{
					//当たり判定のサイズ、形、属性を指定
					mapObj->SetColider(new WallCollider(XMVECTOR{ ColisionSize.x,ColisionSize.y,ColisionSize.z,0 }));
					mapObj->SetVerWall();
					
				}
				else if (ver == 1)
				{
					//当たり判定のサイズ、形、属性を指定
					mapObj->SetColider(new WallCollider(XMVECTOR{ ColisionSize.x,ColisionSize.y,ColisionSize.z,0 }));
					mapObj->SetVerExit();
					
				}
				else if (ver == 2)
				{
					//当たり判定のサイズ、形、属性を指定
					mapObj->SetColider(new SphereCollider(XMVECTOR{ 0,5,0,0 }, 5));
					mapObj->SetVerObj();
				}
				else if (ver == 3)
				{
					//当たり判定のサイズ、形、属性を指定
					mapObj->SetColider(new SphereCollider(XMVECTOR{ 0,5,0,0 }, 5));
					mapObj->SetVerObj2();
				}
				//作るのに必要な情報が揃ったので登録
				//登録
				mapObjs.push_back(std::move(mapObj));
			}
			else
			{
				//typeが０なら自機の初期リスなので変数に代入
				Respawn_position = pos;
			}
		}
	}



	//txtから読み込んだ情報をもとに更新、録画
	for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs) {
		//mapObj_->Update();
		
		if (mapObj_->GetFbxmodelType() != 6 && mapObj_->GetFbxmodelType() != 7)
		{
			mapObj_->Update();
		}
		else if (mapObj_->GetFbxmodelType() == 6)
		{
			//必要な情報をセットしていく
			mapObj_->SetMyPosition(MyPosition);
			mapObj_->SetCameraAxisZ(CammeraZAxis);
			mapObj_->SetTarget(Target);

			mapObj_->SetShotBlue(Shot);
			mapObj_->SetShotRed(Shot2);
			mapObj_->SetWark(wark);

			//Gunを持ったかのフラグを代入
			Cursor_on = mapObj_->Getgetobj();

			mapObj_->ObjUpdate(0, 0);
		}
		else if (mapObj_->GetFbxmodelType() == 7)
		{
			//必要な情報をセットしていく
			mapObj_->SetMyPosition(MyPosition);
			mapObj_->SetCameraAxisZ(CammeraZAxis);
			mapObj_->SetTarget(Target);


			mapObj_->BoxObjUpdate(0, 0);
		}
		
		mapObj_->Draw2(cmdList);
	}
}

void MapEdit::Loadtxt()
{
	//ファイルを開く
	std::ifstream file;
	file.open(L"GameOriginal/MapEdit/STAGE2.txt");
	assert(file.is_open());

	//ファイルの内容をコピー
	MapCommands << file.rdbuf();
	//ファイルを閉じる
	file.close();

}

void MapEdit::CreateCollisionBox()
{
	//生成
	std::unique_ptr<MapEdit>Collision = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
	//初期化
	Collision->Initialize();
	//ここでモデルの形状をセット
	Collision->SetModel(Collision_model);
	//スケールを標準設定
	Collision->SetScale({ 1,1,1 });
	//登録
	Collisions.push_back(std::move(Collision));
}

void MapEdit::UpdateCollisionBox(XMFLOAT3 scale_,XMFLOAT3 pos_,XMFLOAT3 rotate_, std::unique_ptr<MapEdit>& Colobj_)
{
	XMFLOAT3 scale;
	//コリジョンのサイズを調整
	scale.x = scale_.x / 100;
	scale.y = scale_.y / 100;
	scale.z = scale_.z / 100;

	Colobj_->SetPosition(pos_);
	Colobj_->SetScale(scale);

	Colobj_->Update();
}

void MapEdit::ObjSelect(std::unique_ptr<MapEdit>& obj_, XMFLOAT2 mouse)
{
	
	XMMATRIX viewPort = {
		1280.0f / 2  ,0.0f,0.0f,0.0f,
		0.0f,-720.0f / 2  ,0.0f,0.0f,
		0.0f,0.0f         ,1.0f, 0.0f,
		1280.0f / 2,  720.0f/2,0.0f,1.0f
	};

	 XMMATRIX matViewProjection =
		camera->GetViewProjectionMatrix()*viewPort;

	//逆行列を計算
	XMMATRIX matInverseVPV= XMMatrixInverse(nullptr,matViewProjection);

	//スクリーン座標
	XMVECTOR posNear = XMVECTOR{ mouse.x,mouse.y,0};
	XMVECTOR posFar =  XMVECTOR{ mouse.x,mouse.y,1};

	//スクリーン座標からワールド座標へ
	posNear = DirectX::XMVector3TransformCoord(posNear, matInverseVPV);
	posFar= DirectX::XMVector3TransformCoord(posFar, matInverseVPV);

	//マウスレイの方向
	XMVECTOR mouseDirection = DirectX::XMVectorSubtract(posFar, posNear);

	//レイの方向を正規化
	mouseDirection = DirectX::XMVector3Normalize(mouseDirection);

	//カメラから照準オブジェクトの距離
	const float kDistance = 10000;

	//当たり判定用のレイを作成
	Ray mouse_ray;
	mouse_ray.start = posNear;
	mouse_ray.dir = mouseDirection;
	RaycastHit raycastHit;

	
	// 交差判定
	XMFLOAT3 Min_pos;
	XMFLOAT3 Max_pos;
	XMFLOAT3 pos2;
	Min_pos = obj_->GetPosition();
	Max_pos = obj_->GetPosition();
	pos2 = obj_->GetColisionSize();

	//四角の当たり判定の位置最大値、最低値設定
	Min_pos.x = Min_pos.x - pos2.x;
	Min_pos.y = Min_pos.y - pos2.y;
	Min_pos.z = Min_pos.z - pos2.z;

	Max_pos.x = Max_pos.x + pos2.x;
	Max_pos.y = Max_pos.y + pos2.y;
	Max_pos.z = Max_pos.z + pos2.z;

	//計算用変数作成
	double tmin = -INFINITY, tmax = INFINITY;
	double tmin2 = -INFINITY, tmax2 = INFINITY;
	double tmin3 = -INFINITY, tmax3 = INFINITY;
	double tmin4 = -INFINITY, tmax4 = INFINITY;

	//X軸
	double tx1 = (Min_pos.x - mouse_ray.start.m128_f32[0]) / mouse_ray.dir.m128_f32[0];
	double tx2 = (Max_pos.x - mouse_ray.start.m128_f32[0]) / mouse_ray.dir.m128_f32[0];
	
	tmin = max(tmin, min(tx1, tx2));
	tmax = min(tmax, max(tx1, tx2));
	//Y軸
	double ty1 = (Min_pos.y - mouse_ray.start.m128_f32[1]) / mouse_ray.dir.m128_f32[1];
	double ty2 = (Max_pos.y - mouse_ray.start.m128_f32[1]) / mouse_ray.dir.m128_f32[1];

	tmin2 = max(tmin, min(ty1, ty2));
	tmax2 = min(tmax, max(ty1, ty2));
	//Z軸
	double tz1 = (Min_pos.z - mouse_ray.start.m128_f32[2]) / mouse_ray.dir.m128_f32[2];
	double tz2 = (Max_pos.z - mouse_ray.start.m128_f32[2]) / mouse_ray.dir.m128_f32[2];

	tmin3 = max(tmin, min(tz1, tz2));
    tmax3 = min(tmax, max(tz1, tz2));
	
	tmin4 = max(tmin, min(tz1, tz2));
	tmax4 = min(tmax, max(tz1, tz2));

	//当たり判定の部分にマウスポイントのレイが当たっていた場合情報をPICKUPする
	if (tmax2 >= tmin2 && tmax3 >= tmin3 && tmax4 >= tmin4 && input->TriggerMouseRight())
	{
		obj_->SetSelectFlag(true);
	}
	else if (!(tmax2 >= tmin2 && tmax3 >= tmin3 && tmax4 >= tmin4) && input->TriggerMouseRight())
	{
		obj_->SetSelectFlag(false);
	}
}

void MapEdit::CreateSpawn()
{
	//まだ一度も生成していないならfalseなので生成する
	if (Respawn_model_create_flag == false)
	{
		//生成
		std::unique_ptr<MapEdit>respawn = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//初期化
		respawn->Initialize();
		//ここでモデルの形状をセット
		respawn->SetModel(respawn_model);
		//スケールを標準設定
		respawn->SetScale({ 0.1,0.1,0.1 });
		//ポジション初期化
		respawn->SetPosition({ 0.0,0.0,-20.0 });
		//登録
		Respawns.push_back(std::move(respawn));
		//フラグをtrueにして生成を止める
		Respawn_model_create_flag = true;
	}

	//IMGUIでの調整用
	for (std::unique_ptr<MapEdit>& respawn_:Respawns) {

		//ツリーに格納
		if (ImGui::TreeNode("Spawn point")) {
			//imgui用の変数作成
			static XMFLOAT3 slider_position;
			//imguiの変数に現在のOBJのデータ代入
			slider_position = respawn_->GetPosition();
			//Objの情報調整
			//position
			ImGui::InputFloat3("POSITION", &slider_position.x);

			//変更した値を渡す
			respawn_->SetPosition(slider_position);
			ImGui::TreePop();
		}
		//Fbxの更新処理
		respawn_->Update();
		//OBJの単独PICKUP
		respawn_->ObjSelect(respawn_, mouse);
	}
}

void MapEdit::CopyModelInfo(int type, int ver, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize)
{
	//渡されたObjの情報を中継ぎ用変数に書き込み保存する
	//モデル
	Copy_model_num = type;
	//属性
	Copy_ver = ver;
	//position
	Copy_pos = pos;
	//scale
	Copy_scale = scale;
	//rotation
	Copy_rotate = rotate;
	//コリジョンサイズ
	Copy_colision_size = ColisionSize;
}

void MapEdit::PasteInfo()
{
	//Copy_model_numの変数で読み込むモデルを分岐する
	if (Copy_model_num == 1)
	{
		//生成
		std::unique_ptr<MapEdit>fence = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//初期化
		fence->Initialize();
		//ここでモデルの形状をセット
		fence->SetModel(fence_model);
		//コリジョンのサイズ登録(初期は０)
		fence->SetColisionSize(Copy_colision_size);
		//スケールのサイズ
		fence->SetScale(Copy_scale);
		//ポジションの初期位置設定
		fence->SetPosition(Copy_pos);
		//回転
		fence->SetRotate(Copy_rotate);
		//モデルを指定
		fence->SetFbxmodelType(Fence_model_num);
		//ver指定
		fence->SetFbxVer(null_ver);
		//登録
		Fences.push_back(std::move(fence));

		//当たり判定確認用OBJ生成
		CreateCollisionBox();
	}
	else if (Copy_model_num == 2)
	{
		//生成
		std::unique_ptr<MapEdit>wall = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//初期化
		wall->Initialize();
		//ここでモデルの形状をセット
		wall->SetModel(wall_model);
		//コリジョンのサイズ登録(初期は０)
		wall->SetColisionSize(Copy_colision_size);
		//スケールのサイズ
		wall->SetScale(Copy_scale);
		//ポジションの初期位置設定
		wall->SetPosition(Copy_pos);
		//回転
		wall->SetRotate(Copy_rotate);
		//モデルを指定
		wall->SetFbxmodelType(Wall_model_num);
		//ver指定
		wall->SetFbxVer(null_ver);
		//登録
		Walls.push_back(std::move(wall));

		//当たり判定確認用OBJ生成
		CreateCollisionBox();
	}
	else if (Copy_model_num == 3)
	{
		//生成
		std::unique_ptr<MapEdit>floor = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//初期化
		floor->Initialize();
		//ここでモデルの形状をセット
		floor->SetModel(floor_model);
		//コリジョンのサイズ登録(初期は０)
		floor->SetColisionSize(Copy_colision_size);
		//スケールのサイズ
		floor->SetScale(Copy_scale);
		//ポジションの初期位置設定
		floor->SetPosition(Copy_pos);
		//回転
		floor->SetRotate(Copy_rotate);
		//モデルを指定
		floor->SetFbxmodelType(Floor_model_num);
		//ver指定
		floor->SetFbxVer(null_ver);
		//登録
		Floors.push_back(std::move(floor));

		//当たり判定確認用OBJ生成
		CreateCollisionBox();
	}
	else if (Copy_model_num == 4)
	{
		//生成
		std::unique_ptr<MapEdit>drum = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//初期化
		drum->Initialize();
		//ここでモデルの形状をセット
		drum->SetModel(drum_model);
		//コリジョンのサイズ登録(初期は０)
		drum->SetColisionSize(Copy_colision_size);
		//スケールのサイズ
		drum->SetScale(Copy_scale);
		//ポジションの初期位置設定
		drum->SetPosition(Copy_pos);
		//回転
		drum->SetRotate(Copy_rotate);
		//モデルを指定
		drum->SetFbxmodelType(Drum_model_num);
		//ver指定
		drum->SetFbxVer(null_ver);
		//登録
		Drums.push_back(std::move(drum));

		//当たり判定確認用OBJ生成
		CreateCollisionBox();
	}
	else if (Copy_model_num == 5)
	{
		//生成
		std::unique_ptr<MapEdit>exit = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//初期化
		exit->Initialize();
		//ここでモデルの形状をセット
		exit->SetModel(exit_model);
		//コリジョンのサイズ登録(初期は０)
		exit->SetColisionSize(Copy_colision_size);
		//スケールのサイズ
		exit->SetScale(Copy_scale);
		//ポジションの初期位置設定
		exit->SetPosition(Copy_pos);
		//回転
		exit->SetRotate(Copy_rotate);
		//モデルを指定
		exit->SetFbxmodelType(Exit_model_num);
		//ver指定
		exit->SetFbxVer(exit_ver);
		//登録
		Exits.push_back(std::move(exit));

		//当たり判定確認用OBJ生成
		CreateCollisionBox();
	}
	else
	{
	assert(1);
	}
}

void MapEdit::ArowCreate()
{
	//フラグがfalseならまだ生成していないので生成する
	if (Arow_create_flag == false)
	{
		for (int i = 0; i < 3; i++)
		{
			//生成
			std::unique_ptr<MapEdit>arow = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//初期化
			arow->Initialize();

			//iでモデルを分岐する(0=R,1=G,2=B)
			if (i == 0)
			{
				//ここでモデルの形状をセット
				arow->SetModel(Red_arow_model);
				//モデルを指定
				arow->SetFbxmodelType(Red_arow_model_num);
				//回転
				arow->SetRotate({ 0,180,0 });
				//コリジョンのサイズ登録(初期は０)
				arow->SetColisionSize({ 10,1,1 });
			}
			else if(i==1)
			{
				//ここでモデルの形状をセット
				arow->SetModel(Green_arow_model);
				//モデルを指定
				arow->SetFbxmodelType(Green_arow_model_num);
				//回転
				arow->SetRotate({ 0,0,270 });
				//コリジョンのサイズ登録(初期は０)
				arow->SetColisionSize({ 1,10,1 });
			}
			else if (i == 2)
			{
				//ここでモデルの形状をセット
				arow->SetModel(Blue_arow_model);
				//モデルを指定
				arow->SetFbxmodelType(Blue_arow_model_num);
				//回転
				arow->SetRotate({ 0,90,0 });
				//コリジョンのサイズ登録(初期は０)
				arow->SetColisionSize({ 1,1,10 });
			}
			
			//スケールのサイズ
			arow->SetScale({0.1,0.05,0.05});
			//ポジションの初期位置設定
			arow->SetPosition({0,0,0});
		
			//ver指定
			arow->SetFbxVer(null_ver);
			//登録
			Arows.push_back(std::move(arow));
		}
		//生成の処理が終わったらフラグをtrueにする
		Arow_create_flag = true;
	}


}

void MapEdit::ArowUpdate(std::unique_ptr<MapEdit>& obj_, XMFLOAT2 mouse)
{
	for (std::unique_ptr<MapEdit>& arow_ : Arows) {

		//起点の位置はPickUpObjを基準に色ごとに矢印の位置を調整する
		if (arow_->GetFbxmodelType() == -1)
		{
			arow_->SetPosition({ Arow_pos.x+10,Arow_pos.y,Arow_pos.z });
		}
		else if (arow_->GetFbxmodelType() == -2)
		{
			arow_->SetPosition({ Arow_pos.x,Arow_pos.y+10,Arow_pos.z });
		}
		else if (arow_->GetFbxmodelType() == -3)
		{
			arow_->SetPosition({ Arow_pos.x,Arow_pos.y,Arow_pos.z+10 });
		}

		//描画されていたら当たり判定ON
		if (Arow_draw_flag == true)
		{
			arow_->ArowCollision(obj_,arow_,mouse);
		}
		//FBXの更新処理
		arow_->Update();
	}
}

void MapEdit::ArowCollision(std::unique_ptr<MapEdit>& obj_2, std::unique_ptr<MapEdit>& obj_, XMFLOAT2 mouse)
{

	XMMATRIX viewPort = {
		1280.0f / 2  ,0.0f,0.0f,0.0f,
		0.0f,-720.0f / 2  ,0.0f,0.0f,
		0.0f,0.0f         ,1.0f, 0.0f,
		1280.0f / 2,  720.0f / 2,0.0f,1.0f
	};

	XMMATRIX matViewProjection =
		camera->GetViewProjectionMatrix() * viewPort;

	//逆行列を計算
	XMMATRIX matInverseVPV = XMMatrixInverse(nullptr, matViewProjection);

	//スクリーン座標
	XMVECTOR posNear = XMVECTOR{ mouse.x,mouse.y,0 };
	XMVECTOR posFar = XMVECTOR{ mouse.x,mouse.y,1 };

	//スクリーン座標からワールド座標へ
	posNear = DirectX::XMVector3TransformCoord(posNear, matInverseVPV);
	posFar = DirectX::XMVector3TransformCoord(posFar, matInverseVPV);

	//マウスレイの方向
	XMVECTOR mouseDirection = DirectX::XMVectorSubtract(posFar, posNear);

	//レイの方向を正規化
	mouseDirection = DirectX::XMVector3Normalize(mouseDirection);

	//カメラから照準オブジェクトの距離
	const float kDistance = 10000;

	//当たり判定用のレイを作成
	Ray mouse_ray;
	mouse_ray.start = posNear;
	mouse_ray.dir = mouseDirection;
	RaycastHit raycastHit;


	// 交差判定
	XMFLOAT3 Min_pos;
	XMFLOAT3 Max_pos;
	XMFLOAT3 pos2;
	Min_pos = obj_->GetPosition();
	Max_pos = obj_->GetPosition();
	pos2 = obj_->GetColisionSize();

	//四角の当たり判定の位置最大値、最低値設定
	Min_pos.x = Min_pos.x - pos2.x;
	Min_pos.y = Min_pos.y - pos2.y;
	Min_pos.z = Min_pos.z - pos2.z;

	Max_pos.x = Max_pos.x + pos2.x;
	Max_pos.y = Max_pos.y + pos2.y;
	Max_pos.z = Max_pos.z + pos2.z;

	//計算用変数作成
	double tmin = -INFINITY, tmax = INFINITY;
	double tmin2 = -INFINITY, tmax2 = INFINITY;
	double tmin3 = -INFINITY, tmax3 = INFINITY;
	double tmin4 = -INFINITY, tmax4 = INFINITY;

	//X軸
	double tx1 = (Min_pos.x - mouse_ray.start.m128_f32[0]) / mouse_ray.dir.m128_f32[0];
	double tx2 = (Max_pos.x - mouse_ray.start.m128_f32[0]) / mouse_ray.dir.m128_f32[0];

	tmin = max(tmin, min(tx1, tx2));
	tmax = min(tmax, max(tx1, tx2));
	//Y軸
	double ty1 = (Min_pos.y - mouse_ray.start.m128_f32[1]) / mouse_ray.dir.m128_f32[1];
	double ty2 = (Max_pos.y - mouse_ray.start.m128_f32[1]) / mouse_ray.dir.m128_f32[1];

	tmin2 = max(tmin, min(ty1, ty2));
	tmax2 = min(tmax, max(ty1, ty2));
	//Z軸
	double tz1 = (Min_pos.z - mouse_ray.start.m128_f32[2]) / mouse_ray.dir.m128_f32[2];
	double tz2 = (Max_pos.z - mouse_ray.start.m128_f32[2]) / mouse_ray.dir.m128_f32[2];

	tmin3 = max(tmin, min(tz1, tz2));
	tmax3 = min(tmax, max(tz1, tz2));

	tmin4 = max(tmin, min(tz1, tz2));
	tmax4 = min(tmax, max(tz1, tz2));

	//昔のマウスの座標との差で移動量を検出
	XMFLOAT2 move;
	move.x = Old_mouse_move.x - mouse.x;
	move.y = Old_mouse_move.y - mouse.y;

	//そのままだと高感度なので半分に調整
	move.x = move.x * 0.5;
	move.y = move.y * 0.5;

	//当たり判定の部分にマウスポイントのレイが当たっていた場合情報をPICKUPする
	if (tmax2 >= tmin2 && tmax3 >= tmin3 && tmax4 >= tmin4 && input->PushMouseMid())
	{
		Arow_ivent_flag = true;

		//位置の移動を重複させないためにTriggerで分岐させる
		if (input->TriggerMouseMid())
		{
			//矢印の向きで分岐
			if (obj_->GetFbxmodelType() == -1)
			{
				Arow_ivent_fixed = 1;
			}
			else if (obj_->GetFbxmodelType() == -2)
			{
				Arow_ivent_fixed = 2;
			}
			else if (obj_->GetFbxmodelType() == -3)
			{
				Arow_ivent_fixed = 3;
			}
		}
	}
	
	//キーが押されていたらマウスの移動量によってOBJを移動させる
	if (Arow_ivent_flag == true && input->PushMouseMid())
	{
		//pos
		if (Arow_change_content == 1)
		{
			//元の位置を取得
			XMFLOAT3 Origin_obj_pos = obj_2->GetPosition();

			//移動量によって位置を加算していく
			if (obj_->GetFbxmodelType() == -1 && Arow_ivent_fixed == 1)
			{

				Origin_obj_pos.x -= move.x;
				obj_2->SetPosition(Origin_obj_pos);
			}

			if (obj_->GetFbxmodelType() == -2 && Arow_ivent_fixed == 2)
			{
				Origin_obj_pos.y += move.y;
				obj_2->SetPosition(Origin_obj_pos);
			}

			if (obj_->GetFbxmodelType() == -3 && Arow_ivent_fixed == 3)
			{
				Origin_obj_pos.z -= move.x;
				obj_2->SetPosition(Origin_obj_pos);
			}
		}
		else if (Arow_change_content == 2)//scale
		{
			//元の位置を取得
			XMFLOAT3 Origin_obj_scale = obj_2->GetScale();

			//移動量によって位置を加算していく
			if (obj_->GetFbxmodelType() == -1 && Arow_ivent_fixed == 1)
			{

				Origin_obj_scale.x -= move.x*0.5;
				obj_2->SetScale(Origin_obj_scale);
			}

			if (obj_->GetFbxmodelType() == -2 && Arow_ivent_fixed == 2)
			{
				Origin_obj_scale.y += move.y*0.5;
				obj_2->SetScale(Origin_obj_scale);
			}

			if (obj_->GetFbxmodelType() == -3 && Arow_ivent_fixed == 3)
			{
				Origin_obj_scale.z -= move.x*0.5;
				obj_2->SetScale(Origin_obj_scale);
			}
		}
	}




	else if(!(input->PushMouseMid()))
	{
		//キーが離されたら移動をそこで中止させる
		Arow_ivent_flag = false;
		Arow_ivent_fixed = 0;
	}

	//マウスの座標を保存
	Old_mouse_move = mouse;
}
