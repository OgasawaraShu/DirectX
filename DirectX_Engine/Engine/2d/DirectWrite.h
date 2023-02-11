#pragma once
#include "../Base/DirectXCommon.h"

#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )
#pragma comment( lib, "d3d11.lib" )

class DirectWrite:
	public DirectXCommon
{
public://メンバ関数
	void DirectWritePre(const std::string& key);

	void registerTextFormat(const std::string& key,
		const FLOAT fontSize);

	void DirectWritePreDraw();

	void DirectWriteTextLoad();

	void DirectWriteDraw(const std::string& textFormatKey, const std::string& solidColorBrushKey, const std::wstring& text /*,const D2D1_RECT_F& rect*/);

	void DirectWriteText();

	void SetWard(const std::wstring& ward_) { Text = ward_; }

	void SetWardCount(int count_) { Ward_count = count_; }

	void SetDev(const 	Microsoft::WRL::ComPtr<ID3D12Device> dev_) { dev = dev_; }

	void SetCmdQueue(const Microsoft::WRL::ComPtr<ID3D12CommandQueue>cmdQueue_) { cmdQueue = cmdQueue_; }

	void SetHwnd(const HWND hwnd_) { hwnd = hwnd_; }

	void SetBackbuffersSize0(const 	Microsoft::WRL::ComPtr<ID3D12Resource> backBuffers_) { backBuffers[0] = backBuffers_; }

	void SetBackbuffersSize1(const 	Microsoft::WRL::ComPtr<ID3D12Resource> backBuffers_) { backBuffers[1] = backBuffers_; }

	void SetSwapChain(const Microsoft::WRL::ComPtr<IDXGISwapChain4>swapchain_) { swapchain = swapchain_; }


	void DirectWriteInitialize();

	void DirectWriteRenderInitialize();

	std::wstring GetWard() { return Text; }
	int GetWardCount() { return Ward_count; }

	int GetSiezeWard()
	{
		int i = (int)Text.size();
		return i;
	}

private://メンバ関数
	

private://メンバ変数


		//DirectWrite
	IDWriteFactory* directWriteFactory = NULL;
	ID3D11DeviceContext* d3d11On12DeviceContext;

	Microsoft::WRL::ComPtr <ID2D1DeviceContext> d2dDeviceContext;
	Microsoft::WRL::ComPtr < ID3D11On12Device> d3d11On12Device;

	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Brush>> solidColorBrushMap;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<IDWriteTextFormat>>textFormatMap;
	//Microsoft::WRL::ComPtr <ID2D1Factory3> d2dfactory = nullptr;
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	std::vector<Microsoft::WRL::ComPtr<ID3D11Resource>>wrappedBackBuffers;
	std::vector<Microsoft::WRL::ComPtr<ID2D1Image>> d2dRenderTargets;
	int Wait_time = 0;
	int Next_text_wait_time = 0;
	int Draw_string_end = 1;
	int Speed;
	int Next_text_flag = true;
	int Ward_count;
	int Ward_num = 1;
	std::wstring Text;
	//ファイル読み取り
	std::stringstream MapCommands;

	std::list<std::unique_ptr<DirectWrite>> Wards;
};