#pragma once
#include "NDX12.h"
#include <wrl.h>

class ImGuiManager
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

public:
	static ImGuiManager* GetInstance();
	void Init();
	void Finalize();

	//ImGui受付開始
	void Begin();
	//ImGui受付終了
	void End();
	//画面への描画
	void Draw();
};