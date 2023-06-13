#pragma once
#include "NDX12.h"

#include <wrl.h>

class NRootSignature
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12RootSignature> entity_;
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc_{};
	ComPtr<ID3DBlob> rootSigBlob_;

public:
	//ルートシグネチャのシリアライズ(バイナリコード作成)
	void SetRootSignature(const ComPtr<ID3DBlob>& errorBlob_,
		const std::vector<D3D12_ROOT_PARAMETER>& rootParams, const D3D12_STATIC_SAMPLER_DESC& samplerDesc);
	//ルートシグネチャ生成
	void CreateRootSignature();
};

