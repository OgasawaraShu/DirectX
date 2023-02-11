#pragma once

#include<Windows.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include "WinApp.h"
#include <d3d11_1.h>
#include <directxcolors.h>
#include <dwrite.h>
#include <d3d11on12.h>
#include <wingdi.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <unordered_map>
#include <string.h>
#include "stdio.h"
#include <string>
#include <sstream>

#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )
#pragma comment( lib, "d3d11.lib" )
//DirectX���
class DirectXCommon
{
public://�����o�֐�

	//������
	void PreDirectWriteInitialize(WinApp*win);

	void PostDirectWriteInitialize();
 
	void PostRenderDirectWriteInitialize();


	void ImguiPre();

	void PreDraw();

	void PostDrawPreDirectWrite();

	void PostDrawPostDirectWrite();

	ID3D12Device* GetDev() { return dev.Get(); }

	ID3D12CommandQueue* GetCmdQueue() { return cmdQueue.Get(); }

	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }

	ID3D12Resource* GetBackBuffersSize0() { return backBuffers[0].Get(); }

	ID3D12Resource* GetBackBuffersSize1() { return backBuffers[1].Get(); }

	IDXGISwapChain4* GetSwapChain() { return swapchain.Get(); }
	
	void SetHwnd(HWND hwnd_) { hwnd = hwnd_; }

	

protected:

	

	void InitializeDevice();

	void InitializeCommand();

	void InitializeSwapchain();

	void InitializeRenderTargetView();

	void InitializeDepthBuffer();

	void InitializeFence();

	
	/// <summary>
	/// imgui������
	/// </summary>
	/// <returns>����</returns>
	bool InitImgui();
	//�f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device> dev;
	//DXGI�t�@�N�g��
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
	//WindowsAPI
	WinApp* winApp = nullptr;
    //�o�b�N�o�b�t�@
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	



protected: // �����o�ϐ�
    // �E�B���h�E�Y�A�v���P�[�V�����Ǘ�
	// Direct3D�֘A;
	Microsoft::WRL::ComPtr<IDXGISwapChain4>swapchain;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>cmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>cmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>cmdQueue;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>rtvHeaps;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
	Microsoft::WRL::ComPtr<ID3D12Fence>fence;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	HWND hwnd;
	UINT64 fenceVal = 0;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> imguiHeap;
};

