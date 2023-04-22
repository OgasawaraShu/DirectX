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

	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void MapEdit::MapInitialize()
{
	//txt����ǂݍ��񂾏������ƂɍX�V�A�^��
	for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs) {
		
		if (mapObj_->GetFbxmodelType() == 6)
		{
			//�K�v�ȏ����Z�b�g���Ă���
			mapObj_->SetTutorialGun(false);
			mapObj_->SetCursorOn(false);
			mapObj_->SetVerObj();
			mapObj_->SetPosition(mapObj_->GetInitPosition());
			mapObj_->SetRotate(mapObj_->GetInitRotate());
		}
		else if (mapObj_->GetFbxmodelType() == 7)
		{
			mapObj_->SetPosition(mapObj_->GetInitPosition());
			mapObj_->SetRotate(mapObj_->GetInitRotate());
		}
	}
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
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(300, 200));

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
			fence->SetColisionSize({ 10,10,2 });
			//�X�P�[���̃T�C�Y
			fence->SetScale({ 0.1,0.1,0.1 });
			//�|�W�V�����̏����ʒu�ݒ�
			fence->SetPosition({ 0,0,0 });
			//���f�����w��
			fence->SetFbxmodelType(Fence_model_num);
			//ver�w��
			fence->SetFbxVer(block_ver);
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
			drum->SetColisionSize({ 6,6,6 });
			//�X�P�[���̃T�C�Y
			drum->SetScale({ 0.02,0.02,0.02 });
			//�|�W�V�����̏����ʒu�ݒ�
			drum->SetPosition({ 0,10,0 });
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
			//�X�P�[���̃T�C�Y
			floor->SetScale({ 1,0.05,1 });
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
			exit->SetColisionSize({ 10,10,10 });
			//�X�P�[���̃T�C�Y
			exit->SetScale({ 0.1,0.1,0.1 });
			//���f�����w��
			exit->SetFbxmodelType(Exit_model_num);
			//ver�w��
			exit->SetFbxVer(exit_ver);
			//�o�^
			Exits.push_back(std::move(exit));

			//�����蔻��m�F�pOBJ����
			CreateCollisionBox();
		}

		//Gun���f������
		if (ImGui::Button("Gun Create"))
		{
			//����
			std::unique_ptr<MapEdit>gun = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//������
			gun->Initialize();
			//�����Ń��f���̌`����Z�b�g
			gun->SetModel(gun_model);
			//�R���W�����̃T�C�Y�o�^(�����͂O)
			gun->SetColisionSize({ 1,1,1});
			//�X�P�[���̃T�C�Y
			gun->SetScale({ 0.012,0.012,0.012 });
			//���f�����w��
			gun->SetFbxmodelType(Gun_model_num);
			//ver�w��
			gun->SetFbxVer(gun_ver);
			//�o�^
			Guns.push_back(std::move(gun));

			//�����蔻��m�F�pOBJ����
			CreateCollisionBox();
		}


		//�����f������
		if (ImGui::Button("Box Create"))
		{
			//����
			std::unique_ptr<MapEdit>box = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//������
			box->Initialize();
			//�����Ń��f���̌`����Z�b�g
			box->SetModel(box_model);
			//�R���W�����̃T�C�Y�o�^(�����͂O)
			box->SetColisionSize({ 10,10,10 });
			//�X�P�[���̃T�C�Y
			box->SetScale({ 3.5,3.5,3.5 });
			//���f�����w��
			box->SetFbxmodelType(Box_model_num);
			//ver�w��
			box->SetFbxVer(obj_ver);
			//�o�^
			Boxs.push_back(std::move(box));

			//�����蔻��m�F�pOBJ����
			CreateCollisionBox();
		}

		//�����f������
		if (ImGui::Button("BlockBox Create"))
		{
			//����
			std::unique_ptr<MapEdit>block = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//������
			block->Initialize();
			//�����Ń��f���̌`����Z�b�g
			block->SetModel(block_model);
			//�R���W�����̃T�C�Y�o�^(�����͂O)
			block->SetColisionSize({ 10,10,10 });
			//�X�P�[���̃T�C�Y
			block->SetScale({ 0.1,0.1,0.1 });
			//���f�����w��
			block->SetFbxmodelType(Block_model_num);
			//ver�w��
			block->SetFbxVer(block_ver);
			//�o�^
			Blocks.push_back(std::move(block));

			//�����蔻��m�F�pOBJ����
			CreateCollisionBox();
		}

		if (ImGui::Button("Tallet Create"))
		{
			//����
			std::unique_ptr<MapEdit>tallet = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//������
			tallet->Initialize();
			//�����Ń��f���̌`����Z�b�g
			tallet->SetModel(tallet_model);
			//�R���W�����̃T�C�Y�o�^(�����͂O)
			tallet->SetColisionSize({ 3,6,6 });
			//�X�P�[���̃T�C�Y
			tallet->SetScale({ 0.05,0.05,0.05 });
			//���f�����w��
			tallet->SetFbxmodelType(Tallet_model_num);
			//ver�w��
			tallet->SetFbxVer(obj_ver);
			//�o�^
			Tallets.push_back(std::move(tallet));

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
	//�{�^����������ăR�s�[���̒��ɏ�񂪓����Ă���̂Ȃ�y�[�X�g���Đ�������
	if (ImGui::Button("Paste")||(input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_V)))
	{
		PasteInfo();
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

		for (std::unique_ptr<MapEdit>& gun_ : Guns) {
			gun_->MapSave(gun_);
		}

		for (std::unique_ptr<MapEdit>& box_ : Boxs) {
			box_->MapSave(box_);
		}

		for (std::unique_ptr<MapEdit>& block_ : Blocks) {
			block_->MapSave(block_);
		}

		for (std::unique_ptr<MapEdit>& tallet_ : Tallets) {
			tallet_->MapSave(tallet_);
		}


		for (std::unique_ptr<MapEdit>& spawn_ : Respawns) {
			spawn_->MapSave(spawn_);
		}
	}

	ImGui::End();

	ArowCreate();
	

	//PIckUp���ꂽObj�̓��e��\��	
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
			//���ɂ��ʒu�̈ړ�
			ArowUpdate(fence_,mouse);

			
			//�����ꂽ�烂�f���̏����ꎞ�ۑ�(copy)
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

			//���̈ʒu�ϐ����
			Arow_pos = drum_->GetPosition();

			//���ɂ��ʒu�̈ړ�
			ArowUpdate(drum_, mouse);


			//�����ꂽ�烂�f���̏����ꎞ�ۑ�(copy)
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


			//���̈ʒu�ϐ����
			Arow_pos = walls_->GetPosition();

			//���ɂ��ʒu�̈ړ�
			ArowUpdate(walls_, mouse);


			//�����ꂽ�烂�f���̏����ꎞ�ۑ�(copy)
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


			//���̈ʒu�ϐ����
			Arow_pos = floor_->GetPosition();

			//���ɂ��ʒu�̈ړ�
			ArowUpdate(floor_, mouse);



			//�����ꂽ�烂�f���̏����ꎞ�ۑ�(copy)
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


			//���̈ʒu�ϐ����
			Arow_pos = exit_->GetPosition();

			//���ɂ��ʒu�̈ړ�
			ArowUpdate(exit_, mouse);

		}
	}


	for (std::unique_ptr<MapEdit>& gun_ : Guns) {
		if (gun_->GetSelectFlag() == true)
		{
			gun_->AdjustmentObj(gun_);

			Arow_draw_flag = true;


			//���̈ʒu�ϐ����
			Arow_pos = gun_->GetPosition();

			//���ɂ��ʒu�̈ړ�
			ArowUpdate(gun_, mouse);



			//�����ꂽ�烂�f���̏����ꎞ�ۑ�(copy)
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


			//���̈ʒu�ϐ����
			Arow_pos = box_->GetPosition();

			//���ɂ��ʒu�̈ړ�
			ArowUpdate(box_, mouse);



			//�����ꂽ�烂�f���̏����ꎞ�ۑ�(copy)
			if (ImGui::Button("Copy") || (input->PushKey(DIK_LCONTROL) && input->PushKey(DIK_C)))
			{
				CopyModelInfo(box_->GetFbxmodelType(), box_->GetVer(), box_->GetPosition(), box_->GetScale(), box_->GetRotation(), box_->GetColisionSize());
			}
		}
	}


	for (std::unique_ptr<MapEdit>& block_ : Blocks) {
		if (block_->GetSelectFlag() == true)
		{
			block_->AdjustmentObj(block_);

			Arow_draw_flag = true;


			//���̈ʒu�ϐ����
			Arow_pos = block_->GetPosition();

			//���ɂ��ʒu�̈ړ�
			ArowUpdate(block_, mouse);



			//�����ꂽ�烂�f���̏����ꎞ�ۑ�(copy)
			if (ImGui::Button("Copy") || (input->PushKey(DIK_LCONTROL) && input->PushKey(DIK_C)))
			{
				CopyModelInfo(block_->GetFbxmodelType(), block_->GetVer(), block_->GetPosition(), block_->GetScale(), block_->GetRotation(), block_->GetColisionSize());
			}
		}
	}

	for (std::unique_ptr<MapEdit>& tallet_ : Tallets) {
		if (tallet_->GetSelectFlag() == true)
		{
			tallet_ ->AdjustmentObj(tallet_);

			Arow_draw_flag = true;


			//���̈ʒu�ϐ����
			Arow_pos = tallet_->GetPosition();

			//���ɂ��ʒu�̈ړ�
			ArowUpdate(tallet_, mouse);



			//�����ꂽ�烂�f���̏����ꎞ�ۑ�(copy)
			if (ImGui::Button("Copy") || (input->PushKey(DIK_LCONTROL) && input->PushKey(DIK_C)))
			{
				CopyModelInfo(tallet_->GetFbxmodelType(), tallet_->GetVer(), tallet_->GetPosition(), tallet_->GetScale(), tallet_->GetRotation(), tallet_->GetColisionSize());
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

	
	//�eOBJ�̏��̒�������
	//�X�V
	//imgui�֘A
	ImGui::Begin("Obj_Data");
	ImGui::SetWindowPos(ImVec2(0, 200));
	ImGui::SetWindowSize(ImVec2(300, 200));


	//Obj�̃J�E���g�p
	int Fence_count = 1;
	int Drum_count = 1;
	int Wall_count = 1;
	int Floor_count = 1;
	int Exit_count = 1;
	int Gun_count = 1;
	int Box_count = 1;
	int Block_count = 1;
	int Tallet_count = 1;

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
			floor_->AdjustmentObj(floor_)
				;
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


	for (std::unique_ptr<MapEdit>& gun_ : Guns) {


		//�폜�t���O�������Ă���̂Ȃ�OBJ���폜
		Guns.remove_if([](std::unique_ptr<MapEdit>& gun_) {
			return gun_->GetDeadFlag();
			});

		//OBJ�̖��O
		char str[1024] = { 'G','u','n','_' };
		//�ԍ����肠��
		char str2[1024];
		//int��string�ɕϊ�
		sprintf_s(str2, "%d", Gun_count);
		//�����������
		strcat_s(str, str2);
		//�c���[�Ɋi�[
		if (ImGui::TreeNode(str)) {
			gun_->AdjustmentObj(gun_)
				;
			ImGui::TreePop();
		}
		//Fbx�̍X�V����
		gun_->Update();
		//OBJ�̒P��PICKUP
		gun_->ObjSelect(gun_, mouse);


		//for���łP���₷
		Gun_count += 1;

		pos[Collision_count] = { gun_->GetPosition() };
		rotate[Collision_count] = { gun_->GetRotation() };
		colision_size[Collision_count] = { gun_->GetColisionSize() };
		Collision_count += 1;
	}


	for (std::unique_ptr<MapEdit>& box_ : Boxs) {


		//�폜�t���O�������Ă���̂Ȃ�OBJ���폜
		Boxs.remove_if([](std::unique_ptr<MapEdit>& box_) {
			return box_->GetDeadFlag();
			});

		//OBJ�̖��O
		char str[1024] = { 'B','o','x','_' };
		//�ԍ����肠��
		char str2[1024];
		//int��string�ɕϊ�
		sprintf_s(str2, "%d", Box_count);
		//�����������
		strcat_s(str, str2);
		//�c���[�Ɋi�[
		if (ImGui::TreeNode(str)) {
			box_->AdjustmentObj(box_)
				;
			ImGui::TreePop();
		}
		//Fbx�̍X�V����
		box_->Update();
		//OBJ�̒P��PICKUP
		box_->ObjSelect(box_, mouse);


		//for���łP���₷
		Box_count += 1;

		pos[Collision_count] = { box_->GetPosition() };
		rotate[Collision_count] = { box_->GetRotation() };
		colision_size[Collision_count] = { box_->GetColisionSize() };
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


	for (std::unique_ptr<MapEdit>& block_ : Blocks) {


		//�폜�t���O�������Ă���̂Ȃ�OBJ���폜
		Blocks.remove_if([](std::unique_ptr<MapEdit>& block_) {
			return block_->GetDeadFlag();
			});

		//OBJ�̖��O
		char str[1024] = { 'B','l','o','c','k','_'};
		//�ԍ����肠��
		char str2[1024];
		//int��string�ɕϊ�
		sprintf_s(str2, "%d", Block_count);
		//�����������
		strcat_s(str, str2);
		//�c���[�Ɋi�[
		if (ImGui::TreeNode(str)) {
			block_->AdjustmentObj(block_)
				;
			ImGui::TreePop();
		}
		//Fbx�̍X�V����
		block_->Update();
		//OBJ�̒P��PICKUP
		block_->ObjSelect(block_, mouse);


		//for���łP���₷
		Block_count += 1;

		pos[Collision_count] = { block_->GetPosition() };
		rotate[Collision_count] = { block_->GetRotation() };
		colision_size[Collision_count] = { block_->GetColisionSize() };
		Collision_count += 1;
	}



	for (std::unique_ptr<MapEdit>& tallet_ : Tallets) {


		//�폜�t���O�������Ă���̂Ȃ�OBJ���폜
		Tallets.remove_if([](std::unique_ptr<MapEdit>& tallet_) {
			return tallet_->GetDeadFlag();
			});

		//OBJ�̖��O
		char str[1024] = { 'T','a','l','l','e','t','_'};
		//�ԍ����肠��
		char str2[1024];
		//int��string�ɕϊ�
		sprintf_s(str2, "%d", Tallet_count);
		//�����������
		strcat_s(str, str2);
		//�c���[�Ɋi�[
		if (ImGui::TreeNode(str)) {
			tallet_->AdjustmentObj(tallet_)
				;
			ImGui::TreePop();
		}
		//Fbx�̍X�V����
		tallet_->Update();
		//OBJ�̒P��PICKUP
		tallet_->ObjSelect(tallet_, mouse);


		//for���łP���₷
		Tallet_count += 1;

		pos[Collision_count] = { tallet_->GetPosition() };
		rotate[Collision_count] = { tallet_->GetRotation() };
		colision_size[Collision_count] = { tallet_->GetColisionSize() };
		Collision_count += 1;
	}

	CreateSpawn();
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


	for (std::unique_ptr<MapEdit>& guns_ : Guns) {
		guns_->Draw2(cmdList);
	}


	for (std::unique_ptr<MapEdit>& box_ : Boxs) {
		box_->Draw2(cmdList);
	}

	for (std::unique_ptr<MapEdit>& block_ : Blocks) {
		block_->Draw2(cmdList);
	}

	for (std::unique_ptr<MapEdit>& tallet_ : Tallets) {
		tallet_->Draw2(cmdList);
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
	//�e�L�X�g�Í���������Ԃŕۑ�
	fp1 = _fsopen("GameOriginal/MapEdit/STAGE5.txt", "a", _SH_DENYNO);

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

	if (scene == 2)
	{
		MapSet(cmdList, 400, MapCommands2, mapObjs2, mapObjs_red,mapObjs_blue);
	}

	//�����������炱���͊֐�������code��Z������ׂ�
	if (scene != 1)
	{
		MapSet(cmdList, 0, MapCommands, mapObjs, mapObjs_red, mapObjs_blue);
	}

	if (scene == 3)
	{
		MapSet(cmdList, 800, MapCommands3, mapObjs3, mapObjs_red, mapObjs_blue);
	}
	

}

void MapEdit::MapSet(ID3D12GraphicsCommandList* cmdList, float x, std::istream&txt, std::list<std::unique_ptr<MapEdit>>&  Objs, std::list<std::unique_ptr<MapEdit>>& Objs_red, std::list<std::unique_ptr<MapEdit>>& Objs_blue)
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

	while (getline(txt, line)) {
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

			//�^�C�v���O���ƃX�|�[���n�_�Ȃ̂Ŕ�΂�
			if (type != 0)
			{
				std::unique_ptr<MapEdit>mapObj = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
				std::unique_ptr<MapEdit>mapObj_red = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
				std::unique_ptr<MapEdit>mapObj_blue = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
				//������
				mapObj->Initialize();
				mapObj_red->Initialize();
				mapObj_blue->Initialize();
				//type1�Ȃ�t�F���X���f�����Z�b�g
				if (type == 1)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj->SetModel(fence_model);
					//���f�����w��
					mapObj->SetFbxmodelType(Fence_model_num);
				}
				else if (type == 2)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj->SetModel(wall_model);
					//���f�����w��
					mapObj->SetFbxmodelType(Wall_model_num);
				}
				else if (type == 3)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj->SetModel(floor_model);
					//���f�����w��
					mapObj->SetFbxmodelType(Floor_model_num);
				}
				else if (type == 4)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj->SetModel(drum_model);
					//���f�����w��
					mapObj->SetFbxmodelType(Drum_model_num);
				}
				else if (type == 5)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj->SetModel(exit_model);
					//���f�����w��
					mapObj->SetFbxmodelType(Exit_model_num);
				}
				else if (type == 6)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj->SetModel(gun_model);
					//���f�����w��
					mapObj->SetFbxmodelType(Gun_model_num);
				}
				else if (type == 7)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj->SetModel(box_model);
					//���f�����w��
					mapObj->SetFbxmodelType(Box_model_num);
				}
				else if (type == 8)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj->SetModel(block_model);
					//���f�����w��
					mapObj->SetFbxmodelType(Block_model_num);
				}
				else if (type == 9)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj->SetModel(tallet_model);
					//���f�����w��
					mapObj->SetFbxmodelType(Tallet_model_num);
				}


				//pos�����ɂ��炷
				pos.x = pos.x + x;
				//���W���Z�b�g
				mapObj->SetPosition(pos);
				mapObj->SetInitPosition(pos);
				//�X�P�[�����Z�b�g
				mapObj->SetScale(scale);
				//��]���Z�b�g
				mapObj->SetRotate(rotate);
				mapObj->SetInitRotate(rotate);
				//colisionsize�o�^
				mapObj->SetColisionSize(ColisionSize);


				//ver�ő��������߂�
				if (ver == 0)
				{
					//�����蔻��̃T�C�Y�A�`�A�������w��
					mapObj->SetColider(new WallCollider(XMVECTOR{ ColisionSize.x,ColisionSize.y,ColisionSize.z,0 }));
					mapObj->SetVerWall();

				}
				else if (ver == 1)
				{
					//�����蔻��̃T�C�Y�A�`�A�������w��
					mapObj->SetColider(new WallCollider(XMVECTOR{ ColisionSize.x,ColisionSize.y,ColisionSize.z,0 }));
					mapObj->SetVerExit();

				}
				else if (ver == 2)
				{
					//�����蔻��̃T�C�Y�A�`�A�������w��
					mapObj->SetColider(new SphereCollider(XMVECTOR{ 0,5,0,0 }, 5));
					mapObj->SetVerObj();
				}
				else if (ver == 3)
				{
					//�����蔻��̃T�C�Y�A�`�A�������w��
					mapObj->SetColider(new SphereCollider(XMVECTOR{ 0,5,0,0 }, 5));
					mapObj->SetVerObj2();
				}
				else if (ver == 4)
				{
					//�����蔻��̃T�C�Y�A�`�A�������w��
					mapObj->SetColider(new WallCollider(XMVECTOR{ ColisionSize.x,ColisionSize.y,ColisionSize.z,0 }));
					mapObj->SetVerBlock();

				}



				//��
				//type1�Ȃ�t�F���X���f�����Z�b�g
				if (type == 1)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_red->SetModel(fence_model);
					//���f�����w��
					mapObj_red->SetFbxmodelType(Fence_model_num);
				}
				else if (type == 2)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_red->SetModel(wall_model);
					//���f�����w��
					mapObj_red->SetFbxmodelType(Wall_model_num);
				}
				else if (type == 3)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_red->SetModel(floor_model);
					//���f�����w��
					mapObj_red->SetFbxmodelType(Floor_model_num);
				}
				else if (type == 4)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_red->SetModel(drum_model);
					//���f�����w��
					mapObj_red->SetFbxmodelType(Drum_model_num);
				}
				else if (type == 5)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_red->SetModel(exit_model);
					//���f�����w��
					mapObj_red->SetFbxmodelType(Exit_model_num);
				}
				else if (type == 6)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_red->SetModel(gun_model);
					//���f�����w��
					mapObj_red->SetFbxmodelType(Gun_model_num);
				}
				else if (type == 7)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_red->SetModel(box_model);
					//���f�����w��
					mapObj_red->SetFbxmodelType(Box_model_num);
				}
				else if (type == 8)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_red->SetModel(block_model);
					//���f�����w��
					mapObj_red->SetFbxmodelType(Block_model_num);
				}
				else if (type == 9)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj->SetModel(tallet_model);
					//���f�����w��
					mapObj->SetFbxmodelType(Tallet_model_num);
				}

				//���W���Z�b�g
				mapObj_red->SetPosition(pos);
				//�X�P�[�����Z�b�g
				mapObj_red->SetScale(scale);
				//��]���Z�b�g
				mapObj_red->SetRotate(rotate);
				//colisionsize�o�^
				mapObj_red->SetColisionSize(ColisionSize);

				//��
				//type1�Ȃ�t�F���X���f�����Z�b�g
				if (type == 1)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_blue->SetModel(fence_model);
					//���f�����w��
					mapObj_blue->SetFbxmodelType(Fence_model_num);
				}
				else if (type == 2)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_blue->SetModel(wall_model);
					//���f�����w��
					mapObj_blue->SetFbxmodelType(Wall_model_num);
				}
				else if (type == 3)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_blue->SetModel(floor_model);
					//���f�����w��
					mapObj_blue->SetFbxmodelType(Floor_model_num);
				}
				else if (type == 4)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_blue->SetModel(drum_model);
					//���f�����w��
					mapObj_blue->SetFbxmodelType(Drum_model_num);
				}
				else if (type == 5)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_blue->SetModel(exit_model);
					//���f�����w��
					mapObj_blue->SetFbxmodelType(Exit_model_num);
				}
				else if (type == 6)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_blue->SetModel(gun_model);
					//���f�����w��
					mapObj_blue->SetFbxmodelType(Gun_model_num);
				}
				else if (type == 7)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_blue->SetModel(box_model);
					//���f�����w��
					mapObj_blue->SetFbxmodelType(Box_model_num);
				}
				else if (type == 8)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj_blue->SetModel(block_model);
					//���f�����w��
					mapObj_blue->SetFbxmodelType(Block_model_num);
				}
				else if (type == 9)
				{
					//�����Ń��f���̌`����Z�b�g
					mapObj->SetModel(tallet_model);
					//���f�����w��
					mapObj->SetFbxmodelType(Tallet_model_num);
				}

				//���W���Z�b�g
				mapObj_blue->SetPosition(pos);
				//�X�P�[�����Z�b�g
				mapObj_blue->SetScale(scale);
				//��]���Z�b�g
				mapObj_blue->SetRotate(rotate);
				//colisionsize�o�^
				mapObj_blue->SetColisionSize(ColisionSize);
			
				//���̂ɕK�v�ȏ�񂪑������̂œo�^
				//�o�^
				Objs.push_back(std::move(mapObj));
				Objs_red.push_back(std::move(mapObj_red));
				Objs_blue.push_back(std::move(mapObj_blue));
			}
			else
			{
				//type���O�Ȃ玩�@�̏������X�Ȃ̂ŕϐ��ɑ��
				Respawn_position = pos;
			}
		}
	}
}

void MapEdit::Loadtxt()
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open(L"GameOriginal/MapEdit/STAGE2.txt");
	assert(file.is_open());

	//�t�@�C���̓��e���R�s�[
	MapCommands << file.rdbuf();
	//�t�@�C�������
	file.close();

	//�X�e�[�W2

	//�t�@�C�����J��
	std::ifstream file2;
	file2.open(L"GameOriginal/MapEdit/STAGE3.txt");
	assert(file2.is_open());

	//�t�@�C���̓��e���R�s�[
	MapCommands2 << file2.rdbuf();
	//�t�@�C�������
	file2.close();

	//�X�e�[�W3

	//�t�@�C�����J��
	std::ifstream file3;
	file3.open(L"GameOriginal/MapEdit/STAGE4.txt");
	assert(file3.is_open());

	//�t�@�C���̓��e���R�s�[
	MapCommands3 << file3.rdbuf();
	//�t�@�C�������
	file3.close();

}

void MapEdit::ObjMapUpdate(std::list<std::unique_ptr<MapEdit>>& Objs, int sceneSet)
{

	//txt����ǂݍ��񂾏������ƂɍX�V�A�^��
	for (std::unique_ptr<MapEdit>& mapObj_ : Objs) {
		if (mapObj_->GetFbxmodelType() != 6 && mapObj_->GetFbxmodelType() != 7 && mapObj_->GetFbxmodelType() != 9 && scene == sceneSet)
		{
			
			mapObj_->Update();
		}
		else if (mapObj_->GetFbxmodelType() == 6 && scene == sceneSet)
		{
			//�K�v�ȏ����Z�b�g���Ă���
			mapObj_->SetMyPosition(MyPosition);
			mapObj_->SetCameraAxisZ(CammeraZAxis);
			mapObj_->SetTarget(Target);

			mapObj_->SetShotBlue(Shot);
			mapObj_->SetShotRed(Shot2);
			mapObj_->SetWark(wark);

			mapObj_->SetTutorial(Tutorial);

			//Gun�����������̃`���[�g���A���t���O
			Tutorial_gun = mapObj_->GetTutorialGun();
			//Gun�����������̃t���O����
			Cursor_on = mapObj_->Getgetobj();
			//�����_�̎擾
			rotation.y = mapObj_->GetRotationY();
			if (Cursor_on == true)
			{
				mapObj_->SetVerSPHEREOBJ();
			}

			mapObj_->ObjUpdate(0, 0);
		}
		else if (mapObj_->GetFbxmodelType() == 7 && scene == sceneSet)
		{
			//�K�v�ȏ����Z�b�g���Ă���
			mapObj_->SetMyPosition(MyPosition);
			mapObj_->SetCameraAxisZ(CammeraZAxis);
			mapObj_->SetTarget(Target);

			mapObj_->SetTutorial(Tutorial);

			mapObj_->BoxObjUpdate(0, 0);
		}
		else if (mapObj_->GetFbxmodelType() == 9 && scene == sceneSet)
		{
			//�K�v�ȏ����Z�b�g���Ă���
			mapObj_->SetMyPosition(MyPosition);
			mapObj_->SetCameraAxisZ(CammeraZAxis);
			mapObj_->SetTarget(Target);

			mapObj_->SetTutorial(Tutorial);

			mapObj_->TurettUpdate();
		}
	}
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

void MapEdit::CreateSpawn()
{
	//�܂���x���������Ă��Ȃ��Ȃ�false�Ȃ̂Ő�������
	if (Respawn_model_create_flag == false)
	{
		//����
		std::unique_ptr<MapEdit>respawn = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//������
		respawn->Initialize();
		//�����Ń��f���̌`����Z�b�g
		respawn->SetModel(respawn_model);
		//�X�P�[����W���ݒ�
		respawn->SetScale({ 0.1,0.1,0.1 });
		//�|�W�V����������
		respawn->SetPosition({ 0.0,0.0,-20.0 });
		//�o�^
		Respawns.push_back(std::move(respawn));
		//�t���O��true�ɂ��Đ������~�߂�
		Respawn_model_create_flag = true;
	}

	//IMGUI�ł̒����p
	for (std::unique_ptr<MapEdit>& respawn_:Respawns) {

		//�c���[�Ɋi�[
		if (ImGui::TreeNode("Spawn point")) {
			//imgui�p�̕ϐ��쐬
			static XMFLOAT3 slider_position;
			//imgui�̕ϐ��Ɍ��݂�OBJ�̃f�[�^���
			slider_position = respawn_->GetPosition();
			//Obj�̏�񒲐�
			//position
			ImGui::InputFloat3("POSITION", &slider_position.x);

			//�ύX�����l��n��
			respawn_->SetPosition(slider_position);
			ImGui::TreePop();
		}
		//Fbx�̍X�V����
		respawn_->Update();
		//OBJ�̒P��PICKUP
		respawn_->ObjSelect(respawn_, mouse);
	}
}

void MapEdit::CopyModelInfo(int type, int ver, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize)
{
	//�n���ꂽObj�̏��𒆌p���p�ϐ��ɏ������ݕۑ�����
	//���f��
	Copy_model_num = type;
	//����
	Copy_ver = ver;
	//position
	Copy_pos = pos;
	//scale
	Copy_scale = scale;
	//rotation
	Copy_rotate = rotate;
	//�R���W�����T�C�Y
	Copy_colision_size = ColisionSize;
}

void MapEdit::PasteInfo()
{
	//Copy_model_num�̕ϐ��œǂݍ��ރ��f���𕪊򂷂�
	if (Copy_model_num == 1)
	{
		//����
		std::unique_ptr<MapEdit>fence = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//������
		fence->Initialize();
		//�����Ń��f���̌`����Z�b�g
		fence->SetModel(fence_model);
		//�R���W�����̃T�C�Y�o�^(�����͂O)
		fence->SetColisionSize(Copy_colision_size);
		//�X�P�[���̃T�C�Y
		fence->SetScale(Copy_scale);
		//�|�W�V�����̏����ʒu�ݒ�
		fence->SetPosition(Copy_pos);
		//��]
		fence->SetRotate(Copy_rotate);
		//���f�����w��
		fence->SetFbxmodelType(Fence_model_num);
		//ver�w��
		fence->SetFbxVer(block_ver);
		//�o�^
		Fences.push_back(std::move(fence));

		//�����蔻��m�F�pOBJ����
		CreateCollisionBox();
	}
	else if (Copy_model_num == 2)
	{
		//����
		std::unique_ptr<MapEdit>wall = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//������
		wall->Initialize();
		//�����Ń��f���̌`����Z�b�g
		wall->SetModel(wall_model);
		//�R���W�����̃T�C�Y�o�^(�����͂O)
		wall->SetColisionSize(Copy_colision_size);
		//�X�P�[���̃T�C�Y
		wall->SetScale(Copy_scale);
		//�|�W�V�����̏����ʒu�ݒ�
		wall->SetPosition(Copy_pos);
		//��]
		wall->SetRotate(Copy_rotate);
		//���f�����w��
		wall->SetFbxmodelType(Wall_model_num);
		//ver�w��
		wall->SetFbxVer(null_ver);
		//�o�^
		Walls.push_back(std::move(wall));

		//�����蔻��m�F�pOBJ����
		CreateCollisionBox();
	}
	else if (Copy_model_num == 3)
	{
		//����
		std::unique_ptr<MapEdit>floor = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//������
		floor->Initialize();
		//�����Ń��f���̌`����Z�b�g
		floor->SetModel(floor_model);
		//�R���W�����̃T�C�Y�o�^(�����͂O)
		floor->SetColisionSize(Copy_colision_size);
		//�X�P�[���̃T�C�Y
		floor->SetScale(Copy_scale);
		//�|�W�V�����̏����ʒu�ݒ�
		floor->SetPosition(Copy_pos);
		//��]
		floor->SetRotate(Copy_rotate);
		//���f�����w��
		floor->SetFbxmodelType(Floor_model_num);
		//ver�w��
		floor->SetFbxVer(null_ver);
		//�o�^
		Floors.push_back(std::move(floor));

		//�����蔻��m�F�pOBJ����
		CreateCollisionBox();
	}
	else if (Copy_model_num == 4)
	{
		//����
		std::unique_ptr<MapEdit>drum = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//������
		drum->Initialize();
		//�����Ń��f���̌`����Z�b�g
		drum->SetModel(drum_model);
		//�R���W�����̃T�C�Y�o�^(�����͂O)
		drum->SetColisionSize(Copy_colision_size);
		//�X�P�[���̃T�C�Y
		drum->SetScale(Copy_scale);
		//�|�W�V�����̏����ʒu�ݒ�
		drum->SetPosition(Copy_pos);
		//��]
		drum->SetRotate(Copy_rotate);
		//���f�����w��
		drum->SetFbxmodelType(Drum_model_num);
		//ver�w��
		drum->SetFbxVer(null_ver);
		//�o�^
		Drums.push_back(std::move(drum));

		//�����蔻��m�F�pOBJ����
		CreateCollisionBox();
	}
	else if (Copy_model_num == 5)
	{
		//����
		std::unique_ptr<MapEdit>exit = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//������
		exit->Initialize();
		//�����Ń��f���̌`����Z�b�g
		exit->SetModel(exit_model);
		//�R���W�����̃T�C�Y�o�^(�����͂O)
		exit->SetColisionSize(Copy_colision_size);
		//�X�P�[���̃T�C�Y
		exit->SetScale(Copy_scale);
		//�|�W�V�����̏����ʒu�ݒ�
		exit->SetPosition(Copy_pos);
		//��]
		exit->SetRotate(Copy_rotate);
		//���f�����w��
		exit->SetFbxmodelType(Exit_model_num);
		//ver�w��
		exit->SetFbxVer(exit_ver);
		//�o�^
		Exits.push_back(std::move(exit));

		//�����蔻��m�F�pOBJ����
		CreateCollisionBox();
	}
	else if (Copy_model_num == 8)
	{
		//����
		std::unique_ptr<MapEdit>block = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//������
		block->Initialize();
		//�����Ń��f���̌`����Z�b�g
		block->SetModel(block_model);
		//�R���W�����̃T�C�Y�o�^(�����͂O)
		block->SetColisionSize(Copy_colision_size);
		//�X�P�[���̃T�C�Y
		block->SetScale(Copy_scale);
		//�|�W�V�����̏����ʒu�ݒ�
		block->SetPosition(Copy_pos);
		//��]
		block->SetRotate(Copy_rotate);
		//���f�����w��
		block->SetFbxmodelType(Block_model_num);
		//ver�w��
		block->SetFbxVer(block_ver);
		//�o�^
		Blocks.push_back(std::move(block));

		//�����蔻��m�F�pOBJ����
		CreateCollisionBox();
	}
	else if (Copy_model_num == 9)
	{
		//����
		std::unique_ptr<MapEdit>tallet = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
		//������
		tallet->Initialize();
		//�����Ń��f���̌`����Z�b�g
		tallet->SetModel(tallet_model);
		//�R���W�����̃T�C�Y�o�^(�����͂O)
		tallet->SetColisionSize(Copy_colision_size);
		//�X�P�[���̃T�C�Y
		tallet->SetScale(Copy_scale);
		//�|�W�V�����̏����ʒu�ݒ�
		tallet->SetPosition(Copy_pos);
		//��]
		tallet->SetRotate(Copy_rotate);
		//���f�����w��
		tallet->SetFbxmodelType(Tallet_model_num);
		//ver�w��
		tallet->SetFbxVer(obj_ver);
		//�o�^
		Tallets.push_back(std::move(tallet));

		//�����蔻��m�F�pOBJ����
		CreateCollisionBox();
	}
	else
	{
	assert(1);
	}
}

void MapEdit::ArowCreate()
{
	//�t���O��false�Ȃ�܂��������Ă��Ȃ��̂Ő�������
	if (Arow_create_flag == false)
	{
		for (int i = 0; i < 3; i++)
		{
			//����
			std::unique_ptr<MapEdit>arow = std::make_unique<MapEdit>(Window_Width, Window_Height, input);
			//������
			arow->Initialize();

			//i�Ń��f���𕪊򂷂�(0=R,1=G,2=B)
			if (i == 0)
			{
				//�����Ń��f���̌`����Z�b�g
				arow->SetModel(Red_arow_model);
				//���f�����w��
				arow->SetFbxmodelType(Red_arow_model_num);
				//��]
				arow->SetRotate({ 0,180,0 });
				//�R���W�����̃T�C�Y�o�^(�����͂O)
				arow->SetColisionSize({ 10,1,1 });
			}
			else if(i==1)
			{
				//�����Ń��f���̌`����Z�b�g
				arow->SetModel(Green_arow_model);
				//���f�����w��
				arow->SetFbxmodelType(Green_arow_model_num);
				//��]
				arow->SetRotate({ 0,0,270 });
				//�R���W�����̃T�C�Y�o�^(�����͂O)
				arow->SetColisionSize({ 1,10,1 });
			}
			else if (i == 2)
			{
				//�����Ń��f���̌`����Z�b�g
				arow->SetModel(Blue_arow_model);
				//���f�����w��
				arow->SetFbxmodelType(Blue_arow_model_num);
				//��]
				arow->SetRotate({ 0,90,0 });
				//�R���W�����̃T�C�Y�o�^(�����͂O)
				arow->SetColisionSize({ 1,1,10 });
			}
			
			//�X�P�[���̃T�C�Y
			arow->SetScale({0.1,0.05,0.05});
			//�|�W�V�����̏����ʒu�ݒ�
			arow->SetPosition({0,0,0});
		
			//ver�w��
			arow->SetFbxVer(null_ver);
			//�o�^
			Arows.push_back(std::move(arow));
		}
		//�����̏������I�������t���O��true�ɂ���
		Arow_create_flag = true;
	}


}

void MapEdit::ArowUpdate(std::unique_ptr<MapEdit>& obj_, XMFLOAT2 mouse)
{
	for (std::unique_ptr<MapEdit>& arow_ : Arows) {

		//�N�_�̈ʒu��PickUpObj����ɐF���Ƃɖ��̈ʒu�𒲐�����
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

		//�`�悳��Ă����瓖���蔻��ON
		if (Arow_draw_flag == true)
		{
			arow_->ArowCollision(obj_,arow_,mouse);
		}
		//FBX�̍X�V����
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

	//�t�s����v�Z
	XMMATRIX matInverseVPV = XMMatrixInverse(nullptr, matViewProjection);

	//�X�N���[�����W
	XMVECTOR posNear = XMVECTOR{ mouse.x,mouse.y,0 };
	XMVECTOR posFar = XMVECTOR{ mouse.x,mouse.y,1 };

	//�X�N���[�����W���烏�[���h���W��
	posNear = DirectX::XMVector3TransformCoord(posNear, matInverseVPV);
	posFar = DirectX::XMVector3TransformCoord(posFar, matInverseVPV);

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

	//�̂̃}�E�X�̍��W�Ƃ̍��ňړ��ʂ����o
	XMFLOAT2 move;
	move.x = Old_mouse_move.x - mouse.x;
	move.y = Old_mouse_move.y - mouse.y;

	//���̂܂܂��ƍ����x�Ȃ̂Ŕ����ɒ���
	move.x = move.x * 0.5;
	move.y = move.y * 0.5;

	//�����蔻��̕����Ƀ}�E�X�|�C���g�̃��C���������Ă����ꍇ����PICKUP����
	if (tmax2 >= tmin2 && tmax3 >= tmin3 && tmax4 >= tmin4 && input->PushMouseMid())
	{
		Arow_ivent_flag = true;

		//�ʒu�̈ړ����d�������Ȃ����߂�Trigger�ŕ��򂳂���
		if (input->TriggerMouseMid())
		{
			//���̌����ŕ���
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
	
	//�L�[��������Ă�����}�E�X�̈ړ��ʂɂ����OBJ���ړ�������
	if (Arow_ivent_flag == true && input->PushMouseMid())
	{
		//pos
		if (Arow_change_content == 1)
		{
			//���̈ʒu���擾
			XMFLOAT3 Origin_obj_pos = obj_2->GetPosition();

			//�ړ��ʂɂ���Ĉʒu�����Z���Ă���
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
			//���̈ʒu���擾
			XMFLOAT3 Origin_obj_scale = obj_2->GetScale();

			//�ړ��ʂɂ���Ĉʒu�����Z���Ă���
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
		//�L�[�������ꂽ��ړ��������Œ��~������
		Arow_ivent_flag = false;
		Arow_ivent_fixed = 0;
	}

	//�}�E�X�̍��W��ۑ�
	Old_mouse_move = mouse;
}

void MapEdit::MapEditDraw(ID3D12GraphicsCommandList* cmdList)
{
	//txt����ǂݍ��񂾏������ƂɍX�V�A�^��
	for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs) {
		if (scene != 2 && mapObj_->GetFbxmodelType() != 6)
		{
			mapObj_->Draw2(cmdList);
		}

		if (mapObj_->GetFbxmodelType() == 6)
		{
			mapObj_->Draw2(cmdList);
		}
	}

	//�X�e�[�W2
	if (scene == 2)
	{
		for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs2) {
			mapObj_->Draw2(cmdList);
		}
	}

	//�X�e�[�W3
	if (scene == 3)
	{
		for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs3) {
			mapObj_->Draw2(cmdList);
		}
	}
}

void MapEdit::RedCameraObjUpdate(ID3D12GraphicsCommandList* cmdList)
{

	XMFLOAT3 Gun_pos{};
	XMFLOAT3 Gun_rotate{};
	for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs) {
		if (mapObj_->GetFbxmodelType() == 6)
		{
			Gun_pos=mapObj_->GetPosition();
			Gun_rotate = mapObj_->GetRotation();
		}
	}


	int Red_camera_select = 1;

	//txt����ǂݍ��񂾏������ƂɍX�V�A�^��
	for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs_red) {

		if (mapObj_->GetFbxmodelType() == 6)
		{
			//mapObj_->DrawPortalWindowMove(Gun_pos,Gun_rotate, Red_camera_select);
		}
		else
		{
			mapObj_->DrawPortalWindow(Red_camera_select);
		}
		mapObj_->Draw2(cmdList);
	}

}

void MapEdit::BlueCameraObjUpdate(ID3D12GraphicsCommandList* cmdList)
{
	XMFLOAT3 Gun_pos{};
	XMFLOAT3 Gun_rotate{};
	for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs) {
		if (mapObj_->GetFbxmodelType() == 6)
		{
			Gun_pos = mapObj_->GetPosition();
			Gun_rotate = mapObj_->GetRotation();
		}
	}


	int Blue_camera_select = 2;

	//txt����ǂݍ��񂾏������ƂɍX�V�A�^��
	for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs_blue) {

		if (mapObj_->GetFbxmodelType() == 6)
		{
		//	mapObj_->DrawPortalWindowMove(Gun_pos, Gun_rotate, Blue_camera_select);
		}
		else
		{
			mapObj_->DrawPortalWindow(Blue_camera_select);
		}
		mapObj_->Draw2(cmdList);
	}
}


void MapEdit::MapObjUpdate()
{

	//txt����ǂݍ��񂾏������ƂɍX�V�A�^��
	for (std::unique_ptr<MapEdit>& mapObj_ : mapObjs) {
		//mapObj_->Update();

		if (mapObj_->GetFbxmodelType() != 6 && mapObj_->GetFbxmodelType() != 7 && mapObj_->GetFbxmodelType() != 9&&  scene != 2)
		{
			mapObj_->Update();
		}
		else if (mapObj_->GetFbxmodelType() == 6 && scene != 2)
		{
			//�K�v�ȏ����Z�b�g���Ă���
			mapObj_->SetMyPosition(MyPosition);
			mapObj_->SetCameraAxisZ(CammeraZAxis);
			mapObj_->SetTarget(Target);
			mapObj_->SetShotBlue(Shot);
			mapObj_->SetShotRed(Shot2);
			mapObj_->SetWark(wark);
			mapObj_->SetTutorial(Tutorial);

			//Gun�����������̃`���[�g���A���t���O
			Tutorial_gun = mapObj_->GetTutorialGun();
			//Gun�����������̃t���O����
			Cursor_on = mapObj_->Getgetobj();

			if (Cursor_on == true)
			{
				mapObj_->SetVerSPHEREOBJ();
			}

			mapObj_->ObjUpdate(0, 0);
		}
		else if (mapObj_->GetFbxmodelType() == 7 && scene != 2)
		{
			//�K�v�ȏ����Z�b�g���Ă���
			mapObj_->SetMyPosition(MyPosition);
			mapObj_->SetCameraAxisZ(CammeraZAxis);
			mapObj_->SetTarget(Target);
			mapObj_->SetTutorial(Tutorial);

			mapObj_->BoxObjUpdate(0, 0);
		}
		else if (mapObj_->GetFbxmodelType() == 9 && scene !=2)
		{
			//�K�v�ȏ����Z�b�g���Ă���
			mapObj_->SetMyPosition(MyPosition);
			mapObj_->SetCameraAxisZ(CammeraZAxis);
			mapObj_->SetTarget(Target);

			mapObj_->SetTutorial(Tutorial);

			mapObj_->TurettUpdate();
		}
	}

	int sceneSecond = 2;
	int sceneThree = 3;
	ObjMapUpdate(mapObjs2, sceneSecond);
	ObjMapUpdate(mapObjs3, sceneThree);
}
