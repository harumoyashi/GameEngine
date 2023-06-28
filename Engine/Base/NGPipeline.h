#pragma once
#include <DirectXMath.h>
#include "NDX12.h"
#include "NTexture.h"
#include "NRootSignature.h"
#include "NRootParam.h"
#include <d3dx12.h>

#include <wrl.h>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

struct PipelineSet
{
	ComPtr<ID3D12PipelineState> pipelineState_;
	NRootSignature rootSig_;
};

enum class PipelineType
{
	Basic,
	Sprite,
	Particle,
};

class NGPipeline
{
private:
	//�V�F�[�_�[���\�[�X�r���[//
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};	//�ݒ�\����

	//�V�F�[�_�[�܂��(���_���܂�)//
	ComPtr<ID3DBlob> vsBlob_;	// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob_; // �G���[�I�u�W�F�N�g

	//���_���C�A�E�g//
	D3D12_INPUT_ELEMENT_DESC vertLayout3d_[3]{};		//�K�v�ȕ������p�ӂ���
	D3D12_INPUT_ELEMENT_DESC vertLayoutSprite_[2]{};	//�K�v�ȕ������p�ӂ���
	D3D12_INPUT_ELEMENT_DESC vertLayoutPostEffect_[2]{};	//�K�v�ȕ������p�ӂ���

	//�p�C�v���C���X�e�[�g//
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc_ = {};

	//�e�N�X�`���T���v���[//
	D3D12_STATIC_SAMPLER_DESC samplerDesc_{};

	NRootParam rootParams_;	//���[�g�p�����[�^

public:
	//�Q�Ƃ�����̂���
	PipelineSet pipelineSet_;			//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���܂Ƃ߂����

public:
#pragma region �V�F�[�_�[�܂��
	//3D�I�u�W�F�N�g�p���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	void LoadVertShader3d();
	//3D�I�u�W�F�N�g�p�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void LoadPixelShader3d();
	//�X�v���C�g�p���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	void LoadVertShaderSprite();
	//�X�v���C�g�p�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void LoadPixelShaderSprite();
	//�|�X�g�G�t�F�N�g�p���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	void LoadVertShaderPostEffect();
	//�|�X�g�G�t�F�N�g�p�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void LoadPixelShaderPostEffect();
	//�K�E�V�A���u���[�p���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	void LoadVertShaderGaussian();
	//�K�E�V�A���u���[�p�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void LoadPixelShaderGaussian();
	//���W�A���u���[�p���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	void LoadVertShaderRadial();
	//���W�A���u���[�p�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void LoadPixelShaderRadial();
#pragma endregion
#pragma region ���_���C�A�E�g�܂��
	//3D�I�u�W�F�N�g�p���_���C�A�E�g�ݒ�
	void SetVertLayout3d();
	//�X�v���C�g�p���_���C�A�E�g�ݒ�
	void SetVertLayoutSprite();
	//�|�X�g�G�t�F�N�g�p���_���C�A�E�g�ݒ�
	void SetVertLayoutPostEffect();
#pragma endregion
#pragma region �p�C�v���C���X�e�[�g�܂��
	//�V�F�[�_�[�̐ݒ�(�K�p)
	void SetShader();
	//���X�^���C�U�[�ݒ�
	void SetRasterizer(const bool isCull);
	//�u�����h�ݒ�
	void SetBlend(const bool is3d);
	//���̓��C�A�E�g�̐ݒ�
	void SetInputLayout(const bool is3d);
	void SetInputLayoutPostEffect();
	//�}�`�̌`��ݒ�
	void SetTopology();
	//�f�v�X�X�e���V��(�[�x)�ݒ�
	void SetDepth(const bool isDepth);
	//�����_�[�^�[�Q�b�g�ݒ�
	//RTNum->�������_�[�^�[�Q�b�g�K�p���邩
	void SetRenderTarget(const uint32_t RTNum);
	//�A���`�G�C���A�V���O�̂��߂̃T���v�����ݒ�
	void SetAntiAliasing();
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	//texNum->�e�N�X�`�����W�X�^�̐�
	//constantNum->�萔���W�X�^�̐�
	void SetRootSignature(const uint32_t texNum, const uint32_t constantNum);
	//�O���t�B�b�N�X�p�C�v���C���X�e�[�g�I�u�W�F�N�g�̐���
	void CreatePS();
#pragma endregion
#pragma region �e�N�X�`���T���v���[
	//�e�N�X�`���T���v���[�ݒ�
	void SetTexSampler();
#pragma endregion
#pragma region �p�C�v���C������
	//3D�I�u�W�F�N�g�p�p�C�v���C������
	PipelineSet CreatePipeline3d();
	//�X�v���C�g�p�p�C�v���C������
	PipelineSet CreatePipelineSprite();
	//�|�X�g�G�t�F�N�g�p�p�C�v���C������
	PipelineSet CreatePipelinePostEffect();
	//�K�E�V�A���u���[�p�p�C�v���C������
	PipelineSet CreatePipelineGaussian();
	//���W�A���u���[�p�p�C�v���C������
	PipelineSet CreatePipelineRadial();
#pragma endregion

private:

};


class PipeLineManager
{
private:
	NGPipeline pipeline3d_;
	NGPipeline pipelineSprite_;
	NGPipeline pipelinePostEffect_;

	NGPipeline pipelineGaussian_;	//�K�E�V�A���u���[�p
	NGPipeline pipelineRadial_;		//���W�A���u���[�p

	PipelineSet pipelineSet3d_;
	PipelineSet pipelineSetSprite_;
	PipelineSet pipelineSetPostEffect_;

	PipelineSet pipelineSetGaussian_;	//�K�E�V�A���u���[�p
	PipelineSet pipelineSetRadial_;		//���W�A���u���[�p

public:
	PipeLineManager();
	~PipeLineManager();

	static PipeLineManager* GetInstance();
	void Init();

	const PipelineSet& GetPipelineSet(std::string name) const;
};
