#pragma once
#include "NDX12.h"
#include "NRootParam.h"

#include <wrl.h>

class NRootSignature
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12RootSignature> entity_;			//ルートシグネチャ
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc_{};	//ルートシグネチャデスク
	
	D3D12_STATIC_SAMPLER_DESC samplerDesc_{};		//テクスチャサンプラー

	NRootParam rootParams_;							//ルートパラメータ

public:
	//ルートシグネチャの生成
	void Create();
	//テクスチャサンプラーの設定
	//isTiling:タイリングするかフラグ
	void SetSamplerDesc(const bool isTiling);

	//ルートパラメータ取得
	NRootParam GetRootParam()const { return rootParams_; }
};

