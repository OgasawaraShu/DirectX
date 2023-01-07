#include "MapLoader.h"
#include <fstream>


void MapLoader::LoadSet(int type, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize)
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open("STAGE2.txt");
	assert(file.is_open());

	//�t�@�C���̓��e���R�s�[
	MapCommands << file.rdbuf();

	//�t�@�C�������
	file.close();

	//�P�s���̕����������ϐ�
	std::string line;

	//�R�}���h���s���[�v
	while (getline(MapCommands, line)) {
		//1�s���̕���������X�g���[���ϊ����ĉ��
		std::istringstream line_stream(line);

		std::string word;

		//,��؂�ōs�̐擪�������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�s�̓R�����g
		if (word.find("//") ==0 ) {
			//skip
			continue;
		}
		//�|�W�V����

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

			//�ʂ�
			break;
		}
	}


}
