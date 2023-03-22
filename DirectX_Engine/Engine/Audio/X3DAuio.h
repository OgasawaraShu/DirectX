#pragma once
#include "Audio.h"
class X3DAuio :
    public Audio
{
private:

	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: 
	void X3dAudioInitialize();

    void X3dAudioSetPostion(std::string filename,XMFLOAT3 CameraPos,XMFLOAT3 AudioPos);

	void X3dAudioUpdate(std::string filename, XMFLOAT3 CameraPos, XMFLOAT3 AudioPos);

private:

	X3DAUDIO_HANDLE X3DInstance;

	X3DAUDIO_LISTENER Listener = {};

	X3DAUDIO_EMITTER Emitter = {};

	X3DAUDIO_DSP_SETTINGS DSPSettings = {};
};

