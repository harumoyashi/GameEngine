#include "NRootSignature.h"

void NRootSignature::Create()
{
	HRESULT result;

	rootSignatureDesc_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc_.pParameters = &rootParams_.entity_.front();				//ルートパラメータの先頭アドレス
	rootSignatureDesc_.NumParameters = (uint32_t)rootParams_.entity_.size();	//ルートパラメータ数
	rootSignatureDesc_.pStaticSamplers = &samplerDesc_;
	rootSignatureDesc_.NumStaticSamplers = 1;

	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc_, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = NDX12::GetInstance()->GetDevice()->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),IID_PPV_ARGS(&entity_));
	assert(SUCCEEDED(result));
}

void NRootSignature::SetSamplerDesc(const bool isTiling)
{
	if (isTiling)
	{
		samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;			//横繰り返しする（タイリング）
		samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;			//縦繰り返しする（タイリング）
	}
	else
	{
		samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;			//横繰り返ししない（タイリングなし）
		samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;			//縦繰り返ししない（タイリングなし）
	}
	samplerDesc_.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//奥行繰り返し（タイリング）
	samplerDesc_.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc_.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//全てリニア補間
	samplerDesc_.MaxLOD = D3D12_FLOAT32_MAX;								//ミップマップ最大値
	samplerDesc_.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc_.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc_.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダからのみ使用可能
}
