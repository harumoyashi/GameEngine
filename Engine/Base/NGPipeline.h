#pragma once
#include <DirectXMath.h>
#include "NDX12.h"
#include "NTexture.h"
#include "NRootSignature.h"
#include "NRootParam.h"
#include "NShader.h"
#include <d3dx12.h>

#include <wrl.h>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

struct PipelineDesc
{
	struct Shader {
		NShader* pShader;
	} shader;

	struct Blend {
		//�ԗ����l�����ău�����h���邩
		bool isAlphaToCoverage = false;
		//���ꂼ��̃����_�[�^�[�Q�b�g�ɕʁX�̃u�����h���邩
		bool isIndependentBlend = false;
		//�}�X�N�l�FRBGA�S�Ẵ`�����l����`��
		UINT8 RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		//�u�����h��L���ɂ��邩�t���O
		bool isBlend = true;
		//�_�����Z��L���ɂ��邩�t���O
		bool isLogicOp = false;

		//�u�����h���
		struct BlendDesc {			//�����������ƃ��u�����h�ɂȂ�
			D3D12_BLEND_OP BlendOpAlpha = D3D12_BLEND_OP_ADD;
			D3D12_BLEND SrcBlendAlpha = D3D12_BLEND_ONE;
			D3D12_BLEND DestBlendAlpha = D3D12_BLEND_ZERO;

			D3D12_BLEND_OP BlendOp = D3D12_BLEND_OP_ADD;
			D3D12_BLEND SrcBlend = D3D12_BLEND_SRC_ALPHA;
			D3D12_BLEND DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		}blendDesc;
	} blend;

	struct Render {
		//�g���C�A���O���X�g���b�v��؂藣�����ǂ���
		D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBStripCutValue =
			D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;		//�J�b�g�Ȃ�
		//�g�|���W�[�w��
		D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopologyType =
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//-------------------- �T���v���}�X�N --------------------//
		UINT SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
		//-------------------- ���X�^���C�U --------------------//
		D3D12_CULL_MODE CullMode = D3D12_CULL_MODE_NONE;	// �w�ʃJ�����O���Ȃ�
		// �|���S�����h��Ԃ�(D3D12_FILL_MODE_WIREFRAME�ɂ���ƃ��C���[�t���[����)
		D3D12_FILL_MODE FillMode = D3D12_FILL_MODE_SOLID;
		// �[�x�N���b�s���O��L����
		bool isDepthClip = true;

		//�����_�[�^�[�Q�b�g��
		UINT NumRenderTargets = 1;	//�`��Ώ�1��(�}���`�����_�[�^�[�Q�b�g���Ȃ瑝�₷)
		//�����_�[�^�[�Q�b�g���ɂ���đ��₷�\������
		DXGI_FORMAT RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;	//0~255�w���RGBA
		//�A���`�G�C���A�V���O�̂��߂̃T���v����
		DXGI_SAMPLE_DESC SampleDesc = DXGI_SAMPLE_DESC{
			1,	//1�s�N�Z���ɂ�1��T���v�����O
			0	//�Œ�N�I���e�B
		};

		//�����̓p�C�v���C���}�l�[�W���[���Őݒ肷��
		D3D12_INPUT_LAYOUT_DESC InputLayout;
	} render;

	struct Depth {
		//-------------------- �f�v�X�X�e���V���X�e�[�g --------------------//
		bool isDepth = true;	//�[�x�e�X�g���邩
		D3D12_DEPTH_WRITE_MASK DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
		D3D12_COMPARISON_FUNC DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//��������΍��i

		DXGI_FORMAT DSVFormat = DXGI_FORMAT_D32_FLOAT;						//�[�x�l�t�H�[�}�b�g
	} depth;

	NRootSignature rootSig_;
};

class NGPipeline
{
public:
	static void Create(PipelineDesc desc, std::string id);
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetDesc(std::string id);
	static ID3D12PipelineState* GetState(std::string id);
	static NGPipeline* GetGPipeline(std::string id);

	NGPipeline() {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc_{};
	ComPtr<ID3D12PipelineState> pso_ = nullptr;

private:
	void Create();

private:
	//�p�C�v���C���f�X�N�̐ݒ�
	void SetDesc(PipelineDesc desc);

private:
	//�V�F�[�_�[���\�[�X�r���[//
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};	//�ݒ�\����

	//�p�C�v���C���X�e�[�g//
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc_ = {};

	//�e�N�X�`���T���v���[//
	D3D12_STATIC_SAMPLER_DESC samplerDesc_{};

	NRootParam rootParams_;	//���[�g�p�����[�^

public:


public:
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
	void SetTexSampler(const bool isTiling);
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
	//�^�C���p�p�C�v���C������
	PipelineSet CreatePipelineTile();
#pragma endregion

private:

};


class PipeLineManager
{
public:
	static PipeLineManager* GetInstance();
	//�p�C�v���C���S�����
	void CreateAll();
};

//�u�����h���[�h�������₷������p
namespace BlendUtil
{
	enum class BlendMode
	{
		Alpha,	//���u�����h
		Add,	//���Z
		Sub,	//���Z
		Inv,	//���]
	};

	static PipelineDesc::Blend::BlendDesc GetBlendMode(BlendMode blendMode);
};