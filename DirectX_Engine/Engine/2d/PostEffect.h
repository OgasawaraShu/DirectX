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
        DirectX::XMFLOAT3 pos;//x y z�@���W
        DirectX::XMFLOAT2 uv;//u v ���W
    };

    //�萔�o�b�t�@�f�[�^�\����
    struct ConstBufferData8 {
        DirectX::XMFLOAT4 color8;//�F
        DirectX::XMMATRIX mat8;//3D�ϊ��s��
        float Time8;
        float Bar8;
        bool Post;
    };

    struct ConstBufferData9 {
        bool Post;
    };

    //�|�X�g�G�t�F�N�g
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

    //���_�o�b�t�@
    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

    // ���_��
    static const int vertNum_ = 4;

    //���_�o�b�t�@�r���[
    D3D12_VERTEX_BUFFER_VIEW vbView_;

    //�萔�o�b�t�@
    Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

    //XYZ������̉�]��
    DirectX::XMFLOAT3 rotation_ = { 0.0f,0.0f, 0.0f };

    //���W
    DirectX::XMFLOAT3 position_ = { 0,0,0 };

    //���[���h�s��
    DirectX::XMMATRIX matWorld_;

    //�F
    DirectX::XMFLOAT4 color_ = { 1,1,1,1 };

    //�e�N�X�`���ԍ�
    UINT texnumber_ = 0;

    //�傫��
    DirectX::XMFLOAT2 size_ = { 100, 100 };

    //�A���J�[�|�C���g
    DirectX::XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };

    //���E���]
    bool isFlagX_ = false;

    //�㉺���]
    bool isFlagY_ = false;

    //�e�N�X�`��������W
    DirectX::XMFLOAT2 texLeftTop_ = { 0,0 };

    //�e�N�X�`���؂�o���T�C�Y
    DirectX::XMFLOAT2 texSize_ = { 100,100 };

    //��\��
    bool isInvisible_ = false;

    float Yure = 0;

    bool post;

};

