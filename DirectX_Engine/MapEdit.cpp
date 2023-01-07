#include "MapEdit.h"
#include "Wallcolider.h"
#include <imgui.h>
#include <fstream>


MapEdit::MapEdit(int window_width, int window_height, Input* input)
	:Fbx3d(input)
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
	//キーを押して生成
	//imgui関連
	ImGui::Begin("Option");
	ImGui::SetWindowPos(ImVec2(700, 0));
	ImGui::SetWindowSize(ImVec2(500, 200));

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
			fence->SetColisionSize({ 0,0,0 });
			//モデルを指定
			fence->SetFbxmodelType(Fence_model_num);
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
			drum->SetColisionSize({ 0,0,0 });
			//モデルを指定
			drum->SetFbxmodelType(Drum_model_num);
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
			//モデルを指定
			floor->SetFbxmodelType(Floor_model_num);
			//登録
			Floors.push_back(std::move(floor));

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
	}

	

	ImGui::End();

	//各OBJの情報の調整処理
	//更新
	//imgui関連
	ImGui::Begin("Obj_Data");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 200));

	
	//Objのカウント用
	int Fence_count = 1;
	int Drum_count = 1;
	int Wall_count = 1;
	int Floor_count = 1;

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
		sprintf_s(str2, "%d",Fence_count );
		//文字列を結合
		strcat_s(str, str2);
		//ツリーに格納
		if (ImGui::TreeNode(str)) {
			fence_->AdjustmentObj(fence_);
			ImGui::TreePop();
		}
		//Fbxの更新処理
		fence_->Update();
	
		//for文で１増やす
		Fence_count += 1;

		pos[Collision_count] = { fence_->GetPosition() };
		rotate[Collision_count] = { fence_->GetRotation() };
		colision_size[Collision_count] = { fence_->GetColisionSize()};
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
		char str[1024] = { 'F','l','o','o','r','_'};
		//番号割りあて
		char str2[1024];
		//intをstringに変換
		sprintf_s(str2, "%d", Floor_count);
		//文字列を結合
		strcat_s(str, str2);
		//ツリーに格納
		if (ImGui::TreeNode(str)) {
			floor_->AdjustmentObj(floor_);
			ImGui::TreePop();
		}
		//Fbxの更新処理
		floor_->Update();

		//for文で１増やす
		Floor_count += 1;

		pos[Collision_count] = { floor_->GetPosition() };
		rotate[Collision_count] = { floor_->GetRotation() };
		colision_size[Collision_count] = { floor_->GetColisionSize() };
		Collision_count += 1;
	}
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

	
}

void MapEdit::WriteSet(int type, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize)
{
	FILE* fp1;
	//テキスト暗号化した状態で保存
	fp1 = _fsopen("MapEdit/STAGE2.txt", "a", _SH_DENYNO);

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
	WriteSet(obj_->GetFbxmodelType(), obj_->GetPosition(), obj_->GetScale(), obj_->GetRotation(), obj_->GetColisionSize());
}

void MapEdit::LoadSet(ID3D12GraphicsCommandList* cmdList)
{
	//１行分の文字列を入れる変数
	std::string line;

	//各情報宣言
	int type;
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
			std::unique_ptr<MapEdit>mapObj = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//初期化
			mapObj->Initialize();
			//type1ならフェンスモデルをセット
			if (type == 1)
			{
				//ここでモデルの形状をセット
				mapObj->SetModel(fence_model);
			}
			else if (type == 2)
			{
				//ここでモデルの形状をセット
				mapObj->SetModel(wall_model);
			}
			else if (type == 3)
			{
				//ここでモデルの形状をセット
				mapObj->SetModel(floor_model);
			}
			else if (type == 4)
			{
				//ここでモデルの形状をセット
				mapObj->SetModel(drum_model);
			}

			//座標をセット
			mapObj->SetPosition(pos);
			//スケールをセット
			mapObj->SetScale(scale);
			//回転をセット
			mapObj->SetRotate(rotate);
			//colisionsize登録
			mapObj->SetColisionSize(ColisionSize);
			//当たり判定のサイズ、形、属性を指定
			mapObj->SetColider(new WallCollider(XMVECTOR{ ColisionSize.x,ColisionSize.y,ColisionSize.z,0 }));
			mapObj->SetVerWall();
			//作るのに必要な情報が揃ったので登録
			//登録
			mapObjs.push_back(std::move(mapObj));
		}
	}



	//txtから読み込んだ情報をもとに更新、録画
	for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs) {

		mapObj_->Update();
		mapObj_->Draw2(cmdList);
	}
}

void MapEdit::Loadtxt()
{
	//ファイルを開く
	std::ifstream file;
	file.open("MapEdit/STAGE2.txt");
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

	scale.x = scale_.x / 100;
	scale.y = scale_.y / 100;
	scale.z = scale_.z / 100;

	Colobj_->SetPosition(pos_);
//	Colobj_->SetRotate(rotate_);
	Colobj_->SetScale(scale);

	Colobj_->Update();
}
