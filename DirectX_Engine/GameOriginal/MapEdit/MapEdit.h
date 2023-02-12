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
	//HWNDをセット
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
	//スポーン地点関連
	void CreateSpawn();
	//copy処理
	void CopyModelInfo(int type, int ver, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize);
	//ペースト処理
	void PasteInfo();
	//矢印生成処理
	void ArowCreate();
	//矢印更新
	void ArowUpdate(std::unique_ptr<MapEdit>& obj_, XMFLOAT2 mouse);
	//矢印当たり判定
	void ArowCollision(std::unique_ptr<MapEdit>& obj_2,std::unique_ptr<MapEdit>& obj_, XMFLOAT2 mouse);

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
	//Gunモデル
	void SetGunModel(Model* model) { gun_model = model; }
	//箱モデル
	void SetBoxModel(Model* model) { box_model = model; }
	//コリジョンモデル
	void SetCollisionModel(Model* model) { Collision_model = model; }

	//セット
    //OBJの取ったかの参照
	bool GetGetGun() { return Cursor_on; }

	//デバック用モデル
	//スタート場所モデル
	void SetRespawnModel(Model* model) { respawn_model = model; }
	//矢印赤モデル
	void SetRedArowModel(Model* model) { Red_arow_model = model; }
	//矢印緑モデル
	void SetGreenArowModel(Model* model) { Green_arow_model = model; }
	//矢印青モデル
	void SetBlueArowModel(Model* model) { Blue_arow_model = model; }

	//ゲッター
	//初期リス
	XMFLOAT3 GetRespawn() { return Respawn_position; }
	//カメラインスタンスセット
	static void SetMapCamera(Camera* camera) { MapEdit::M_camera = camera; }
	//ファイル読み取り
	std::stringstream MapCommands;

private:

	//オブジェクトの可変配列
	std::list<std::unique_ptr<MapEdit>> Fences;

	std::list<std::unique_ptr<MapEdit>> Walls;

	std::list<std::unique_ptr<MapEdit>> Floors;

	std::list<std::unique_ptr<MapEdit>> Drums;

	std::list<std::unique_ptr<MapEdit>> Exits;

	std::list<std::unique_ptr<MapEdit>> Guns;

	std::list<std::unique_ptr<MapEdit>> Boxs;

	//エディットデバック可視化用モデル
	std::list<std::unique_ptr<MapEdit>> Respawns;

	std::list<std::unique_ptr<MapEdit>> Collisions;

	std::list<std::unique_ptr<MapEdit>> Arows;


	//出力時のオブジェクト配列
	std::list<std::unique_ptr<MapEdit>> mapObjs;

	//リスポーン地点代入変数
	XMFLOAT3 Respawn_position{};

	//COPY用入れ物変数
	XMFLOAT3 Copy_pos{};

	XMFLOAT3 Copy_scale{};

	XMFLOAT3 Copy_rotate{};

	XMFLOAT3 Copy_colision_size{};

	int Copy_model_num=100;

	int Copy_ver=0;

	//矢印の位置
	XMFLOAT3 Arow_pos{};
	//矢印の表示フラッグ
	bool Arow_draw_flag = false;
	//矢印の生成フラッグ
	bool Arow_create_flag = false;
	
	//マウスの1フレーム前座標
	XMFLOAT2 Old_mouse_move{};

	//矢印による位置移動フラグ
	bool Arow_ivent_flag = false;

	//矢印の制限管理変数
	int Arow_ivent_fixed = 0;

	float scaleX = 1.0f;
	float scaleY = 1.0f;

	//Windowサイズ
	int Window_Width;

	int Window_Height;

	//スポーンポイント生成フラグ
	bool Respawn_model_create_flag = false;

	//オブジェクトのモデルナンバー
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


	//オブジェクトの属性
	int null_ver = 0;

	int exit_ver = 1;

	int gun_ver = 2;

	int obj_ver = 3;

	//portalgunを取ったかのフラグ
	bool Cursor_on;

	//コリジョンサイズのモデル可視化フラグ
	bool Colision_draw_flag = true;
	//選択されたobjのフラッグ
	bool Select_obj_flag = false;
	//矢印のずらす内容フラグ
	int Arow_change_content = 1;
	//マウスの座標をFLOATに変換するための変数
	XMFLOAT2 mouse;
	//マウスの位置
	POINT mousePosition;
	//入力ポインタ
	Input* input;

   static  Camera* M_camera;

   //オブジェクトのモデルポインタ
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

