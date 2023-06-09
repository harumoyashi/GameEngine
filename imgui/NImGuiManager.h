#pragma once
#include "NDX12.h"
#include <wrl.h>

class NImGuiManager
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<ID3D12DescriptorHeap> srvHeap__;

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
};