#include "NRootParam.h"

NRootParam::~NRootParam()
{
	entity.clear();
}

void NRootParam::SetDescRange()
{
	descriptorRange.NumDescriptors = 3;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

void NRootParam::SetRootParam()
{
	D3D12_ROOT_PARAMETER rootParam{};

	//定数バッファ0番
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParam.Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParam.Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	entity.push_back(rootParam);
	//テクスチャレジスタ0番
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange;			//デスクリプタレンジ
	rootParam.DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//全てのシェーダーから見える
	entity.push_back(rootParam);
	//定数バッファ1番
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParam.Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParam.Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	entity.push_back(rootParam);
	//定数バッファ2番(マテリアル)
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParam.Descriptor.ShaderRegister = 2;					//定数バッファ番号
	rootParam.Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	entity.push_back(rootParam);
	//定数バッファ3番(平行光源)
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParam.Descriptor.ShaderRegister = 3;					//定数バッファ番号
	rootParam.Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	entity.push_back(rootParam);
	//定数バッファ4番(点光源)
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParam.Descriptor.ShaderRegister = 4;					//定数バッファ番号
	rootParam.Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	entity.push_back(rootParam);
	//定数バッファ5番(スポットライト)
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParam.Descriptor.ShaderRegister = 5;					//定数バッファ番号
	rootParam.Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	entity.push_back(rootParam);
	//定数バッファ6番(丸影)
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParam.Descriptor.ShaderRegister = 6;					//定数バッファ番号
	rootParam.Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	entity.push_back(rootParam);
}
