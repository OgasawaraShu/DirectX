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
	void WriteSet(int type,int ver, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize);
	//情報を調整
	void AdjustmentObj(std::unique_ptr<MapEdit>& obj_);
	//情報をtxtに記録
	void MapSave(std::unique_ptr<MapEdit>& obj_);
	//tｘtの情報をセット
	void LoadSet(ID3D12GraphicsCommandList* cmdList);
	//txtの情報を読み込む
	void Loadtxt();
    //コリジョンボックス作成
	void CreateCollisionBox();
	//コリジョンボックスUpdate
	void UpdateCollisionBox(XMFLOAT3 scale_, XMFLOAT3 pos_, XMFLOAT3 rotate_, std::unique_ptr<MapEdit>& Colobj_);
	//OBJを選択
	void ObjSelect(std::unique_ptr<MapEdit>& obj_,XMFLOAT2 mouse);

	//モデルセット
	//フェンスモデル
	void SetFenceModel(Model* model) { fence_model = model; }
	//壁モデル
	void SetWallModel(Model* model) { wall_model = model; }
	//床モデル
	void SetFloorModel(Model* model) { floor_model = model; }
	//ドラムモデル
	void SetDrumModel(Model* model) { drum_model = model; }
	//出口モデル
	void SetExitModel(Model* model) { exit_model = model; }
	//コリジョンモデル
	void SetCollisionModel(Model* model) { Collision_model = model; }


	static void SetMapCamera(Camera* camera) { MapEdit::M_camera = camera; }

	//XMFLOAT3 CalcScrennToWorld(XMFLOAT3 )
	std::stringstream MapCommands;

	std::stringstream MapCommands2;
private:

	//オブジェクトの可変配列
	std::list<std::unique_ptr<MapEdit>> Fences;

	std::list<std::unique_ptr<MapEdit>> Walls;

	std::list<std::unique_ptr<MapEdit>> Floors;

	std::list<std::unique_ptr<MapEdit>> Drums;

	std::list<std::unique_ptr<MapEdit>> Exits;

	std::list<std::unique_ptr<MapEdit>> Collisions;

	//出力時のオブジェクト配列
	std::list<std::unique_ptr<MapEdit>> mapObjs;

	float scaleX = 1.0f;
	float scaleY = 1.0f;

	//Windowサイズ
	int Window_Width;

	int Window_Height;

	//オブジェクトのモデルナンバー
	int Fence_model_num = 1;

	int Wall_model_num = 2;

	int Floor_model_num = 3;

	int Drum_model_num = 4;

	int Exit_model_num = 5;

	//オブジェクトの属性
	int null_ver = 0;

	int exit_ver = 1;


	//コリジョンサイズのモデル可視化フラグ
	bool Colision_draw_flag = true;
	//選択されたobjのフラッグ
	bool Select_obj_flag = false;


	XMFLOAT2 mouse;
	POINT mousePosition;
	Input* input;
   static  Camera* M_camera;

   //オブジェクトのモデルポインタ
	Model*fence_model;

	Model*wall_model;

	Model*floor_model;

	Model*drum_model;

	Model* Collision_model;

	Model* exit_model;

	//HWND
	HWND hwnd = nullptr;
};

