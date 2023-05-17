#include "NRootParam.h"

NRootParam::~NRootParam()
{
	entity.clear();
}

void NRootParam::SetDescRange()
{
	descriptorRange.NumDescriptors = 3;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

void NRootParam::SetRootParam()
{
	D3D12_ROOT_PARAMETER rootParam{};

	//�萔�o�b�t�@0��
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParam.Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	entity.push_back(rootParam);
	//�e�N�X�`�����W�X�^0��
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange;			//�f�X�N���v�^�����W
	rootParam.DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_�[���猩����
	entity.push_back(rootParam);
	//�萔�o�b�t�@1��
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParam.Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	entity.push_back(rootParam);
	//�萔�o�b�t�@2��(�}�e���A��)
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParam.Descriptor.ShaderRegister = 2;					//�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	entity.push_back(rootParam);
	//�萔�o�b�t�@3��(���s����)
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParam.Descriptor.ShaderRegister = 3;					//�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	entity.push_back(rootParam);
	//�萔�o�b�t�@4��(�_����)
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParam.Descriptor.ShaderRegister = 4;					//�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	entity.push_back(rootParam);
	//�萔�o�b�t�@5��(�X�|�b�g���C�g)
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParam.Descriptor.ShaderRegister = 5;					//�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	entity.push_back(rootParam);
	//�萔�o�b�t�@6��(�ۉe)
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParam.Descriptor.ShaderRegister = 6;					//�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	entity.push_back(rootParam);
}
