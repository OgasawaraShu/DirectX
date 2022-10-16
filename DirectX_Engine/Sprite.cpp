#include "Sprite.h"
#include <d3dx12.h>
#include "WinApp.h"
#include "Input.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

using namespace::DirectX;

Sprite* Sprite::Create(SpriteCommon* spriteCommon, UINT texnumber, DirectX::XMFLOAT2 anchorpoint, bool isFlagX, bool isFlagY)
{
    Sprite* instance = new Sprite();

    instance->Initialize(spriteCommon, texnumber,
        anchorpoint, isFlagX, isFlagY);

    return instance;
}

void Sprite::Initialize(SpriteCommon* spriteCommon, UINT texnumber,
    DirectX::XMFLOAT2 anchorpoint, bool isFlagX, bool isFlagY)
{
    HRESULT result = S_FALSE;


    //アンカーポイントをコピー
    anchorpoint_ = anchorpoint;

    //反転フラグをコピー
    isFlagX_ = isFlagX;

    isFlagY_ = isFlagY;

    spriteCommon_ = spriteCommon;

    VertexPosUv vertices[] = {
        {{0.0f	,100.0f	,0.0f},{0.0f,1.0f} },
        {{0.0f	,0.0f	,0.0f},{0.0f,0.0f} },
        {{100.0f,100.0f	,0.0f},{1.0f,1.0f} },
        {{100.0f,0.0f	,0.0f},{1.0f,0.0f} },


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

    if (spriteCommon_->GetTexBuff(texnumber_))
    {
        //テクスチャ情報の画像が読み込み済みなら
        D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetTexBuff(texnumber_)->GetDesc();

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
    result = spriteCommon_->GetDevice()->CreateCommittedResource(
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
    constMap->mat8 = spriteCommon_->GetMatProjection();
    constBuff_->Unmap(0, nullptr);

}

void Sprite::SpriteTransVertexBuffer()
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
    if (spriteCommon_->GetTexBuff(texnumber_))
    {
        //テクスチャ情報取得
     //   D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[sprite.texnumber]->GetDesc();
        D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetTexBuff(texnumber_)->GetDesc();

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

void Sprite::SpriteDraw()
{
    if (isInvisible_ == true)
    {
        return;
    }
    ID3D12GraphicsCommandList* cmdList = spriteCommon_->GetcmdList();

    //頂点バッファをセット
    cmdList->IASetVertexBuffers(0, 1, &vbView_);

    //定数バッファをセット
    cmdList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

    spriteCommon_->SetGraphicsRootDescriptorTable(1, texnumber_);
    //
    //cmdList->SetGraphicsRootDescriptorTable(1,
       // CD3DX12_GPU_DESCRIPTOR_HANDLE(spriteCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
       //     texnumber_,
       //     spriteCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

    //ポリゴンの描画
    cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::Update()
{
    //ワールド行列の更新
    matWorld_ = XMMatrixIdentity();

    //X軸回転
  //  matWorld_ *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));

    //Y軸回転
   // matWorld_ *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));

    //Z軸回転
    matWorld_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));




    //平行移動
    matWorld_ *= XMMatrixTranslation(position_.x, position_.y, position_.z);

    //定数バッファの転送
    ConstBufferData8* constMap = nullptr;
    HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
    constMap->color8 = color_;
    constMap->mat8 = matWorld_ * spriteCommon_->GetMatProjection();

    /*
    if (constMap->Time8 < 2.0f)
    {
        constMap->Time8 += 0.02f;
    }
    else
    {
        constMap->Time8 = 0.1f;
    }
    */

    constMap->Time8 += 2.0f;


    if (constMap->Bar8 < 20.0f)
    {
        constMap->Bar8 += 0.05f;
    }
    else
    {
        constMap->Bar8 = -30.0f;
    }
    constBuff_->Unmap(0, nullptr);
}

void Sprite::SpriteDivDraw(float& DIVnum, float Divsize, float& sizex, float sizey, float& count)
{
    count++;
    if (count >= 100)
    {
        DIVnum++;
        SettexLeftTop({ 0 + Divsize * DIVnum,0 });
        SettexSize({ sizex,sizey });
        count = 0;
        SpriteTransVertexBuffer();
    }
    if (DIVnum == 4)
    {
        DIVnum = 0;
    }
}

const float Sprite::clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

Sprite* Sprite::PostCreate(SpriteCommon* spriteCommon, UINT texnumber, DirectX::XMFLOAT2 anchorpoint, bool isFlagX, bool isFlagY)
{
    Sprite* instance = new Sprite();

    instance->PostInitialize(spriteCommon, texnumber,
        anchorpoint, isFlagX, isFlagY);

    return instance;
}


void Sprite::PostInitialize(SpriteCommon* spriteCommon, UINT texnumber,
    DirectX::XMFLOAT2 anchorpoint, bool isFlagX, bool isFlagY)
{
    HRESULT result;



    //アンカーポイントをコピー
    anchorpoint_ = anchorpoint;

    //反転フラグをコピー
    isFlagX_ = isFlagX;

    isFlagY_ = isFlagY;

    spriteCommon_ = spriteCommon;

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

    if (spriteCommon_->GetTexBuff(texnumber_))
    {
        //テクスチャ情報の画像が読み込み済みなら
        D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetTexBuff(texnumber_)->GetDesc();

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
    result = spriteCommon_->GetDevice()->CreateCommittedResource(
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
    constMap->mat8 = spriteCommon_->GetMatProjection();
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


void Sprite::CreateGraphicsPipelineState_Post()
{
    /*
    HRESULT result = S_OK;



    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;// ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;//ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;//ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

    // 頂点シェーダの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/shaders/SpriteVS.hlsl",  // シェーダファイル名
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
        "main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
        0,
        &vsBlob, &errorBlob);


    if (FAILED(result)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }


    // ピクセルシェーダの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/shaders/PostPS.hlsl",   // シェーダファイル名
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
        "main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
        0,
        &psBlob, &errorBlob);



    if (FAILED(result)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    // 頂点レイアウト
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        }, // (1行で書いたほうが見やすい)


        {
            "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        }, // (1行で書いたほうが見やすい)
    };


    CD3DX12_DESCRIPTOR_RANGE  descRangeSRV;
    //descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);//b0 レジスタ
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//t0 レジスタ


    CD3DX12_ROOT_PARAMETER rootparams[2];
    rootparams[0].InitAsConstantBufferView(0);//定数バッファビューとして初期化(b0　レジスタ)
    rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

    // グラフィックスパイプライン設定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

    //頂点シェーダ　ピクセルシェーダ
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());


    //ブレンド
//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA全てのチャンネルを描画
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    //共通設定(BLEND)
    blenddesc.BlendEnable = true;					//ブレンドを有効にする
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

    //加算合成
    //blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	 //加算
    //blenddesc.SrcBlend = D3D12_BLEND_ONE;	 //ソースの値を100%使う
    //blenddesc.DestBlend = D3D12_BLEND_ONE;	 //デストの値を100%使う

    //半透明合成
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    gpipeline.NumRenderTargets = 1; // 描画対象は1つ
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
    gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

        //標準的な設定(ポリゴン内塗りつぶし 深度クリッピングを有効に)
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面をカリングなし

    //デプスステンシルステートの設定

    //標準的な設定(書き込み許可)
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//一旦標準値をセット
    gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//常に上書き
    gpipeline.DepthStencilState.DepthEnable = false;//深度テストしない

    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット

    //デスクリプタテーブルの設定
    D3D12_DESCRIPTOR_RANGE descTblRange{};
    descTblRange.NumDescriptors = 1;
    descTblRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    descTblRange.BaseShaderRegister = 0;
    descTblRange.OffsetInDescriptorsFromTableStart =
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    //ルートパラメータ
    D3D12_ROOT_PARAMETER rootparam = {};
    rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootparam.DescriptorTable.pDescriptorRanges = &descTblRange;
    rootparam.DescriptorTable.NumDescriptorRanges = 1;
    rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;



    //ルートシグネチャ生成
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;



    //ルートシグネチャの設定
    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

    rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // PipelineSet pipelineSet;

    Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;

    result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
        &rootSigBlob, &errorBlob);

    result = spriteCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
        IID_PPV_ARGS(&pipelineSet_.rootsignature));

    gpipeline.pRootSignature = pipelineSet_.rootsignature.Get();

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;// ID3D12PipelineState* pipelinestate = nullptr;
    result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet_.pipelinestate));
    */

}

void Sprite::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
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
            spriteCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
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

void Sprite::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
    for (int i = 0; i < 2; i++) {
        cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
    }
}

void Sprite::PostDraw()
{


    /*
    if (isInvisible_ == true)
    {
        return;
    }
    ID3D12GraphicsCommandList* cmdList = spriteCommon_->GetcmdList();

    //パイプラインステートの設定
   // cmdList->SetPipelineState(pipelineSet.pipelinestate.Get());

    //ルートシグネチャの設定
   // /cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());

    //プリミティブ形状を設定
    //cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //テクスチャ用デスクリプタヒープ
    ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
    cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);



    //頂点バッファをセット
    cmdList->IASetVertexBuffers(0, 1, &vbView_);

    //定数バッファをセット
    cmdList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

    //spriteCommon_->SetGraphicsRootDescriptorTable(1, descHeapSRV);
    //
    //cmdList->SetGraphicsRootDescriptorTable(1,
       // CD3DX12_GPU_DESCRIPTOR_HANDLE(spriteCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
       //     texnumber_,
       //     spriteCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

    //ポリゴンの描画
    cmdList->DrawInstanced(4, 1, 0, 0);
    */

    // ワールド行列の更新
   // this->matWorld = XMMatrixIdentity();
   // this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
  //  this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

    // 定数バッファにデータ転送
  //  ConstBufferData* constMap = nullptr;
  //  HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
 //   if (SUCCEEDED(result)) {
 //       constMap->color = this->color;
 //       constMap->mat = this->matWorld * matProjection;	// 行列の合成	
 //       this->constBuff->Unmap(0, nullptr);
  //  }
//
    




    ID3D12GraphicsCommandList* cmdList = spriteCommon_->GetcmdList();


    //パイプラインステートの設定
   // cmdList->SetPipelineState(pipelineSet_.pipelinestate.Get());

    //ルートシグネチャの設定
   // cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());

    //プリミティブ形状を設定
   // cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


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
            spriteCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
        ));



    cmdList->SetGraphicsRootDescriptorTable(2,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(
            descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1,
            spriteCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
        ));
    // 描画コマンド
    cmdList->DrawInstanced(4, 1, 0, 0);
}
