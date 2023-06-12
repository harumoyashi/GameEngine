#include "NRootSignature.h"

void NRootSignature::SetRootSignature(ComPtr<ID3DBlob> errorBlob_, std::vector<D3D12_ROOT_PARAMETER> rootParams, D3D12_STATIC_SAMPLER_DESC samplerDesc)
{
	HRESULT result;

	rootSignatureDesc_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc_.pParameters = &rootParams.front();		//ルートパラメータの先頭アドレス
	rootSignatureDesc_.NumParameters = (uint32_t)rootParams.size();		//ルートパラメータ数
	rootSignatureDesc_.pStaticSamplers = &samplerDesc;
	rootSignatureDesc_.NumStaticSamplers = 1;

	result = D3D12SerializeRootSignature(&rootSignatureDesc_, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob_, &errorBlob_);
	assert(SUCCEEDED(result));
}

void NRootSignature::CreateRootSignature()
{
	HRESULT result;

	result = NDX12::GetInstance()->GetDevice()->CreateRootSignature(0, rootSigBlob_->GetBufferPointer(), rootSigBlob_->GetBufferSize(),
		IID_PPV_ARGS(&entity_));
	assert(SUCCEEDED(result));
}
