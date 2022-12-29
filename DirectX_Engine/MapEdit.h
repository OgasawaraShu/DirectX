#pragma once
#include "Input.h"
#include "Fbx3d.h"
#include "memory.h"
#include <list>
#include "stdio.h"
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
	void WriteSet(int type, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize);
	//���𒲐�
	void AdjustmentObj(std::unique_ptr<MapEdit>& obj_);
	//����txt�ɋL�^
	void MapSave(std::unique_ptr<MapEdit>& obj_);
	//���f���Z�b�g
	void SetFenceModel(Model*model) { fence_model = model; }
	//t��t�̏����Z�b�g
	void LoadSet(ID3D12GraphicsCommandList* cmdList);
	//txt�̏���ǂݍ���
	void Loadtxt();

	std::stringstream MapCommands;
private:
	std::list<std::unique_ptr<MapEdit>> Fences;

	std::list<std::unique_ptr<MapEdit>> mapObjs;

	float scaleX = 1.0f;
	float scaleY = 1.0f;

	int Window_Width;
	int Window_Height;

	Input* input;
	Model*fence_model;
	HWND hwnd = nullptr;
};

