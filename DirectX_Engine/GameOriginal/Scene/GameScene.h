
/*
#pragma once
#include "../Game/BulletFbx.h"
#include "../Game/PlayerFbx.h"
#include "../Game/ObjFbx.h"
#include "../Game/PortalExit.h"
#include "../Game/Physics.h"
#include "SceneSelect.h"
#include "../MapEdit/MapEdit.h"
#include "../Particle/ParticleManager.h"

#include "../../Engine/Base/DirectXCommon.h"
#include <DirectXMath.h>
#include "../../Engine/Input/Input.h"
#include "../../Engine/2d/Sprite.h"
#include "../Particle/ParticleManager.h"
#include "../../Engine/2d/DebugText.h"
#include "../../Engine/Audio/Audio.h"
#include "../../Engine/Camera/DebugCamera.h"
#include "../../Engine/Light/LightGroup.h"

class GameScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;
public:

	//シーン初期化
	void SceneInitialize(DirectXCommon* dxCommon, Input* input, Audio* audio);
	//シーン更新
	void SceneUpdate();
	//シーン描画
	void SceneDraw();
private:
    
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText;
	LightGroup* lightGroup = nullptr;

	//モデル
	Model* model1 = nullptr;
	Model* model2 = nullptr;
	Model* model3 = nullptr;
	Model* model4 = nullptr;
	Model* model5 = nullptr;
	Model* model6 = nullptr;
	Model* model7 = nullptr;
	Model* model8 = nullptr;
	Model* model9 = nullptr;
	Model* model10 = nullptr;
	Model* model11 = nullptr;
	Model* model12 = nullptr;
	Model* model13 = nullptr;
	Model* model13_2 = nullptr;
	Model* model15 = nullptr;
	Model* model16 = nullptr;
	Model* model17 = nullptr;
	Model* model14 = nullptr;
	Model* model18 = nullptr;
	Model* model19 = nullptr;
	Model* model20 = nullptr;
	Model* model21 = nullptr;
	Model* model22 = nullptr;
	Model* model23 = nullptr;
	Model* model24 = nullptr;
	Model* model25 = nullptr;
	Model* model26 = nullptr;
	Model* model27 = nullptr;
	Model* model28 = nullptr;
	Model* model29 = nullptr;
	Model* model30 = nullptr;

	//ポインタ
	BulletFbx* bullet = nullptr;
	PlayerFbx* player = nullptr;
	MapEdit* mapedit = nullptr;

};

*/