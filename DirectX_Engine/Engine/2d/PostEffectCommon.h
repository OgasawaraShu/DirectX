#pragma once
#include "../PipeLineSet.h"
#include <DirectXMath.h>

class PostEffectCommon
{
public:
	//�X�v���C�g�p�e�N�X�`������
	static const int spriteSRVCount = 512;

	//������
	void PostInitialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

	void PostPreDraw();
	

	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState_Post;

	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature_Post;

	void PostCreateGraphicsPipelineState();
	//�擾
	ID3D12Resource* GetTexBuff(int texNumber);

	ID3D12Device* GetDevice() { return device_; }

	const DirectX::XMMATRIX& GetMatProjection() { return matProjection_; }

	ID3D12GraphicsCommandList* GetcmdList() { return cmdList_; }

	//�p�C�v���C���Z�b�g
	PipeLineSet pipelineSet_;

private:

	void PostCreateSprite2dpipe();

private:

	//�e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_[spriteSRVCount];

	//�ˉe�s��
	DirectX::XMMATRIX matProjection_{};

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap_;

	ID3D12Device* device_ = nullptr;
	//�R�}���h��
	ID3D12GraphicsCommandList* cmdList_ = nullptr;
};

