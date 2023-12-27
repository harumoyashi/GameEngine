#pragma once
#include "NDX12.h"
#include <wrl.h>

// ImGui管理 //

class NImGuiManager final
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

public:
	//シングルトンインスタンス取得
	static NImGuiManager* GetInstance();
	//初期化
	void Init();
	//終了処理
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