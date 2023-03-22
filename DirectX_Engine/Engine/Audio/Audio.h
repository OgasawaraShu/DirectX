#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <x3daudio.h>
#include <wrl.h>
#include <cstdint>
#include <map>
#include<string>

class Audio
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
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
    //�t�H�[�}�b�g�`��
    struct FormatChunk
    {
        ChunkHeader chunk;
        WAVEFORMATEX fmt;
    };
    //�T�E���h�̃f�[�^
    struct SoundData
    {
        WAVEFORMATEX wfex;

        BYTE* pBuffer;

        unsigned int bufferSize;
    };

public : //�����o�֐�
    //�T�E���h�ǂݍ���
    void SoundLoadWave(const char* filename);
    //�T�E���h�A�����[�h
    void SoundUnload(SoundData* soundData);
    //�T�E���h�Đ�
    void SoundPlayWave(std::string filename, float Volume);
    //�T�E���h���[�v
    void SoundPlayWaveLoop(std::string filename, float Volume);

    //������
    void Initialize();
    //���
    void Finalize();
protected://�����o�ϐ�

    ComPtr<IXAudio2> xAudio2_;

    IXAudio2MasteringVoice* masterVoice;

    IXAudio2SubmixVoice* submixVoice;

    std::map<std::string, SoundData> soudDatas_;
};

