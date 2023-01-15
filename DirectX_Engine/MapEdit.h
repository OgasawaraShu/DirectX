#pragma once
#define __STDC_WANT_LIB_EXT1__ 1
#include "Input.h"
#include "Fbx3d.h"
#include "memory.h"
#include <list>
#include "stdio.h"
#include <string.h>
#include <DirectXMath.h>
#include <share.h>
#include "MapLoader.h"
#include <sstream>

class MapEdit :
	public Fbx3d
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
	//�R���W�������f��
	void SetCollisionModel(Model* model) { Collision_model = model; }


	static void SetMapCamera(Camera* camera) { MapEdit::M_camera = camera; }

	//XMFLOAT3 CalcScrennToWorld(XMFLOAT3 )
	std::stringstream MapCommands;

	std::stringstream MapCommands2;
private:

	//�I�u�W�F�N�g�̉ϔz��
	std::list<std::unique_ptr<MapEdit>> Fences;

	std::list<std::unique_ptr<MapEdit>> Walls;

	std::list<std::unique_ptr<MapEdit>> Floors;

	std::list<std::unique_ptr<MapEdit>> Drums;

	std::list<std::unique_ptr<MapEdit>> Exits;

	std::list<std::unique_ptr<MapEdit>> Collisions;

	//�o�͎��̃I�u�W�F�N�g�z��
	std::list<std::unique_ptr<MapEdit>> mapObjs;

	float scaleX = 1.0f;
	float scaleY = 1.0f;

	//Window�T�C�Y
	int Window_Width;

	int Window_Height;

	//�I�u�W�F�N�g�̃��f���i���o�[
	int Fence_model_num = 1;

	int Wall_model_num = 2;

	int Floor_model_num = 3;

	int Drum_model_num = 4;

	int Exit_model_num = 5;

	//�I�u�W�F�N�g�̑���
	int null_ver = 0;

	int exit_ver = 1;


	//�R���W�����T�C�Y�̃��f�������t���O
	bool Colision_draw_flag = true;
	//�I�����ꂽobj�̃t���b�O
	bool Select_obj_flag = false;


	XMFLOAT2 mouse;
	POINT mousePosition;
	Input* input;
   static  Camera* M_camera;

   //�I�u�W�F�N�g�̃��f���|�C���^
	Model*fence_model;

	Model*wall_model;

	Model*floor_model;

	Model*drum_model;

	Model* Collision_model;

	Model* exit_model;

	//HWND
	HWND hwnd = nullptr;
};

