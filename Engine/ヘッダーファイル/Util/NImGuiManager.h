#pragma once
#include "NDX12.h"
#include <wrl.h>

class NImGuiManager final
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

public:
	static NImGuiManager* GetInstance();
	void Init();
	void Finalize();

	//ImGui受付開始
	void Begin();
	//ImGui受付終了
	void End();
	//画面への描画
	void Draw();

private:
	NImGuiManager() = default;
	NImGuiManager(const NImGuiManager&) = delete;
	~NImGuiManager() = default;
	NImGuiManager& operator=(const NImGuiManager&) = delete;
};