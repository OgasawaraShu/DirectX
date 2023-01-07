#pragma once
#include "stdio.h"
#include <DirectXMath.h>
#include <share.h>

class MapWrite
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

	void WriteSet(int type, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT3 ColisionSize);

};

