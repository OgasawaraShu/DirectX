#include "MapLoader.h"
#include <fstream>


void MapLoader::LoadSet(int type, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize)
{
	//ファイルを開く
	std::ifstream file;
	file.open("STAGE2.txt");
	assert(file.is_open());

	//ファイルの内容をコピー
	MapCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();

	//１行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(MapCommands, line)) {
		//1行分の文字列をっストリーム変換して解析
		std::istringstream line_stream(line);

		std::string word;

		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") ==0 ) {
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
			//rotate.x
			getline(line_stream, word, ',');
			ColisionSize.x = (float)std::atof(word.c_str());

			//rotate.y
			getline(line_stream, word, ',');
			ColisionSize.y = (float)std::atof(word.c_str());

			//rotate.z
			getline(line_stream, word, ',');
			ColisionSize.z = (float)std::atof(word.c_str());

			//ぬく
			break;
		}
	}


}
