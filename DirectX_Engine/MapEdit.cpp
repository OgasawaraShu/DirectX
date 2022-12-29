#include "MapEdit.h"
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

	//フェンスモデル生成
	if (input->TriggerKey(DIK_1))
	{
		//生成
		std::unique_ptr<MapEdit>fence = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//初期化
		fence->Initialize();
		//ここでモデルの形状をセット
		fence->SetModel(fence_model);
		//コリジョンのサイズ登録(初期は０)
		fence->SetColisionSize({ 0,0,0 });
		//登録
		Fences.push_back(std::move(fence));
		
	}


	//各OBJの情報の調整処理
	//更新
	for (std::unique_ptr<MapEdit>& fence_ : Fences) {
		fence_->AdjustmentObj(fence_);
	}
	
	//描画
	for (std::unique_ptr<MapEdit>& fence_ : Fences) {
		fence_->Draw2(cmdList);
	}

	//データ保存（仮）
	if (input->TriggerKey(DIK_P))
	{
		//情報を記録
		for (std::unique_ptr<MapEdit>& fence_ : Fences) {
			fence_->MapSave(fence_);
		}
	}
}

void MapEdit::WriteSet(int type, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize)
{
	FILE* fp1;
	//テキスト暗号化した状態で保存
	fp1 = _fsopen("MapEdit/STAGE2.txt", "w", _SH_DENYNO);

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
		fprintf(fp1, "COLISIONSIZE,%f,%f,%f\r\n", ColisionSize.x, ColisionSize.y, ColisionSize.z);//rotate
		fclose(fp1);
	}
}

void MapEdit::AdjustmentObj(std::unique_ptr<MapEdit>& obj_)
{
	//Objの情報調整
	obj_->SetPosition({ 0,10,0 });//確認用
	//Fbxの更新処理
	Update();
}

void MapEdit::MapSave(std::unique_ptr<MapEdit>& obj_)
{
	//情報を登録(txtに書き込み)
	//pos,scale,rotation,colisionsize順
	WriteSet(1, obj_->GetPosition(), obj_->GetScale(), obj_->GetRotation(), obj_->GetColisionSize());

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


	//ここでOBJを生成する（モデルや座標の情報をwhile文で取るためここでは入れ物だけ）
	//生成
	std::unique_ptr<MapEdit>mapObj = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
	//初期化
	mapObj->Initialize();

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
		//ポジション

		if (word.find("TYPE") == 0)
		{
			getline(line_stream, word, ',');
			type = (float)std::atof(word.c_str());

			//type1ならフェンスモデルをセット
			if (type == 1)
			{
				//ここでモデルの形状をセット
				mapObj->SetModel(fence_model);
			}



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

			//座標をセット
			mapObj->SetPosition(pos);
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

			//スケールをセット
			mapObj->SetScale(scale);

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

			//回転をセット
			mapObj->SetRotate(rotate);

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

			//colisionsize登録
			mapObj->SetColisionSize(ColisionSize);

			//作るのに必要な情報が揃ったので登録
			//登録
			mapObjs.push_back(std::move(mapObj));

			//ぬく
			break;
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
