#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "PostEffectCommon.h"

class PostEffect
{
private:
    struct VertexPosUv
    {
        DirectX::XMFLOAT3 pos;//x y z　座標
        DirectX::XMFLOAT2 uv;//u v 座標
    };

    //定数バッファデータ構造体
    struct ConstBufferData8 {
        DirectX::XMFLOAT4 color8;//色
        DirectX::XMMATRIX mat8;//3D変換行列
        float Time8;
        float Bar8;
        bool Post;
    };

    struct ConstBufferData9 {
        bool Post;
    };

    //ポストエフェクト
    static const float clearColor[4];

public:
 

   
    static PostEffect* PostCreate(PostEffectCommon* spriteCommon, UINT texnumber,
        DirectX::XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlagX = false, bool isFlagY = false);


    void PostInitialize(PostEffectCommon* spriteCommon, UINT texnumber,
        DirectX::XMFLOAT2 anchorpoint, bool isFlagX, bool isFlagY);

    

    void SetPost(bool a) { post = a; }

    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    void PostUpdate();

    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

    void SpriteTransVertexBuffer();

    void PostDraw();


private:
    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[2];

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    Microsoft::WRL::ComPtr<ID3D12Resource>depthBuff;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapRTV;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapDSV;

    Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState_Post;

    Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;

    PostEffectCommon* postCommon_ = nullptr;

    //頂点バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

    // 頂点数
    static const int vertNum_ = 4;

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

    float Yure = 0;

    bool post;

};

