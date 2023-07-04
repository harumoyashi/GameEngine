#pragma once
#include "NDX12.h"
#include "NRootParam.h"

#include <wrl.h>

class NRootSignature
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12RootSignature> entity_;			//���[�g�V�O�l�`��
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc_{};	//���[�g�V�O�l�`���f�X�N
	
	D3D12_STATIC_SAMPLER_DESC samplerDesc_{};		//�e�N�X�`���T���v���[

	NRootParam rootParams_;							//���[�g�p�����[�^

public:
	//���[�g�V�O�l�`���̐���
	void Create();
	//�e�N�X�`���T���v���[�̐ݒ�
	//isTiling:�^�C�����O���邩�t���O
	void SetSamplerDesc(const bool isTiling);

	//���[�g�p�����[�^�擾
	NRootParam GetRootParam()const { return rootParams_; }
};

