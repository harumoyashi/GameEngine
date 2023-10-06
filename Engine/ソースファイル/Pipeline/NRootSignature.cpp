#include "NRootSignature.h"

void NRootSignature::Create()
{
	HRESULT result;

	rootSignatureDesc_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc_.pParameters = rootParam_.begin()._Ptr;				//ルートパラメータの先頭アドレス
	rootSignatureDesc_.NumParameters = (uint32_t)rootParam_.size();	//ルートパラメータ数
	rootSignatureDesc_.pStaticSamplers = &samplerDesc_;
	rootSignatureDesc_.NumStaticSamplers = 1;

	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc_, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = NDX12::GetInstance()->GetDevice()->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&entity_));
	assert(SUCCEEDED(result));
}

void NRootSignature::SetSamplerDesc(bool isTiling)
{
	if (isTiling)
	{
		samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;			//横繰り返しする（タイリング）
		samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;			//縦繰り返しする（タイリング）
	}
	else
	{
		samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;			//横繰り返ししない（タイリングなし）
		samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;			//縦繰り返ししない（タイリングなし）
	}
	samplerDesc_.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//奥行繰り返し（タイリング）
	samplerDesc_.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc_.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//全てリニア補間
	samplerDesc_.MaxLOD = D3D12_FLOAT32_MAX;								//ミップマップ最大値
	samplerDesc_.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc_.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc_.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダからのみ使用可能
}

void NRootSignature::SetRootParam(const uint32_t texNum, const uint32_t constantNum)
{
	//デスクリプタレンジの設定
	for (uint32_t i = 0; i < texNum; i++)
	{
		descriptorRange_.emplace_back();
		descriptorRange_.back().NumDescriptors = 1;
		descriptorRange_.back().RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange_.back().BaseShaderRegister = i;	//テクスチャレジスタ番号
		descriptorRange_.back().OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	D3D12_ROOT_PARAMETER rootParam{};

	for (uint32_t i = 0; i < texNum; i++)
	{
		//テクスチャレジスタ
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
		rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange_[i];		//デスクリプタレンジ
		rootParam.DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//全てのシェーダーから見える
		rootParam_.push_back(rootParam);
	}

	for (uint32_t i = 0; i < constantNum; i++)
	{
		//定数バッファレジスタ
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
		rootParam.Descriptor.ShaderRegister = i;					//定数バッファ番号
		rootParam.Descriptor.RegisterSpace = 0;						//デフォルト値
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
		rootParam_.push_back(rootParam);
	}
}
