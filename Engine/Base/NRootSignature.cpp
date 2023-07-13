#include "NRootSignature.h"

void NRootSignature::Create()
{
	HRESULT result;

	rootSignatureDesc_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc_.pParameters = rootParam_.begin()._Ptr;				//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc_.NumParameters = (uint32_t)rootParam_.size();	//���[�g�p�����[�^��
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

void NRootSignature::SetSamplerDesc(bool isTiling)
{
	if (isTiling)
	{
		samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;			//���J��Ԃ�����i�^�C�����O�j
		samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;			//�c�J��Ԃ�����i�^�C�����O�j
	}
	else
	{
		samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;			//���J��Ԃ����Ȃ��i�^�C�����O�Ȃ��j
		samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;			//�c�J��Ԃ����Ȃ��i�^�C�����O�Ȃ��j
	}
	samplerDesc_.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//���s�J��Ԃ��i�^�C�����O�j
	samplerDesc_.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc_.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//�S�ă��j�A���
	samplerDesc_.MaxLOD = D3D12_FLOAT32_MAX;								//�~�b�v�}�b�v�ő�l
	samplerDesc_.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc_.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc_.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_����̂ݎg�p�\
}

void NRootSignature::SetRootParam(const uint32_t texNum, const uint32_t constantNum)
{
	//�f�X�N���v�^�����W�̐ݒ�
	for (uint32_t i = 0; i < texNum; i++)
	{
		descriptorRange_.emplace_back();
		descriptorRange_.back().NumDescriptors = 1;
		descriptorRange_.back().RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange_.back().BaseShaderRegister = i;	//�e�N�X�`�����W�X�^�ԍ�
		descriptorRange_.back().OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	D3D12_ROOT_PARAMETER rootParam{};

	for (uint32_t i = 0; i < texNum; i++)
	{
		//�e�N�X�`�����W�X�^
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
		rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange_[i];		//�f�X�N���v�^�����W
		rootParam.DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_�[���猩����
		rootParam_.push_back(rootParam);
	}

	for (uint32_t i = 0; i < constantNum; i++)
	{
		//�萔�o�b�t�@���W�X�^
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
		rootParam.Descriptor.ShaderRegister = i;					//�萔�o�b�t�@�ԍ�
		rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
		rootParam_.push_back(rootParam);
	}
}
