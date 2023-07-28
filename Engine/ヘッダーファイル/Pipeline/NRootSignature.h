#pragma once
#include "NDX12.h"
#include "NRootParam.h"

#include <wrl.h>

class NRootSignature final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12RootSignature> entity_;			//ルートシグネチャ
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc_{};	//ルートシグネチャデスク
	
	D3D12_STATIC_SAMPLER_DESC samplerDesc_{};		//テクスチャサンプラー

	std::vector<D3D12_ROOT_PARAMETER> rootParam_;			//ルートパラメータ
	std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRange_{};	//デスクリプタレンジ

public:
	//ルートシグネチャの生成
	void Create();
	//テクスチャサンプラーの設定
	//isTiling:タイリングするかフラグ
	void SetSamplerDesc(bool isTiling);

	//ルートパラメータ取得
	void SetRootParam(const uint32_t texNum, const uint32_t constantNum);
	//ルートシグネチャ取得
	ID3D12RootSignature* GetRootSignature()const { return entity_.Get(); }
};

