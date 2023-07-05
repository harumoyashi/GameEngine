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
		D3D12_RASTERIZER_DESC RasterizerState = D3D12_RASTERIZER_DESC{
			D3D12_FILL_MODE_SOLID,	// �|���S�����h��Ԃ�(D3D12_FILL_MODE_WIREFRAME�ɂ���ƃ��C���[�t���[����)
			D3D12_CULL_MODE_NONE,	// �w�ʃJ�����O���Ȃ�
			0,
			0,
			0.f,
			0.f,
			true,					// �[�x�N���b�s���O��L����
			0,
			0,
			0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		};

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
		D3D12_INPUT_LAYOUT_DESC InputLayout{};
	} render;

	struct Depth {
		//-------------------- �f�v�X�X�e���V���X�e�[�g --------------------//
		D3D12_DEPTH_STENCIL_DESC DepthStencilState = D3D12_DEPTH_STENCIL_DESC{
			true,							//�[�x�e�X�g���邩
			D3D12_DEPTH_WRITE_MASK_ALL,		//�������݋���
			D3D12_COMPARISON_FUNC_LESS,		//��������΍��i
			false,
			0,
			0,
			D3D12_DEPTH_STENCILOP_DESC{},
			D3D12_DEPTH_STENCILOP_DESC{}
		};

		DXGI_FORMAT DSVFormat = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	} depth;

	NRootSignature rootSig;
};

class NGPipeline
{
public:
	//�p�C�v���C���f�X�N
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc_{};
	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> pso_ = nullptr;

public:
	//���_���C�A�E�g����(�K�v�ȕ������p�ӂ���)
	D3D12_INPUT_ELEMENT_DESC vertLayoutObj_[3];
	D3D12_INPUT_ELEMENT_DESC vertLayoutSprite_[2];
	D3D12_INPUT_ELEMENT_DESC vertLayoutPostEffect_[2];
	D3D12_INPUT_ELEMENT_DESC vertLayoutParticle_[3];

public:
	//�p�C�v���C�������A�w�肵��ID�œo�^
	static void Create(PipelineDesc desc, std::string id);
	//�w�肵��ID�̃p�C�v���C���f�X�N���擾
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetDesc(std::string id);
	//�w�肵��ID�̃p�C�v���C���X�e�[�g���擾
	static ID3D12PipelineState* GetState(std::string id);
	//�w�肵��ID�̃p�C�v���C�����擾
	static NGPipeline* GetGPipeline(std::string id);

	NGPipeline() {};

private:
	//�p�C�v���C������
	void Create();
	//�p�C�v���C���f�X�N�̐ݒ�
	void SetDesc(PipelineDesc desc);

public:
#pragma region ���_���C�A�E�g�܂��
	//3D�I�u�W�F�N�g�p���_���C�A�E�g�ݒ�
	void SetVertLayoutObj();
	//�X�v���C�g�p���_���C�A�E�g�ݒ�
	void SetVertLayoutSprite();
	//�|�X�g�G�t�F�N�g�p���_���C�A�E�g�ݒ�
	void SetVertLayoutPostEffect();
	//3D�p�[�e�B�N���p���_���C�A�E�g�ݒ�
	void SetVertLayoutParticle();
#pragma endregion
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

	//�w�肵���u�����h���[�h�̃u�����h�����擾
	static PipelineDesc::Blend::BlendDesc GetBlendMode(BlendMode blendMode);
};