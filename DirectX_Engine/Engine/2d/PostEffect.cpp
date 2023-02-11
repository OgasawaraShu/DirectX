#include "PostEffect.h"
#include <d3dx12.h>
#include "../Base/WinApp.h"
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
using namespace::DirectX;

const float PostEffect::clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

PostEffect* PostEffect::PostCreate(PostEffectCommon* spriteCommon, UINT texnumber, DirectX::XMFLOAT2 anchorpoint, bool isFlagX, bool isFlagY)
{
    PostEffect* instance = new PostEffect();

    instance->PostInitialize(spriteCommon, texnumber,
        anchorpoint, isFlagX, isFlagY);

    return instance;
}

void PostEffect::PostInitialize(PostEffectCommon* spriteCommon, UINT texnumber, DirectX::XMFLOAT2 anchorpoint, bool isFlagX, bool isFlagY)
{
    HRESULT result;



    //アンカーポイントをコピー
    anchorpoint_ = anchorpoint;

    //反転フラグをコピー
    isFlagX_ = isFlagX;

    isFlagY_ = isFlagY;

    postCommon_ = spriteCommon;

    VertexPosUv vertices[] = {
        {{0.0f	,720.0f	,0.0f},{0.0f,1.0f} },
        {{0.0f	,0.0f	,0.0f},{0.0f,0.0f} },
        {{1280.0f,720.0f,0.0f},{1.0f,1.0f} },
        {{1280.0f,0.0f	,0.0f},{1.0f,0.0f} },


    };




    //頂点バッファの生成
    result = spriteCommon->GetDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff_)
    );

    texnumber_ = texnumber;

    //   if (spriteCommon_->texBuff_[sprite.texnumber])]

    if (postCommon_->GetTexBuff(texnumber_))
    {
        //テクスチャ情報の画像が読み込み済みなら
        D3D12_RESOURCE_DESC resDesc = postCommon_->GetTexBuff(texnumber_)->GetDesc();

        //スプライトの大きさを画像の解像度に合わせる
        size_ = { (float)resDesc.Width,(float)resDesc.Height };
        //  texSize = { (float)resDesc.Width,(float)resDesc.Height };
    }

    //頂点バッファデータ転送
    SpriteTransVertexBuffer();




    //頂点バッファビューへのデータ転送
    VertexPosUv* vertMap = nullptr;
    result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
    memcpy(vertMap, vertices, sizeof(vertices));
    vertBuff_->Unmap(0, nullptr);

    //頂点バッファビューの生成
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = sizeof(vertices);
    vbView_.StrideInBytes = sizeof(vertices[0]);

    //定数バッファの生成
    result = postCommon_->GetDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData8) + 0xff) & ~0xff),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuff_));

    //定数バッファにデータを転送
    ConstBufferData8* constMap = nullptr;
    result = constBuff_->Map(0, nullptr, (void**)&constMap);
    constMap->color8 = XMFLOAT4(1, 1, 1, 1);//色指定(R G B A)
    constMap->Time8 = 0.0f;
    constMap->Bar8 = -10.0f;

    //平行投影法
    constMap->mat8 = postCommon_->GetMatProjection();

    if (post == true)
    {
        constMap->Post = false;
    }
    else
    {
        constMap->Post = false;
    }
    constBuff_->Unmap(0, nullptr);



    CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        DXGI_FORMAT_R8G8B8A8_UNORM,
        WinApp::window_width,
        (UINT)WinApp::window_height,
        1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
    );

    for (int i = 0; i < 2; i++)
    {
        //テクスチャバッファ生成
        result = spriteCommon->GetDevice()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
                D3D12_MEMORY_POOL_L0),
            D3D12_HEAP_FLAG_NONE,
            &texresDesc,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
            IID_PPV_ARGS(&texBuff[i])
        );


        const UINT pixelCount = WinApp::window_width * WinApp::window_height;

        const UINT rowPitch = sizeof(UINT) * WinApp::window_width;

        const UINT depthPitch = rowPitch * WinApp::window_height;

        UINT* img = new UINT[pixelCount];
        for (int j = 0; j < pixelCount; j++) { img[j] = 0xff0000ff; }

        result = texBuff[i]->WriteToSubresource(0, nullptr,
            img, rowPitch, depthPitch);

        delete[] img;
    }
    D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
    srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    srvDescHeapDesc.NumDescriptors = 2;

    result = spriteCommon->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;


    for (int i = 0; i < 2; i++)
    {
        spriteCommon->GetDevice()->CreateShaderResourceView(texBuff[i].Get(),
            &srvDesc,
            CD3DX12_CPU_DESCRIPTOR_HANDLE(
                descHeapSRV->GetCPUDescriptorHandleForHeapStart(), i,
                spriteCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    }


    //RTV
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
    rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDescHeapDesc.NumDescriptors = 2;

    result = spriteCommon->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));

    for (int i = 0; i < 2; i++)
    {

        spriteCommon->GetDevice()->CreateRenderTargetView(texBuff[i].Get(),
            nullptr,
            CD3DX12_CPU_DESCRIPTOR_HANDLE(
                descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
                spriteCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
    }

    //深度バッファリソース設定
    CD3DX12_RESOURCE_DESC depthResDesc =
        CD3DX12_RESOURCE_DESC::Tex2D(
            DXGI_FORMAT_D32_FLOAT,
            WinApp::window_width,
            WinApp::window_height,
            1, 0,
            1, 0,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
        );

    //深度バッファ生成
    result = spriteCommon->GetDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depthResDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
        IID_PPV_ARGS(&depthBuff)
    );

    //DSV
    D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
    DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    DescHeapDesc.NumDescriptors = 1;

    result = spriteCommon->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));

    //デスクリプタヒープにDSV作成
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    spriteCommon->GetDevice()->CreateDepthStencilView(depthBuff.Get(),
        &dsvDesc,
        descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
    // ID3D12GraphicsCommandList* cmdList = spriteCommon_->GetcmdList()
    for (int i = 0; i < 2; i++) {
        cmdList->ResourceBarrier(1,
            &CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
                D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                D3D12_RESOURCE_STATE_RENDER_TARGET));
    }


    D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2];
    for (int i = 0; i < 2; i++) {
        rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
            postCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
    }
    D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
        descHeapDSV->GetCPUDescriptorHandleForHeapStart();

    cmdList->OMSetRenderTargets(2, rtvHs, false, &dsvH);

    CD3DX12_VIEWPORT viewports[2];
    CD3DX12_RECT scissorRects[2];
    for (int i = 0; i < 2; i++)
    {
        viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);
        scissorRects[i] = CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height);
    }
    cmdList->RSSetViewports(2, viewports);

    cmdList->RSSetScissorRects(2, scissorRects);

    for (int i = 0; i < 2; i++) {
        cmdList->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
    }
    cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PostEffect::PostUpdate()
{
    //ワールド行列の更新
    matWorld_ = XMMatrixIdentity();

    //X軸回転
    matWorld_ *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));

    //Y軸回転
    matWorld_ *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));

    //Z軸回転
    matWorld_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));




    //平行移動
    matWorld_ *= XMMatrixTranslation(position_.x, position_.y, position_.z);

    //定数バッファの転送
    ConstBufferData8* constMap = nullptr;
    HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
    constMap->color8 = color_;
    constMap->mat8 = matWorld_ * postCommon_->GetMatProjection();

    constMap->Time8 += 2.0f;


    if (constMap->Bar8 < 20.0f)
    {
        constMap->Bar8 += 0.05f;
    }
    else
    {
        constMap->Bar8 = -30.0f;
    }

    if (post == true)
    {
        constMap->Post = true;
    }
    else
    {
        constMap->Post = false;
    }
    constBuff_->Unmap(0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
    for (int i = 0; i < 2; i++) {
        cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
    }
}

void PostEffect::SpriteTransVertexBuffer()
{
    HRESULT result = S_FALSE;

    VertexPosUv vertices[] = {
        {{},{} },
        {{},{} },
        {{},{} },
        {{},{} },

    };

    //	 左下 左上 右下 右上
    enum { LB, LT, RB, RT };

    float left = (0.0f - anchorpoint_.x) * size_.x;

    float right = (1.0f - anchorpoint_.x) * size_.x;

    float top = (0.0f - anchorpoint_.y) * size_.y;

    float bottom = (1.0f - anchorpoint_.y) * size_.y;

    if (isFlagX_ == true)
    {
        left = -left;
        right = -right;
    }

    if (isFlagY_ == true)
    {
        top = -top;
        bottom = -bottom;
    }

    vertices[LB].pos = { left,	bottom, 0.0f };
    vertices[LT].pos = { left,	top,	0.0f };
    vertices[RB].pos = { right,	bottom, 0.0f };
    vertices[RT].pos = { right,	top,	0.0f };

    //UV
    //指定番号の画像が読み込み済みなら
    if (postCommon_->GetTexBuff(texnumber_))
    {
        //テクスチャ情報取得
     //   D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[sprite.texnumber]->GetDesc();
        D3D12_RESOURCE_DESC resDesc = postCommon_->GetTexBuff(texnumber_)->GetDesc();

        float tex_left = texLeftTop_.x / resDesc.Width;

        float tex_right = (texLeftTop_.x + texSize_.x) / resDesc.Width;

        float tex_top = texLeftTop_.y / resDesc.Height;

        float tex_bottom = (texLeftTop_.y + texSize_.y) / resDesc.Height;

        vertices[LB].uv = { tex_left,	tex_bottom };
        vertices[LT].uv = { tex_left,	tex_top };
        vertices[RB].uv = { tex_right,	tex_bottom };
        vertices[RT].uv = { tex_right,	tex_top };

    }

    //頂点バッファへのデータ転送
    VertexPosUv* vertMap = nullptr;
    result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
    memcpy(vertMap, vertices, sizeof(vertices));
    vertBuff_->Unmap(0, nullptr);
}

void PostEffect::PostDraw()
{
    ID3D12GraphicsCommandList* cmdList = postCommon_->GetcmdList();

    // 頂点バッファの設定
    cmdList->IASetVertexBuffers(0, 1, &this->vbView_);

    ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
    // デスクリプタヒープをセット
    cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    // 定数バッファビューをセット
    cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff_->GetGPUVirtualAddress());
    // シェーダリソースビューをセット
   // cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber, descriptorHandleIncrementSize));
    cmdList->SetGraphicsRootDescriptorTable(1,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(
            descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 0,
            postCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
        ));



    cmdList->SetGraphicsRootDescriptorTable(2,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(
            descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1,
            postCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
        ));
    // 描画コマンド
    cmdList->DrawInstanced(4, 1, 0, 0);
}
