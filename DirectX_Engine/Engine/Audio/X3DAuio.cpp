#include "X3DAuio.h"
//#pragma comment(lib,"xaudio3.lib")

void X3DAuio::X3dAudioInitialize()
{
	//X3dAuio������
	DWORD dwChannelMask;
	masterVoice->GetChannelMask(&dwChannelMask);

	X3DAudioInitialize(dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, X3DInstance);
}

void X3DAuio::X3dAudioSetPostion(std::string filename, XMFLOAT3 CameraPos, XMFLOAT3 AudioPos)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soudDatas_.find(filename);
	assert(it != soudDatas_.end());

	SoundData& soundData = it->second;

	//X3DAUDIO_LISTENER�����X3DAUDIO_EMITTER�\���̂̃C���X�^���X���쐬
	//�J�����̃|�W�V����
	Listener.Position = CameraPos;

	//�����o�����̂̈ʒu
	Emitter.Position = AudioPos;
	Emitter.ChannelCount = 1;
	Emitter.CurveDistanceScaler = Emitter.DopplerScaler = 1.0f;

	//X3DAUDIO_DSP_SETTINGS�\���̂̃C���X�^���X���쐬
	FLOAT32* matrix = new FLOAT32[soundData.wfex.nChannels];//
	DSPSettings.SrcChannelCount = 1;
	DSPSettings.DstChannelCount = soundData.wfex.nChannels;//
	DSPSettings.pMatrixCoefficients = matrix;

}

void X3DAuio::X3dAudioUpdate(std::string filename, XMFLOAT3 CameraPos, XMFLOAT3 AudioPos)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soudDatas_.find(filename);
	assert(it != soudDatas_.end());

	SoundData& soundData = it->second;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));


	//�ύX���ꂽ�l��K�p
	//Emitter.OrientFront = EmitterOrientFront;
	//Emitter.OrientTop = EmitterOrientTop;
	Emitter.Position = AudioPos;
	//Emitter.Velocity = EmitterVelocity;
	//Listener.OrientFront = ListenerOrientFront;
	//Listener.OrientTop = ListenerOrientTop;
	Listener.Position = CameraPos;
	//Listener.Velocity = ListenerVelocity;


	//�����̐V�����ݒ���v�Z
	X3DAudioCalculate(X3DInstance, &Listener, &Emitter,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB,
		&DSPSettings);

	//�{�����[���ƃs�b�`�̒l���\�[�X�����ɓK�p
	pSourceVoice->SetOutputMatrix(masterVoice, 1, soundData.wfex.nChannels, DSPSettings.pMatrixCoefficients);
	pSourceVoice->SetFrequencyRatio(DSPSettings.DopplerFactor);

	//�v�Z���ꂽ���o�[�u ���x�����T�u�~�b�N�X�����ɓK�p
	pSourceVoice->SetOutputMatrix(submixVoice, 1, 1, &DSPSettings.ReverbLevel);

	//�v�Z���ꂽ���[�p�X �t�B���^�[�_�C���N�g�W�����\�[�X�����ɓK�p
	XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFDirectCoefficient), 1.0f };
	pSourceVoice->SetFilterParameters(&FilterParameters);

}

