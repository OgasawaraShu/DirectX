#include "DirectWrite.h"
#include<cassert>
#include <d2d1_2.h>
#include<d2d1_3.h>
#include <fstream>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "dxguid.lib")
using namespace Microsoft::WRL;
const std::string DirectWrite::baseDirectory = "GameOriginal/Text";


void DirectWrite::DirectWriteLost()
{
    MapCommands.str("");
    // 状態をクリア
    MapCommands.clear(std::stringstream::goodbit);
    //変数の中身をリセット
     Wait_time = 0;
     Next_text_wait_time = 0;
     Draw_string_end = 1;
     Next_text_flag = true;
     Ward_num = 1;
}

void DirectWrite::DirectWritePre(const std::string& key)
{
    HRESULT result;

    if (solidColorBrushMap.find(key) != solidColorBrushMap.end()) [[unlikely]] {
        return;
    }
    ComPtr<ID2D1SolidColorBrush> brush = nullptr;

    d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(0.9f, 0.35f, 0.35f, 1.0f), brush.GetAddressOf());
    solidColorBrushMap[key] = brush;
}

void DirectWrite::registerTextFormat(const std::string& key, const FLOAT fontSize)
{
    HRESULT result;

    if (textFormatMap.find(key) != textFormatMap.end()) [[unlikely]] {
        return;
    }

    ComPtr<IDWriteTextFormat> textFormat = nullptr;
    directWriteFactory->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"ja-jp", textFormat.GetAddressOf());
    textFormatMap[key] = textFormat;
}

void DirectWrite::DirectWritePreDraw()
{
    const auto backBufferIndex = swapchain->GetCurrentBackBufferIndex();
    const auto wrappedBackBuffer = wrappedBackBuffers[backBufferIndex];
    const auto backBufferForD2D = d2dRenderTargets[backBufferIndex];
    d3d11On12Device->AcquireWrappedResources(wrappedBackBuffer.GetAddressOf(), 1);
    d2dDeviceContext->SetTarget(backBufferForD2D.Get());
    d2dDeviceContext->BeginDraw();
    d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

void DirectWrite::DirectWriteTextLoad(const std::string& key)
{
    //同じ名前から読み込む
    const  std::string directoryPath = baseDirectory +"/";
    //拡張子追加
    const  std::string fileName = key + ".txt";
    //連結してフルパスを得る
    const  std::string fullpath = directoryPath + fileName;

    //ファイルを開く
    std::ifstream file;
    file.open(fullpath);
    assert(file.is_open());

    //ファイルの内容をコピー
    MapCommands << file.rdbuf();
    //ファイルを閉じる
    file.close();
}

void DirectWrite::DirectWriteDraw(const std::string& textFormatKey, const std::string& solidColorBrushKey, const std::wstring& text)
{
    const auto textFormat = textFormatMap.at(textFormatKey);
    const auto solidColorBrush = solidColorBrushMap.at(solidColorBrushKey);
    d2dDeviceContext->DrawTextW(text.c_str(), static_cast<UINT32>(text.length()), textFormat.Get(), D2D1::RectF(215, 530, 1061, 655), solidColorBrush.Get());
    // Direct2Dの描画を終了する
    const auto backBufferIndex = swapchain->GetCurrentBackBufferIndex();
    const auto wrappedBackBuffer = wrappedBackBuffers[backBufferIndex];
    d2dDeviceContext->EndDraw();
    d3d11On12Device->ReleaseWrappedResources(wrappedBackBuffer.GetAddressOf(), 1);
    d3d11On12DeviceContext->Flush();
}

void DirectWrite::DirectWriteText(const std::string& key)
{
    //１行分の文字列を入れる変数
    std::string line;
    //読み取る変数、文字列
    std::string Load_text;
    int speed;

    //コマンド実行ループ
    int j = 1;
    if (Next_text_flag == true)
    {
        while (getline(MapCommands, line)) {
            //1行分の文字列をっストリーム変換して解析
            std::istringstream line_stream(line);

            std::string word;

            //,区切りで行の先頭文字を取得
            getline(line_stream, word, ',');

            //"//"から始まる行はコメント
            if (word.find("//") == 0) {
                //skip
                continue;
            }

            if (word.find("\r\n") == 0) {
                //skip
                continue;
            }


            if (word.find("WORD") == 0)
            {
                //可変長配列を生成
                std::unique_ptr<DirectWrite>ward_ = std::make_unique<DirectWrite>();

                //,区切りで文を読み取る
                getline(line_stream, word, ',');

                //stringをwstringに変換
                int in_length = (int)word.length();
                int out_length = MultiByteToWideChar(CP_UTF8, 0, word.c_str(), in_length, 0, 0);
                std::wstring result(out_length, L'\0');
                if (out_length)
                    MultiByteToWideChar(CP_UTF8, 0, word.c_str(), in_length, &result[0], out_length);

                //文とnumberをセット
                ward_->SetWard(result);

                ward_->SetWardCount(j);

                j += 1;

                //登録
                Wards.push_back(std::move(ward_));
            }
            else if (word.find("SPEED") == 0)
            {
                getline(line_stream, word, ',');
                speed = (float)std::atof(word.c_str());
                Speed = speed;
                Next_text_flag = false;

            }
        }

    }


    //テキスト内容
    int i = 0;
    for (std::unique_ptr<DirectWrite>& wards_ : Wards) {
        //カウントとnumが同じならそれを表示させるために代入する
        if (wards_->GetWardCount() == Ward_num)
        {
            i = wards_->GetSiezeWard();
            Text = wards_->GetWard();
        }

    }



    Wait_time++;
    //規定時間を超えたら次の文字を表示
    if (Wait_time > Speed)
    {
        //一文より長くなったらリセットして次の文を表示に移行
        if (i > Draw_string_end)
        {
            Draw_string_end += 1;
            Wait_time = 0;
            Under_sprite_flag = true;
        }
        else
        {
           
            Next_text_wait_time += 1;
            if (Next_text_wait_time > 120)
            {
                Next_text_wait_time = 0;
                Next_text_flag = true;
                Draw_string_end = 1;
                Ward_num += 1;
                Wait_time = 0;
                Under_sprite_flag = false;
            }
        }

    }

    //カウントの数によって一文字ずつ表示させていく範囲を決める
    std::wstring text2 = Text.substr(0, Draw_string_end);



    DirectWritePreDraw();

    DirectWriteDraw(key, key, text2);
}

void DirectWrite::DirectWriteInitialize()
{
    HRESULT result = S_FALSE;

    //IDWriteFactoryの生成
    result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&directWriteFactory));


    //ID3D11On12Deviceの生成
    ComPtr<ID3D11Device> d3d11Device = nullptr;
    UINT d3d11DeviceFlags = 0U;
#ifdef _DEBUG
    d3d11DeviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#else
    d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif
    result = D3D11On12CreateDevice
    (dev.Get(),
        d3d11DeviceFlags,
        nullptr,
        0U,
        reinterpret_cast<IUnknown**>(cmdQueue.GetAddressOf()),
        1U,
        0U,
        &d3d11Device,
        &d3d11On12DeviceContext,
        nullptr);

    d3d11Device.As(&d3d11On12Device);


    //ID2D1DeviceContextの生成
    ComPtr<ID2D1Factory3> d2dfactory = nullptr;
    constexpr D2D1_FACTORY_OPTIONS factoryOptions{};
    result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &factoryOptions, &d2dfactory);

    ComPtr<IDXGIDevice> dxgiDevice = nullptr;
    d3d11On12Device.As(&dxgiDevice);
    ComPtr<ID2D1Device> d2dDevice = nullptr;
    d2dfactory->CreateDevice(dxgiDevice.Get(), d2dDevice.ReleaseAndGetAddressOf());

    d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext);

    //裏表の2つ分
    backBuffers.resize(2);
}

void DirectWrite::DirectWriteRenderInitialize()
{
    HRESULT result;
    //ID2D1Bitmap1の生成,レンダ―ターゲットの初期化
    D3D11_RESOURCE_FLAGS flags = { D3D11_BIND_RENDER_TARGET };
    const UINT dpi = GetDpiForWindow(hwnd);
    D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), static_cast<float>(dpi), static_cast<float>(dpi));
    for (UINT i = 0U; i < 2; ++i) {
        ComPtr<ID3D11Resource> wrappedBackBuffer = nullptr;
        result = d3d11On12Device->CreateWrappedResource(backBuffers[i].Get(), &flags, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT, IID_PPV_ARGS(wrappedBackBuffer.ReleaseAndGetAddressOf()));

        ComPtr<IDXGISurface> dxgiSurface = nullptr;
        result = wrappedBackBuffer.As(&dxgiSurface);

        ComPtr<ID2D1Bitmap1> d2dRenderTarget = nullptr;

        result = d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bitmapProperties, &d2dRenderTarget);

        wrappedBackBuffers.emplace_back(wrappedBackBuffer);
        d2dRenderTargets.emplace_back(d2dRenderTarget);
    }
}
