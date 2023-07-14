#pragma once
#include "NDX12.h"
#include "NRootParam.h"

#include <wrl.h>

class NRootSignature final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12RootSignature> entity_;			//���[�g�V�O�l�`��
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc_{};	//���[�g�V�O�l�`���f�X�N
	
	D3D12_STATIC_SAMPLER_DESC samplerDesc_{};		//�e�N�X�`���T���v���[

	std::vector<D3D12_ROOT_PARAMETER> rootParam_;			//���[�g�p�����[�^
	std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRange_{};	//�f�X�N���v�^�����W

public:
	//���[�g�V�O�l�`���̐���
	void Create();
	//�e�N�X�`���T���v���[�̐ݒ�
	//isTiling:�^�C�����O���邩�t���O
	void SetSamplerDesc(bool isTiling);

	//���[�g�p�����[�^�擾
	void SetRootParam(const uint32_t texNum, const uint32_t constantNum);
	//���[�g�V�O�l�`���擾
	ID3D12RootSignature* GetRootSignature()const { return entity_.Get(); }
};

