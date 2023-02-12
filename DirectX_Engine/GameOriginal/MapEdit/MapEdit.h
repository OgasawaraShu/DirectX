#pragma once
#define __STDC_WANT_LIB_EXT1__ 1
#include "../../Engine/Input/Input.h"
#include "../../Engine/3d/Fbx3d.h"
#include "../Game/ObjFbx.h"
#include "memory.h"
#include <list>
#include "stdio.h"
#include <string.h>
#include <DirectXMath.h>
#include <share.h>
#include <sstream>

class MapEdit :
	public ObjFbx
{
protected: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:


public:
	MapEdit(int window_width, int window_height, Input* input);
	//HWND���Z�b�g
	void SetHwnd(HWND hwnd_) { hwnd = hwnd_; }

	void MapInitialize();
	//obj�����X�V���C������
	void CreateObj(ID3D12GraphicsCommandList* cmdList);
	//txt�ɏ������ޏ���
	void WriteSet(int type,int ver, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize);
	//���𒲐�
	void AdjustmentObj(std::unique_ptr<MapEdit>& obj_);
	//����txt�ɋL�^
	void MapSave(std::unique_ptr<MapEdit>& obj_);
	//t��t�̏����Z�b�g
	void LoadSet(ID3D12GraphicsCommandList* cmdList);
	//txt�̏���ǂݍ���
	void Loadtxt();
    //�R���W�����{�b�N�X�쐬
	void CreateCollisionBox();
	//�R���W�����{�b�N�XUpdate
	void UpdateCollisionBox(XMFLOAT3 scale_, XMFLOAT3 pos_, XMFLOAT3 rotate_, std::unique_ptr<MapEdit>& Colobj_);
	//OBJ��I��
	void ObjSelect(std::unique_ptr<MapEdit>& obj_,XMFLOAT2 mouse);
	//�X�|�[���n�_�֘A
	void CreateSpawn();
	//copy����
	void CopyModelInfo(int type, int ver, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize);
	//�y�[�X�g����
	void PasteInfo();
	//��󐶐�����
	void ArowCreate();
	//���X�V
	void ArowUpdate(std::unique_ptr<MapEdit>& obj_, XMFLOAT2 mouse);
	//��󓖂��蔻��
	void ArowCollision(std::unique_ptr<MapEdit>& obj_2,std::unique_ptr<MapEdit>& obj_, XMFLOAT2 mouse);

	//���f���Z�b�g
	//�t�F���X���f��
	void SetFenceModel(Model* model) { fence_model = model; }
	//�ǃ��f��
	void SetWallModel(Model* model) { wall_model = model; }
	//�����f��
	void SetFloorModel(Model* model) { floor_model = model; }
	//�h�������f��
	void SetDrumModel(Model* model) { drum_model = model; }
	//�o�����f��
	void SetExitModel(Model* model) { exit_model = model; }
	//Gun���f��
	void SetGunModel(Model* model) { gun_model = model; }
	//�����f��
	void SetBoxModel(Model* model) { box_model = model; }
	//�R���W�������f��
	void SetCollisionModel(Model* model) { Collision_model = model; }

	//�Z�b�g
    //OBJ�̎�������̎Q��
	bool GetGetGun() { return Cursor_on; }

	//�f�o�b�N�p���f��
	//�X�^�[�g�ꏊ���f��
	void SetRespawnModel(Model* model) { respawn_model = model; }
	//���ԃ��f��
	void SetRedArowModel(Model* model) { Red_arow_model = model; }
	//���΃��f��
	void SetGreenArowModel(Model* model) { Green_arow_model = model; }
	//�����f��
	void SetBlueArowModel(Model* model) { Blue_arow_model = model; }

	//�Q�b�^�[
	//�������X
	XMFLOAT3 GetRespawn() { return Respawn_position; }
	//�J�����C���X�^���X�Z�b�g
	static void SetMapCamera(Camera* camera) { MapEdit::M_camera = camera; }
	//�t�@�C���ǂݎ��
	std::stringstream MapCommands;

private:

	//�I�u�W�F�N�g�̉ϔz��
	std::list<std::unique_ptr<MapEdit>> Fences;

	std::list<std::unique_ptr<MapEdit>> Walls;

	std::list<std::unique_ptr<MapEdit>> Floors;

	std::list<std::unique_ptr<MapEdit>> Drums;

	std::list<std::unique_ptr<MapEdit>> Exits;

	std::list<std::unique_ptr<MapEdit>> Guns;

	std::list<std::unique_ptr<MapEdit>> Boxs;

	//�G�f�B�b�g�f�o�b�N�����p���f��
	std::list<std::unique_ptr<MapEdit>> Respawns;

	std::list<std::unique_ptr<MapEdit>> Collisions;

	std::list<std::unique_ptr<MapEdit>> Arows;


	//�o�͎��̃I�u�W�F�N�g�z��
	std::list<std::unique_ptr<MapEdit>> mapObjs;

	//���X�|�[���n�_����ϐ�
	XMFLOAT3 Respawn_position{};

	//COPY�p���ꕨ�ϐ�
	XMFLOAT3 Copy_pos{};

	XMFLOAT3 Copy_scale{};

	XMFLOAT3 Copy_rotate{};

	XMFLOAT3 Copy_colision_size{};

	int Copy_model_num=100;

	int Copy_ver=0;

	//���̈ʒu
	XMFLOAT3 Arow_pos{};
	//���̕\���t���b�O
	bool Arow_draw_flag = false;
	//���̐����t���b�O
	bool Arow_create_flag = false;
	
	//�}�E�X��1�t���[���O���W
	XMFLOAT2 Old_mouse_move{};

	//���ɂ��ʒu�ړ��t���O
	bool Arow_ivent_flag = false;

	//���̐����Ǘ��ϐ�
	int Arow_ivent_fixed = 0;

	float scaleX = 1.0f;
	float scaleY = 1.0f;

	//Window�T�C�Y
	int Window_Width;

	int Window_Height;

	//�X�|�[���|�C���g�����t���O
	bool Respawn_model_create_flag = false;

	//�I�u�W�F�N�g�̃��f���i���o�[
	int Fence_model_num = 1;

	int Wall_model_num = 2;

	int Floor_model_num = 3;

	int Drum_model_num = 4;

	int Exit_model_num = 5;

	int Gun_model_num = 6;

	int Box_model_num = 7;

	int spawn_model_num = 0;

	int Red_arow_model_num = -1;

	int Green_arow_model_num = -2;

	int Blue_arow_model_num = -3;


	//�I�u�W�F�N�g�̑���
	int null_ver = 0;

	int exit_ver = 1;

	int gun_ver = 2;

	int obj_ver = 3;

	//portalgun����������̃t���O
	bool Cursor_on;

	//�R���W�����T�C�Y�̃��f�������t���O
	bool Colision_draw_flag = true;
	//�I�����ꂽobj�̃t���b�O
	bool Select_obj_flag = false;
	//���̂��炷���e�t���O
	int Arow_change_content = 1;
	//�}�E�X�̍��W��FLOAT�ɕϊ����邽�߂̕ϐ�
	XMFLOAT2 mouse;
	//�}�E�X�̈ʒu
	POINT mousePosition;
	//���̓|�C���^
	Input* input;

   static  Camera* M_camera;

   //�I�u�W�F�N�g�̃��f���|�C���^
	Model*fence_model;

	Model*wall_model;

	Model*floor_model;

	Model*drum_model;

	Model* Collision_model;

	Model* exit_model;

	Model* gun_model;

	Model* box_model;

	Model* respawn_model;

	Model* Red_arow_model;

	Model* Green_arow_model;

	Model* Blue_arow_model;

	//HWND
	HWND hwnd = nullptr;
};

