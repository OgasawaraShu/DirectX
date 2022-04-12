#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

#include "SpriteCommon.h"

class Sprite
{
public:
    struct VertexPosUv
    {
        DirectX::XMFLOAT3 pos;//x y z　座標
        DirectX::XMFLOAT2 uv;//u v 座標
    };

    //定数バッファデータ構造体
    struct ConstBufferData8 {
        DirectX::XMFLOAT4 color8;//色
        DirectX::XMMATRIX mat8;//3D変換行列
    };

    static Sprite*Create(SpriteCommon* spriteCommon, UINT texnumber,
        DirectX::XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlagX = false, bool isFlagY = false);


private:
    SpriteCommon* spriteCommon_=nullptr;

    //頂点バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

    //頂点バッファビュー
    D3D12_VERTEX_BUFFER_VIEW vbView_;

    //定数バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

    //XYZ軸周りの回転軸
    DirectX::XMFLOAT3 rotation_ = { 0.0f,0.0f, 0.0f };

    //座標
    DirectX::XMFLOAT3 position_ = { 0,0,0 };

    //ワールド行列
    DirectX::XMMATRIX matWorld_;

    //色
    DirectX::XMFLOAT4 color_ = { 1,1,1,1 };

    //テクスチャ番号
    UINT texnumber_ = 0;

    //大きさ
    DirectX::XMFLOAT2 size_ = { 100, 100 };

    //アンカーポイント
    DirectX::XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };

    //左右反転
    bool isFlagX_ = false;

    //上下反転
    bool isFlagY_ = false;

    //テクスチャ左上座標
    DirectX::XMFLOAT2 texLeftTop_ = { 0,0 };

    //テクスチャ切り出しサイズ
    DirectX::XMFLOAT2 texSize_ = { 100,100 };

    //非表示
    bool isInvisible_ = false;

public:
    void Initialize(SpriteCommon* spriteCommon, UINT texnumber,
        DirectX::XMFLOAT2 anchorpoint, bool isFlagX , bool isFlagY );

    void SpriteTransVertexBuffer();

    void  SpriteDraw();


    //位置
    void SetPosition(const DirectX::XMFLOAT3& position) { position_ = position; }
    void SetRotation(const DirectX::XMFLOAT3& rotation) { rotation_= rotation; }
    void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; }
    void SettexLeftTop(const DirectX::XMFLOAT2& texLeftTop) { texLeftTop_ = texLeftTop; }
    void SettexSize(const DirectX::XMFLOAT2& texSize) { texSize_ = texSize; }

    void Update();
};

