#pragma once
#include "NDX12.h"
#include <wrl.h>

class NImGuiManager
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

public:
	static NImGuiManager* GetInstance();
	void Init();
	void Finalize();

	//ImGui��t�J�n
	void Begin();
	//ImGui��t�I��
	void End();
	//��ʂւ̕`��
	void Draw();
};