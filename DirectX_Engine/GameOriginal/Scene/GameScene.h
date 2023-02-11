
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
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �ÓI�����o�ϐ�
	static const int debugTextTexNumber = 0;
public:

	//�V�[��������
	void SceneInitialize(DirectXCommon* dxCommon, Input* input, Audio* audio);
	//�V�[���X�V
	void SceneUpdate();
	//�V�[���`��
	void SceneDraw();
private:
    
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText;
	LightGroup* lightGroup = nullptr;

	//���f��
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

	//�|�C���^
	BulletFbx* bullet = nullptr;
	PlayerFbx* player = nullptr;
	MapEdit* mapedit = nullptr;

};

*/