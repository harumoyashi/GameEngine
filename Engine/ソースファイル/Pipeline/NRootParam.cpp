#include "NRootParam.h"

NRootParam::~NRootParam()
{
	entity_.clear();
}

void NRootParam::SetRootParam(const uint32_t texNum, const uint32_t constantNum)
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
		entity_.push_back(rootParam);
	}

	for (uint32_t i = 0; i < constantNum; i++)
	{
		//定数バッファレジスタ
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
		rootParam.Descriptor.ShaderRegister = i;					//定数バッファ番号
		rootParam.Descriptor.RegisterSpace = 0;						//デフォルト値
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
		entity_.push_back(rootParam);
	}
}
