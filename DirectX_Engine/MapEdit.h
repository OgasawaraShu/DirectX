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
protected: // エイリアス
	// DirectX::を省略
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
	//objを作る更新メイン処理
	void CreateObj(ID3D12GraphicsCommandList* cmdList);
	//txtに書き込む処理
	void WriteSet(int type, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize);
	//情報を調整
	void AdjustmentObj(std::unique_ptr<MapEdit>& obj_);
	//情報をtxtに記録
	void MapSave(std::unique_ptr<MapEdit>& obj_);
	//モデルセット
	void SetFenceModel(Model*model) { fence_model = model; }
	//tｘtの情報をセット
	void LoadSet(ID3D12GraphicsCommandList* cmdList);
	//txtの情報を読み込む
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

