#include "MapEdit.h"
#include "Wallcolider.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include "Collision.h"
#include <imgui.h>
#include <fstream>

Camera* MapEdit::M_camera = nullptr;
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
	//�I���������Ⴂ�����Ȃ��t���O��߂�
	Select_obj_flag = false;
	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();


	//�}�E�X���W�擾
	GetCursorPos(&mousePosition);

	//�N���C�A���g���W�ɕϊ�
	ScreenToClient(hwnd, &mousePosition);

	//long�^��float�^�ɕϊ�
	mouse.x = mousePosition.x;
	mouse.y = mousePosition.y;


	//�L�[�������Đ���
	//imgui�֘A
	ImGui::Begin("Option");
	ImGui::SetWindowPos(ImVec2(700, 0));
	ImGui::SetWindowSize(ImVec2(500, 200));

	if (ImGui::TreeNode("Create")) {
		//�t�F���X���f������
		if (ImGui::Button("Fence Create"))
		{
			//����
			std::unique_ptr<MapEdit>fence = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//������
			fence->Initialize();
			//�����Ń��f���̌`����Z�b�g
			fence->SetModel(fence_model);
			//�R���W�����̃T�C�Y�o�^(�����͂O)
			fence->SetColisionSize({ 0,0,0 });
			//���f�����w��
			fence->SetFbxmodelType(Fence_model_num);
			//ver�w��
			fence->SetFbxVer(null_ver);
			//�o�^
			Fences.push_back(std::move(fence));

			//�����蔻��m�F�pOBJ����
			CreateCollisionBox();
		}

		//�h�������f������
		if (ImGui::Button("Drum Create"))
		{
			//����
			std::unique_ptr<MapEdit>drum = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//������
			drum->Initialize();
			//�����Ń��f���̌`����Z�b�g
			drum->SetModel(drum_model);
			//�R���W�����̃T�C�Y�o�^(�����͂O)
			drum->SetColisionSize({ 0,0,0 });
			//���f�����w��
			drum->SetFbxmodelType(Drum_model_num);
			//ver�w��
			drum->SetFbxVer(null_ver);
			//�o�^
			Drums.push_back(std::move(drum));

			//�����蔻��m�F�pOBJ����
			CreateCollisionBox();
		}

		//�ǃ��f������
		if (ImGui::Button("Wall Create"))
		{
			//����
			std::unique_ptr<MapEdit>wall = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//������
			wall->Initialize();
			//�����Ń��f���̌`����Z�b�g
			wall->SetModel(wall_model);
			//�R���W�����̃T�C�Y�o�^(�����͂O)
			wall->SetColisionSize({ 0,0,0 });
			//���f�����w��
			wall->SetFbxmodelType(Wall_model_num);
			//ver�w��
			wall->SetFbxVer(null_ver);
			//�o�^
			Walls.push_back(std::move(wall));

			//�����蔻��m�F�pOBJ����
			CreateCollisionBox();
		}


		//�����f������
		if (ImGui::Button("Floor Create"))
		{
			//����
			std::unique_ptr<MapEdit>floor = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//������
			floor->Initialize();
			//�����Ń��f���̌`����Z�b�g
			floor->SetModel(floor_model);
			//�R���W�����̃T�C�Y�o�^(�����͂O)
			floor->SetColisionSize({ 0,0,0 });
			//���f�����w��
			floor->SetFbxmodelType(Floor_model_num);
			//ver�w��
			floor->SetFbxVer(null_ver);
			//�o�^
			Floors.push_back(std::move(floor));

			//�����蔻��m�F�pOBJ����
			CreateCollisionBox();
		}

		//�o�����f������
		if (ImGui::Button("Exit Create"))
		{
			//����
			std::unique_ptr<MapEdit>exit = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//������
			exit->Initialize();
			//�����Ń��f���̌`����Z�b�g
			exit->SetModel(exit_model);
			//�R���W�����̃T�C�Y�o�^(�����͂O)
			exit->SetColisionSize({ 0,0,0 });
			//���f�����w��
			exit->SetFbxmodelType(Exit_model_num);
			//ver�w��
			exit->SetFbxVer(exit_ver);
			//�o�^
			Exits.push_back(std::move(exit));

			//�����蔻��m�F�pOBJ����
			CreateCollisionBox();
		}

		ImGui::TreePop();
	}

	//�{�^�������ꂽ��R���W�������������邩���Ȃ����ς�����
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


	//�f�[�^�ۑ��i���j
	if (ImGui::Button("Save"))
	{
		//�����L�^
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
	}



	ImGui::End();



	//PIckUp���ꂽObj�̓��e��\��	
	ImGui::Begin("PickUp Obj");
	ImGui::SetWindowPos(ImVec2(0, 580));
	ImGui::SetWindowSize(ImVec2(500, 200));


	for (std::unique_ptr<MapEdit>& fence_ : Fences) {
		if (fence_->GetSelectFlag() == true)
		{
			fence_->AdjustmentObj(fence_);
		}
	}

	for (std::unique_ptr<MapEdit>& drum_ : Drums) {
		if (drum_->GetSelectFlag() == true)
		{
			drum_->AdjustmentObj(drum_);
		}
	}

	for (std::unique_ptr<MapEdit>& walls_ : Walls) {
		if (walls_->GetSelectFlag() == true)
		{
			walls_->AdjustmentObj(walls_);
		}
	}

	for (std::unique_ptr<MapEdit>& floor_ : Floors) {
		if (floor_->GetSelectFlag() == true)
		{
			floor_->AdjustmentObj(floor_);
		}
	}

	for (std::unique_ptr<MapEdit>& exit_ : Exits) {
		if (exit_->GetSelectFlag() == true)
		{
			exit_->AdjustmentObj(exit_);
		}
	}
	ImGui::End();


	//�eOBJ�̏��̒�������
	//�X�V
	//imgui�֘A
	ImGui::Begin("Obj_Data");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 200));


	//Obj�̃J�E���g�p
	int Fence_count = 1;
	int Drum_count = 1;
	int Wall_count = 1;
	int Floor_count = 1;
	int Exit_count = 1;

	//�����蔻������p����ϐ�
	int Collision_count = 0;
	XMFLOAT3 pos[100] = {};
	XMFLOAT3 rotate[100] = {};
	XMFLOAT3 colision_size[100] = {};


	for (std::unique_ptr<MapEdit>& fence_ : Fences) {

		//�폜�t���O�������Ă���̂Ȃ�OBJ���폜
		Fences.remove_if([](std::unique_ptr<MapEdit>& fence_) {
			return fence_->GetDeadFlag();
			});

		//OBJ�̖��O
		char str[1024] = { 'F','e','n','c','e','_' };
		//�ԍ����肠��
		char str2[1024];
		//int��string�ɕϊ�
		sprintf_s(str2, "%d", Fence_count);
		//�����������
		strcat_s(str, str2);
		//�c���[�Ɋi�[
		if (ImGui::TreeNode(str)) {
			fence_->AdjustmentObj(fence_);
			ImGui::TreePop();
		}
		//Fbx�̍X�V����
		fence_->Update();
		//OBJ�̒P��PICKUP
		fence_->ObjSelect(fence_, mouse);

		//for���łP���₷
		Fence_count += 1;

		pos[Collision_count] = { fence_->GetPosition() };
		rotate[Collision_count] = { fence_->GetRotation() };
		colision_size[Collision_count] = { fence_->GetColisionSize() };
		Collision_count += 1;
	}


	for (std::unique_ptr<MapEdit>& drum_ : Drums) {


		//�폜�t���O�������Ă���̂Ȃ�OBJ���폜
		Drums.remove_if([](std::unique_ptr<MapEdit>& drum_) {
			return drum_->GetDeadFlag();
			});

		//OBJ�̖��O
		char str[1024] = { 'D','r','u','m','_' };
		//�ԍ����肠��
		char str2[1024];
		//int��string�ɕϊ�
		sprintf_s(str2, "%d", Drum_count);
		//�����������
		strcat_s(str, str2);
		//�c���[�Ɋi�[
		if (ImGui::TreeNode(str)) {
			drum_->AdjustmentObj(drum_);
			ImGui::TreePop();
		}
		//Fbx�̍X�V����
		drum_->Update();
		//OBJ�̒P��PICKUP
		drum_->ObjSelect(drum_, mouse);


		//for���łP���₷
		Drum_count += 1;

		pos[Collision_count] = { drum_->GetPosition() };
		rotate[Collision_count] = { drum_->GetRotation() };
		colision_size[Collision_count] = { drum_->GetColisionSize() };
		Collision_count += 1;
	}

	for (std::unique_ptr<MapEdit>& wall_ : Walls) {


		//�폜�t���O�������Ă���̂Ȃ�OBJ���폜
		Walls.remove_if([](std::unique_ptr<MapEdit>& wall_) {
			return wall_->GetDeadFlag();
			});

		//OBJ�̖��O
		char str[1024] = { 'W','a','l','l','_' };
		//�ԍ����肠��
		char str2[1024];
		//int��string�ɕϊ�
		sprintf_s(str2, "%d", Wall_count);
		//�����������
		strcat_s(str, str2);
		//�c���[�Ɋi�[
		if (ImGui::TreeNode(str)) {
			wall_->AdjustmentObj(wall_);
			ImGui::TreePop();
		}
		//Fbx�̍X�V����
		wall_->Update();
		//OBJ�̒P��PICKUP
		wall_->ObjSelect(wall_, mouse);


		//for���łP���₷
		Wall_count += 1;

		pos[Collision_count] = { wall_->GetPosition() };
		rotate[Collision_count] = { wall_->GetRotation() };
		colision_size[Collision_count] = { wall_->GetColisionSize() };
		Collision_count += 1;
	}

	for (std::unique_ptr<MapEdit>& floor_ : Floors) {


		//�폜�t���O�������Ă���̂Ȃ�OBJ���폜
		Floors.remove_if([](std::unique_ptr<MapEdit>& floor_) {
			return floor_->GetDeadFlag();
			});

		//OBJ�̖��O
		char str[1024] = { 'F','l','o','o','r','_' };
		//�ԍ����肠��
		char str2[1024];
		//int��string�ɕϊ�
		sprintf_s(str2, "%d", Floor_count);
		//�����������
		strcat_s(str, str2);
		//�c���[�Ɋi�[
		if (ImGui::TreeNode(str)) {
			floor_->AdjustmentObj(floor_);
			ImGui::TreePop();
		}
		//Fbx�̍X�V����
		floor_->Update();
		//OBJ�̒P��PICKUP
		floor_->ObjSelect(floor_, mouse);


		//for���łP���₷
		Floor_count += 1;

		pos[Collision_count] = { floor_->GetPosition() };
		rotate[Collision_count] = { floor_->GetRotation() };
		colision_size[Collision_count] = { floor_->GetColisionSize() };
		Collision_count += 1;
	}

	for (std::unique_ptr<MapEdit>& exit_ : Exits) {


		//�폜�t���O�������Ă���̂Ȃ�OBJ���폜
		Exits.remove_if([](std::unique_ptr<MapEdit>& exit_) {
			return exit_->GetDeadFlag();
			});

		//OBJ�̖��O
		char str[1024] = { 'E','x','i','t','_' };
		//�ԍ����肠��
		char str2[1024];
		//int��string�ɕϊ�
		sprintf_s(str2, "%d", Exit_count);
		//�����������
		strcat_s(str, str2);
		//�c���[�Ɋi�[
		if (ImGui::TreeNode(str)) {
			exit_->AdjustmentObj(exit_);
			ImGui::TreePop();
		}
		//Fbx�̍X�V����
		exit_->Update();
		//OBJ�̒P��PICKUP
		exit_->ObjSelect(exit_, mouse);


		//for���łP���₷
		Exit_count += 1;

		pos[Collision_count] = { exit_->GetPosition() };
		rotate[Collision_count] = { exit_->GetRotation() };
		colision_size[Collision_count] = { exit_->GetColisionSize() };
		Collision_count += 1;
	}
	//imgui�̏I���
	ImGui::End();

	Collision_count = 0;//�z��v�f���J�E���g��0�ɂ���
	for (std::unique_ptr<MapEdit>& collision_ : Collisions) {
		//�e�����R���W�����ɑ������
		UpdateCollisionBox(colision_size[Collision_count], pos[Collision_count], rotate[Collision_count], collision_);
		Collision_count += 1;
	}

	

	//�`��

	//�t���O�������Ă�����R���W��������������
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
}

void MapEdit::WriteSet(int type,int ver, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize)
{
	FILE* fp1;
	//�e�L�X�g�Í���������Ԃŕۑ�
	fp1 = _fsopen("MapEdit/STAGE2.txt", "a", _SH_DENYNO);

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
		fprintf(fp1, "VERSION,%d\r\n", ver);//����
		fprintf(fp1, "COLISIONSIZE,%f,%f,%f", ColisionSize.x, ColisionSize.y, ColisionSize.z);//rotate
		fclose(fp1);
	}
}

void MapEdit::AdjustmentObj(std::unique_ptr<MapEdit>& obj_)
{
	//imgui�p�̕ϐ��쐬
	static XMFLOAT3 slider_position;
	static XMFLOAT3 slider_scale;
	static XMFLOAT3 slider_rotate;
	static XMFLOAT3 slider_colisionsize;
	//imgui�̕ϐ��Ɍ��݂�OBJ�̃f�[�^���
	slider_position = obj_->GetPosition();
	slider_scale = obj_->GetScale();
	slider_rotate = obj_->GetRotation();
	slider_colisionsize = obj_->GetColisionSize();


	//Obj�̏�񒲐�
	//position
	ImGui::InputFloat3("POSITION",&slider_position.x);
	//scale
	ImGui::InputFloat3("SCALE", &slider_scale.x);
	//rotation
	ImGui::InputFloat3("ROTATION", &slider_rotate.x);
	//colision
	ImGui::InputFloat3("COLISION", &slider_colisionsize.x);

	//�����ꂽ��OBJ���폜
	if (ImGui::Button("Delete"))
	{
		obj_->SetDeadFlag(true);
	}
	
	//�ύX�����l��n��
	obj_->SetPosition(slider_position);
	obj_->SetScale(slider_scale);
	obj_->SetRotate(slider_rotate);
	obj_->SetColisionSize(slider_colisionsize);	
}

void MapEdit::MapSave(std::unique_ptr<MapEdit>& obj_)
{
	//����o�^(txt�ɏ�������)
	//modelnum,pos,scale,rotation,colisionsize��
	WriteSet(obj_->GetFbxmodelType(), obj_->GetVer(),obj_->GetPosition(), obj_->GetScale(), obj_->GetRotation(), obj_->GetColisionSize());
}

void MapEdit::LoadSet(ID3D12GraphicsCommandList* cmdList)
{
	//�P�s���̕����������ϐ�
	std::string line;

	//�e���錾
	int type;
	int ver;
	XMFLOAT3 pos{};
	XMFLOAT3 scale{};
	XMFLOAT3 rotate{};
	XMFLOAT3 ColisionSize{};

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

		if (word.find("\r\n") == 0) {
			//skip
			continue;
		}
		//�|�W�V����

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



			//�W�߂�����������OBJ�̒��Ɏ��[���Ă���
			//������OBJ�𐶐�����i���f������W�̏���while���Ŏ�邽�߂����ł͓��ꕨ�����j
            //����
			std::unique_ptr<MapEdit>mapObj = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//������
			mapObj->Initialize();
			//type1�Ȃ�t�F���X���f�����Z�b�g
			if (type == 1)
			{
				//�����Ń��f���̌`����Z�b�g
				mapObj->SetModel(fence_model);
			}
			else if (type == 2)
			{
				//�����Ń��f���̌`����Z�b�g
				mapObj->SetModel(wall_model);
			}
			else if (type == 3)
			{
				//�����Ń��f���̌`����Z�b�g
				mapObj->SetModel(floor_model);
			}
			else if (type == 4)
			{
				//�����Ń��f���̌`����Z�b�g
				mapObj->SetModel(drum_model);
			}


			


			//���W���Z�b�g
			mapObj->SetPosition(pos);
			//�X�P�[�����Z�b�g
			mapObj->SetScale(scale);
			//��]���Z�b�g
			mapObj->SetRotate(rotate);
			//colisionsize�o�^
			mapObj->SetColisionSize(ColisionSize);
			//�����蔻��̃T�C�Y�A�`�A�������w��
			mapObj->SetColider(new WallCollider(XMVECTOR{ ColisionSize.x,ColisionSize.y,ColisionSize.z,0 }));
			//ver�ő��������߂�
			if (ver == 0)
			{
				mapObj->SetVerWall();
			}
			else if (ver == 1)
			{
				mapObj->SetVerExit();
			}
			//���̂ɕK�v�ȏ�񂪑������̂œo�^
			//�o�^
			mapObjs.push_back(std::move(mapObj));
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

void MapEdit::CreateCollisionBox()
{
	//����
	std::unique_ptr<MapEdit>Collision = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
	//������
	Collision->Initialize();
	//�����Ń��f���̌`����Z�b�g
	Collision->SetModel(Collision_model);
	//�X�P�[����W���ݒ�
	Collision->SetScale({ 1,1,1 });
	//�o�^
	Collisions.push_back(std::move(Collision));
}

void MapEdit::UpdateCollisionBox(XMFLOAT3 scale_,XMFLOAT3 pos_,XMFLOAT3 rotate_, std::unique_ptr<MapEdit>& Colobj_)
{
	XMFLOAT3 scale;
	//�R���W�����̃T�C�Y�𒲐�
	scale.x = scale_.x / 100;
	scale.y = scale_.y / 100;
	scale.z = scale_.z / 100;

	Colobj_->SetPosition(pos_);
//	Colobj_->SetRotate(rotate_);
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

	//�t�s����v�Z
	XMMATRIX matInverseVPV= XMMatrixInverse(nullptr,matViewProjection);

	//�X�N���[�����W
	XMVECTOR posNear = XMVECTOR{ mouse.x,mouse.y,0};
	XMVECTOR posFar =  XMVECTOR{ mouse.x,mouse.y,1};

	//�X�N���[�����W���烏�[���h���W��
	posNear = DirectX::XMVector3TransformCoord(posNear, matInverseVPV);
	posFar= DirectX::XMVector3TransformCoord(posFar, matInverseVPV);

	//�}�E�X���C�̕���
	XMVECTOR mouseDirection = DirectX::XMVectorSubtract(posFar, posNear);

	//���C�̕����𐳋K��
	mouseDirection = DirectX::XMVector3Normalize(mouseDirection);

	//�J��������Ə��I�u�W�F�N�g�̋���
	const float kDistance = 10000;

	//�����蔻��p�̃��C���쐬
	Ray mouse_ray;
	mouse_ray.start = posNear;
	mouse_ray.dir = mouseDirection;
	RaycastHit raycastHit;

	
	// ��������
	XMFLOAT3 Min_pos;
	XMFLOAT3 Max_pos;
	XMFLOAT3 pos2;
	Min_pos = obj_->GetPosition();
	Max_pos = obj_->GetPosition();
	pos2 = obj_->GetColisionSize();

	//�l�p�̓����蔻��̈ʒu�ő�l�A�Œ�l�ݒ�
	Min_pos.x = Min_pos.x - pos2.x;
	Min_pos.y = Min_pos.y - pos2.y;
	Min_pos.z = Min_pos.z - pos2.z;

	Max_pos.x = Max_pos.x + pos2.x;
	Max_pos.y = Max_pos.y + pos2.y;
	Max_pos.z = Max_pos.z + pos2.z;

	//�v�Z�p�ϐ��쐬
	double tmin = -INFINITY, tmax = INFINITY;
	double tmin2 = -INFINITY, tmax2 = INFINITY;
	double tmin3 = -INFINITY, tmax3 = INFINITY;
	double tmin4 = -INFINITY, tmax4 = INFINITY;

	//X��
	double tx1 = (Min_pos.x - mouse_ray.start.m128_f32[0]) / mouse_ray.dir.m128_f32[0];
	double tx2 = (Max_pos.x - mouse_ray.start.m128_f32[0]) / mouse_ray.dir.m128_f32[0];
	
	//tmin = min(tx1, tx2);
    //tmax = max(tx1, tx2);

	tmin = max(tmin, min(tx1, tx2));
	tmax = min(tmax, max(tx1, tx2));
	//Y��
	double ty1 = (Min_pos.y - mouse_ray.start.m128_f32[1]) / mouse_ray.dir.m128_f32[1];
	double ty2 = (Max_pos.y - mouse_ray.start.m128_f32[1]) / mouse_ray.dir.m128_f32[1];

	tmin2 = max(tmin, min(ty1, ty2));
	tmax2 = min(tmax, max(ty1, ty2));
	//Z��
	double tz1 = (Min_pos.z - mouse_ray.start.m128_f32[2]) / mouse_ray.dir.m128_f32[2];
	double tz2 = (Max_pos.z - mouse_ray.start.m128_f32[2]) / mouse_ray.dir.m128_f32[2];

	tmin3 = max(tmin, min(tz1, tz2));
    tmax3 = min(tmax, max(tz1, tz2));
	
	tmin4 = max(tmin, min(tz1, tz2));
	tmax4 = min(tmax, max(tz1, tz2));

	//�����蔻��̕����Ƀ}�E�X�|�C���g�̃��C���������Ă����ꍇ����PICKUP����
	if (tmax2 >= tmin2 && tmax3 >= tmin3 && tmax4 >= tmin4 && input->TriggerMouseRight())
	{
		obj_->SetSelectFlag(true);
	}
	else if (!(tmax2 >= tmin2 && tmax3 >= tmin3 && tmax4 >= tmin4) && input->TriggerMouseRight())
	{
		obj_->SetSelectFlag(false);
	}
}
