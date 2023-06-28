#include "NRootParam.h"

NRootParam::~NRootParam()
{
	entity_.clear();
}

void NRootParam::SetDescRange(const uint32_t texNum)
{
	for (uint32_t i = 0; i < texNum; i++)
	{
		descriptorRange_.emplace_back();
		descriptorRange_.back().NumDescriptors = 1;
		descriptorRange_.back().RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange_.back().BaseShaderRegister = i;	//�e�N�X�`�����W�X�^�ԍ�
		descriptorRange_.back().OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}
}

void NRootParam::SetRootParam(const uint32_t texNum, const uint32_t constantNum)
{
	D3D12_ROOT_PARAMETER rootParam{};

	for (uint32_t i = 0; i < texNum; i++)
	{
		//�e�N�X�`�����W�X�^
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
		rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange_[i];		//�f�X�N���v�^�����W
		rootParam.DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_�[���猩����
		entity_.push_back(rootParam);
	}

	for (uint32_t i = 0; i < constantNum; i++)
	{
		//�萔�o�b�t�@���W�X�^
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
		rootParam.Descriptor.ShaderRegister = i;					//�萔�o�b�t�@�ԍ�
		rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
		entity_.push_back(rootParam);
	}

	////�萔�o�b�t�@0��
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	//rootParam.Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	//rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//entity_.push_back(rootParam);
	////�e�N�X�`�����W�X�^0��
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	//rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange0_;			//�f�X�N���v�^�����W
	//rootParam.DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_�[���猩����
	//entity_.push_back(rootParam);
	////�萔�o�b�t�@1��
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	//rootParam.Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	//rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//entity_.push_back(rootParam);
	////�萔�o�b�t�@2��(�}�e���A��)
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	//rootParam.Descriptor.ShaderRegister = 2;					//�萔�o�b�t�@�ԍ�
	//rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//entity_.push_back(rootParam);
	////�萔�o�b�t�@3��(�F)
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	//rootParam.Descriptor.ShaderRegister = 3;					//�萔�o�b�t�@�ԍ�
	//rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//entity_.push_back(rootParam);
	////�萔�o�b�t�@4��(���s����)
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	//rootParam.Descriptor.ShaderRegister = 4;					//�萔�o�b�t�@�ԍ�
	//rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//entity_.push_back(rootParam);
	////�萔�o�b�t�@5��(�_����)
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	//rootParam.Descriptor.ShaderRegister = 5;					//�萔�o�b�t�@�ԍ�
	//rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//entity_.push_back(rootParam);
	////�萔�o�b�t�@6��(�X�|�b�g���C�g)
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	//rootParam.Descriptor.ShaderRegister = 6;					//�萔�o�b�t�@�ԍ�
	//rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//entity_.push_back(rootParam);
	////�萔�o�b�t�@7��(�ۉe)
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	//rootParam.Descriptor.ShaderRegister = 7;					//�萔�o�b�t�@�ԍ�
	//rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//entity_.push_back(rootParam);
	////�e�N�X�`�����W�X�^1��
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	//rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange1_;		//�f�X�N���v�^�����W
	//rootParam.DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_�[���猩����
	//entity_.push_back(rootParam);
}
