#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <cstdint>
#include <map>
#include<string>

class Audio
{
private: // エイリアス
// Microsoft::WRL::を省略
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
    struct ChunkHeader
    {
        char id[4];
        int32_t  size;
    };

    struct RiffHeader
    {
        ChunkHeader chunk;
        char type[4];

    };
    //フォーマット形式
    struct FormatChunk
    {
        ChunkHeader chunk;
        WAVEFORMATEX fmt;
    };
    //サウンドのデータ
    struct SoundData
    {
        WAVEFORMATEX wfex;

        BYTE* pBuffer;

        unsigned int bufferSize;
    };

public : //メンバ関数
    //サウンド読み込み
    void SoundLoadWave(const char* filename);
    //サウンドアンロード
    void SoundUnload(SoundData* soundData);
    //サウンド再生
    void SoundPlayWave(std::string filename);
    //サウンドループ
    void SoundPlayWaveLoop(std::string filename);

    //初期化
    void Initialize();
    //解放
    void Finalize();
private://メンバ変数

    ComPtr<IXAudio2> xAudio2_;

    IXAudio2MasteringVoice* masterVoice;

    std::map<std::string, SoundData> soudDatas_;
};

