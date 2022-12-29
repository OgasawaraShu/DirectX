#include "MapEdit.h"
#include <fstream>


MapEdit::MapEdit(int window_width, int window_height, Input* input)
	:Fbx3d(input)
{
	this->input = input;
	this->Window_Width = window_width;
	this->Window_Height = window_height;

	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void MapEdit::MapInitialize()
{

}

void MapEdit::CreateObj(ID3D12GraphicsCommandList* cmdList)
{
	//�L�[�������Đ���

	//�t�F���X���f������
	if (input->TriggerKey(DIK_1))
	{
		//����
		std::unique_ptr<MapEdit>fence = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//������
		fence->Initialize();
		//�����Ń��f���̌`����Z�b�g
		fence->SetModel(fence_model);
		//�R���W�����̃T�C�Y�o�^(�����͂O)
		fence->SetColisionSize({ 0,0,0 });
		//�o�^
		Fences.push_back(std::move(fence));
		
	}


	//�eOBJ�̏��̒�������
	//�X�V
	for (std::unique_ptr<MapEdit>& fence_ : Fences) {
		fence_->AdjustmentObj(fence_);
	}
	
	//�`��
	for (std::unique_ptr<MapEdit>& fence_ : Fences) {
		fence_->Draw2(cmdList);
	}

	//�f�[�^�ۑ��i���j
	if (input->TriggerKey(DIK_P))
	{
		//�����L�^
		for (std::unique_ptr<MapEdit>& fence_ : Fences) {
			fence_->MapSave(fence_);
		}
	}
}

void MapEdit::WriteSet(int type, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize)
{
	FILE* fp1;
	//�e�L�X�g�Í���������Ԃŕۑ�
	fp1 = _fsopen("MapEdit/STAGE2.txt", "w", _SH_DENYNO);

	if (fp1 == NULL) {
		assert(0);
	}
	else {
		//file��null����Ȃ��̂Ȃ�e��������
		fprintf(fp1, "\r\n");//���s
		fprintf(fp1, "TYPE,%d\r\n", type);//���f���^�C�v
		fprintf(fp1, "POSITION,%f,%f,%f\r\n", pos.x, pos.y, pos.z);//pos
		fprintf(fp1, "SCALE,%f,%f,%f\r\n", scale.x, scale.y, scale.z);//scale
		fprintf(fp1, "ROTATE,%f,%f,%f\r\n", rotate.x, rotate.y, rotate.z);//rotate
		fprintf(fp1, "COLISIONSIZE,%f,%f,%f\r\n", ColisionSize.x, ColisionSize.y, ColisionSize.z);//rotate
		fclose(fp1);
	}
}

void MapEdit::AdjustmentObj(std::unique_ptr<MapEdit>& obj_)
{
	//Obj�̏�񒲐�
	obj_->SetPosition({ 0,10,0 });//�m�F�p
	//Fbx�̍X�V����
	Update();
}

void MapEdit::MapSave(std::unique_ptr<MapEdit>& obj_)
{
	//����o�^(txt�ɏ�������)
	//pos,scale,rotation,colisionsize��
	WriteSet(1, obj_->GetPosition(), obj_->GetScale(), obj_->GetRotation(), obj_->GetColisionSize());

}

void MapEdit::LoadSet(ID3D12GraphicsCommandList* cmdList)
{


	//�P�s���̕����������ϐ�
	std::string line;



	//�e���錾
	int type;
	XMFLOAT3 pos{};
	XMFLOAT3 scale{};
	XMFLOAT3 rotate{};
	XMFLOAT3 ColisionSize{};


	//������OBJ�𐶐�����i���f������W�̏���while���Ŏ�邽�߂����ł͓��ꕨ�����j
	//����
	std::unique_ptr<MapEdit>mapObj = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
	//������
	mapObj->Initialize();

	//�R�}���h���s���[�v
	while (getline(MapCommands, line)) {
		//1�s���̕���������X�g���[���ϊ����ĉ��
		std::istringstream line_stream(line);

		std::string word;

		//,��؂�ōs�̐擪�������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�s�̓R�����g
		if (word.find("//") == 0) {
			//skip
			continue;
		}
		//�|�W�V����

		if (word.find("TYPE") == 0)
		{
			getline(line_stream, word, ',');
			type = (float)std::atof(word.c_str());

			//type1�Ȃ�t�F���X���f�����Z�b�g
			if (type == 1)
			{
				//�����Ń��f���̌`����Z�b�g
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

			//���W���Z�b�g
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

			//�X�P�[�����Z�b�g
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

			//��]���Z�b�g
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

			//colisionsize�o�^
			mapObj->SetColisionSize(ColisionSize);

			//���̂ɕK�v�ȏ�񂪑������̂œo�^
			//�o�^
			mapObjs.push_back(std::move(mapObj));

			//�ʂ�
			break;
		}
	}

	//txt����ǂݍ��񂾏������ƂɍX�V�A�^��
	for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs) {

		mapObj_->Update();
		mapObj_->Draw2(cmdList);
	}
}

void MapEdit::Loadtxt()
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open("MapEdit/STAGE2.txt");
	assert(file.is_open());

	//�t�@�C���̓��e���R�s�[
	MapCommands << file.rdbuf();

	//�t�@�C�������
	file.close();
}
