#include "NGPipeline.h"

std::map<std::string, NGPipeline> psoMap;	//�p�C�v���C���X�e�[�g�̃}�b�v

void NGPipeline::Create(PipelineDesc desc, std::string id)
{
	psoMap[id] = NGPipeline();
	NGPipeline* ptr = &psoMap[id];
	ptr->SetDesc(desc);
	ptr->Create();
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC* NGPipeline::GetDesc(std::string id)
{
	return &psoMap[id].psDesc_;
}

ID3D12PipelineState* NGPipeline::GetState(std::string id)
{
	return psoMap[id].pso_.Get();
}

NGPipeline* NGPipeline::GetGPipeline(std::string id)
{
	return &psoMap[id];
}

void NGPipeline::Create()
{
	NDX12::GetInstance()->GetDevice()->
		CreateGraphicsPipelineState(&psDesc_, IID_PPV_ARGS(&pso_));
}

void NGPipeline::SetDesc(PipelineDesc desc)
{
	//-------------------------------- �V�F�[�_�[�̐ݒ� --------------------------------//
	pipelineDesc_.VS.pShaderBytecode = desc.shader.pShader->GetVSBlob()->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = desc.shader.pShader->GetVSBlob()->GetBufferSize();
	pipelineDesc_.PS.pShaderBytecode = desc.shader.pShader->GetPSBlob()->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = desc.shader.pShader->GetPSBlob()->GetBufferSize();
	if (desc.shader.pShader->GetGSBlob() != nullptr)	//�W�I���g���V�F�[�_�[������Ȃ�
	{
		psDesc_.GS.pShaderBytecode = desc.shader.pShader->GetGSBlob()->GetBufferPointer();
		psDesc_.GS.BytecodeLength = desc.shader.pShader->GetGSBlob()->GetBufferSize();
	}

	//-------------------------------- �}�`�̌`��ݒ� --------------------------------//
	//�g���C�A���O���X�g���b�v��؂藣�����ǂ���
	psDesc_.IBStripCutValue = desc.render.IBStripCutValue;
	//�g�|���W�[�w��
	psDesc_.PrimitiveTopologyType = desc.render.PrimitiveTopologyType;

	//-------------------------------- ���X�^���C�U�̐ݒ� --------------------------------//
	// �T���v���}�X�N�̐ݒ�
	psDesc_.SampleMask = desc.render.SampleMask;
	// ���X�^���C�U�̐ݒ�
	psDesc_.RasterizerState.CullMode = desc.render.CullMode;

	// �|���S�����h��Ԃ����邩
	psDesc_.RasterizerState.FillMode = desc.render.FillMode;
	// �[�x�N���b�s���O��L���ɂ��邩
	psDesc_.RasterizerState.DepthClipEnable = desc.render.isDepthClip;

	//-------------------------------- �����_�[�^�[�Q�b�g���̐ݒ� --------------------------------//
	psDesc_.NumRenderTargets = desc.render.NumRenderTargets;
	for (uint32_t i = 0; i < (uint32_t)psDesc_.NumRenderTargets; i++)
	{
		psDesc_.RTVFormats[i] = desc.render.RTVFormat;
	}

	//-------------------------------- �T���v�����O���̐ݒ� --------------------------------//
	psDesc_.SampleDesc = desc.render.SampleDesc;

	//-------------------------------- ���_���C�A�E�g�̐ݒ� --------------------------------//
	psDesc_.InputLayout = desc.render.InputLayout;

	//-------------------------------- �u�����h�f�X�N�̐ݒ� --------------------------------//
	psDesc_.BlendState.AlphaToCoverageEnable = desc.blend.isAlphaToCoverage;		//�ԗ����l�����ău�����h���邩
	psDesc_.BlendState.IndependentBlendEnable = desc.blend.isIndependentBlend;		//���ꂼ��̃����_�[�^�[�Q�b�g�ɕʁX�̃u�����h���邩

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	psDesc_.BlendState.RenderTarget->BlendEnable = desc.blend.isBlend;				//�u�����h��L���ɂ��邩
	psDesc_.BlendState.RenderTarget->LogicOpEnable = desc.blend.isLogicOp;			//�_�����Z���邩
	psDesc_.BlendState.RenderTarget->RenderTargetWriteMask =
		desc.blend.RenderTargetWriteMask;											//�}�X�N�l

	//�w�肵�Ă��u�����h����ݒ�
	psDesc_.BlendState.RenderTarget->BlendOpAlpha = desc.blend.blendDesc.BlendOpAlpha;
	psDesc_.BlendState.RenderTarget->SrcBlendAlpha = desc.blend.blendDesc.SrcBlendAlpha;
	psDesc_.BlendState.RenderTarget->DestBlendAlpha = desc.blend.blendDesc.DestBlendAlpha;

	psDesc_.BlendState.RenderTarget->BlendOp = desc.blend.blendDesc.BlendOp;
	psDesc_.BlendState.RenderTarget->SrcBlend = desc.blend.blendDesc.SrcBlend;
	psDesc_.BlendState.RenderTarget->DestBlend = desc.blend.blendDesc.DestBlend;

	//�ݒ肵���u�����h��K�p(�����_�[�^�[�Q�b�g�̐�����)
	//�����_�[�^�[�Q�b�g���ƂɃu�����h���[�h�ς����悤�ɂ������ȁ`(��])
	for (uint32_t i = 0; i < (uint32_t)psDesc_.NumRenderTargets; i++)
	{
		psDesc_.BlendState.RenderTarget[i] = blendDesc_;
	}

	//-------------------------------- �[�x���̐ݒ� --------------------------------//
	psDesc_.DepthStencilState.DepthEnable = desc.depth.isDepth;
	psDesc_.DepthStencilState.DepthWriteMask = desc.depth.DepthWriteMask;
	psDesc_.DepthStencilState.DepthFunc = desc.depth.DepthFunc;
	psDesc_.DSVFormat = desc.depth.DSVFormat;

	//-------------------------------- ���[�g�V�O�l�`���̐ݒ�@--------------------------------//
	psDesc_.pRootSignature = desc.rootSig.entity_.Get();
}

void NGPipeline::SetVertLayoutObj()
{
	// ���_���C�A�E�g
	//���W
	vertLayoutObj_[0] = {
	"POSITION",										//�Z�}���e�B�b�N��
	0,												//�����̃Z�}���e�B�b�N������Ƃ��g���C���f�b�N�X
	DXGI_FORMAT_R32G32B32_FLOAT,					//�v�f���ƃr�b�g����\��
	0,												//���̓X���b�g�C���f�b�N�X
	D3D12_APPEND_ALIGNED_ELEMENT,					//�f�[�^�̃I�t�Z�b�g�n(���͎̂����ݒ�)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//���̓f�[�^���
	0												//��x�ɕ`�悷��C���X�^���X��(0�ł悢)
	};// (1�s�ŏ������ق������₷��)
	//���W�ȊO�ɐF�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	//�@���x�N�g��
	vertLayoutObj_[1] = {
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	//UV
	vertLayoutObj_[2] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
}

void NGPipeline::SetVertLayoutSprite()
{
	// ���_���C�A�E�g
	//���W
	vertLayoutSprite_[0] = {
	"POSITION",										//�Z�}���e�B�b�N��
	0,												//�����̃Z�}���e�B�b�N������Ƃ��g���C���f�b�N�X
	DXGI_FORMAT_R32G32B32_FLOAT,					//�v�f���ƃr�b�g����\��
	0,												//���̓X���b�g�C���f�b�N�X
	D3D12_APPEND_ALIGNED_ELEMENT,					//�f�[�^�̃I�t�Z�b�g�n(���͎̂����ݒ�)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//���̓f�[�^���
	0												//��x�ɕ`�悷��C���X�^���X��(0�ł悢)
	};// (1�s�ŏ������ق������₷��)
	//���W�ȊO�ɐF�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	//UV
	vertLayoutSprite_[1] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
}

void NGPipeline::SetVertLayoutPostEffect()
{
	// ���_���C�A�E�g
	//���W
	vertLayoutPostEffect_[0] = {
	"SV_POSITION",									//�Z�}���e�B�b�N��
	0,												//�����̃Z�}���e�B�b�N������Ƃ��g���C���f�b�N�X
	DXGI_FORMAT_R32G32B32_FLOAT,					//�v�f���ƃr�b�g����\��
	0,												//���̓X���b�g�C���f�b�N�X
	D3D12_APPEND_ALIGNED_ELEMENT,					//�f�[�^�̃I�t�Z�b�g�n(���͎̂����ݒ�)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//���̓f�[�^���
	0												//��x�ɕ`�悷��C���X�^���X��(0�ł悢)
	};// (1�s�ŏ������ق������₷��)
	//���W�ȊO�ɐF�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	//UV
	vertLayoutPostEffect_[1] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
}

PipelineSet NGPipeline::CreatePipelinePostEffect()
{
	//�V�F�[�_�[
	LoadVertShaderPostEffect();
	LoadPixelShaderPostEffect();

	//���_���C�A�E�g�ݒ�
	SetVertLayoutPostEffect();

	//�p�C�v���C���X�e�[�g
	SetShader();
	SetRasterizer(false);
	SetBlend(false);
	SetInputLayoutPostEffect();
	SetTopology();
	SetRenderTarget(1);
	SetAntiAliasing();
	SetDepth(false);

	//�e�N�X�`���T���v���[�̐ݒ�
	SetTexSampler(false);

	//���[�g�V�O�l�`��
	//�e�N�X�`��2�A�s��A�F
	SetRootSignature(2, 2);

	//�p�C�v���C���X�e�[�g����
	CreatePS();

	return pipelineSet_;
}

PipelineSet NGPipeline::CreatePipelineGaussian()
{
	//�V�F�[�_�[
	LoadVertShaderGaussian();
	LoadPixelShaderGaussian();

	//���_���C�A�E�g�ݒ�
	SetVertLayoutPostEffect();

	//�p�C�v���C���X�e�[�g
	SetShader();
	SetRasterizer(false);
	SetBlend(false);
	SetInputLayoutPostEffect();
	SetTopology();
	SetRenderTarget(1);
	SetAntiAliasing();
	SetDepth(false);

	//�e�N�X�`���T���v���[�̐ݒ�
	SetTexSampler(false);

	//���[�g�V�O�l�`��
	//�e�N�X�`��2�A�s��A�F
	SetRootSignature(2, 2);

	//�p�C�v���C���X�e�[�g����
	CreatePS();

	return pipelineSet_;
}

PipelineSet NGPipeline::CreatePipelineRadial()
{
	//�V�F�[�_�[
	LoadVertShaderRadial();
	LoadPixelShaderRadial();

	//���_���C�A�E�g�ݒ�
	SetVertLayoutPostEffect();

	//�p�C�v���C���X�e�[�g
	SetShader();
	SetRasterizer(false);
	SetBlend(false);
	SetInputLayoutPostEffect();
	SetTopology();
	SetRenderTarget(1);
	SetAntiAliasing();
	SetDepth(false);

	//�e�N�X�`���T���v���[�̐ݒ�
	SetTexSampler(false);

	//���[�g�V�O�l�`��
	//�e�N�X�`��2�A�s��A�F
	SetRootSignature(2, 2);

	//�p�C�v���C���X�e�[�g����
	CreatePS();

	return pipelineSet_;
}

PipelineSet NGPipeline::CreatePipelineTile()
{
	//�V�F�[�_�[
	LoadVertShaderTile();
	LoadPixelShaderTile();

	//���_���C�A�E�g�ݒ�
	SetVertLayout3d();

	//�p�C�v���C���X�e�[�g
	SetShader();
	SetRasterizer(true);
	SetBlend(true);
	SetInputLayout(true);
	SetTopology();
	SetRenderTarget(2);
	SetAntiAliasing();
	SetDepth(true);

	//�e�N�X�`���T���v���[�̐ݒ�
	SetTexSampler(true);

	//���[�g�V�O�l�`��
	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A����
	SetRootSignature(1, 4);

	//�p�C�v���C���X�e�[�g����
	CreatePS();

	return pipelineSet_;
}

PipeLineManager::PipeLineManager()
{
}

PipeLineManager::~PipeLineManager()
{
}

PipeLineManager* PipeLineManager::GetInstance()
{
	static PipeLineManager instance;
	return &instance;
}

void PipeLineManager::CreateAll()
{
#pragma region �f�t�H���g3D
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("obj", "Obj", false);

	PipelineDesc objDesc;
	NGPipeline objPipeLine;
	//���_���C�A�E�g�ݒ�
	objPipeLine.SetVertLayoutObj();
	objDesc.render.InputLayout.pInputElementDescs = objPipeLine.vertLayoutObj_;
	objDesc.render.InputLayout.NumElements = _countof(objPipeLine.vertLayoutObj_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSig;
	rootSig.SetSamplerDesc(false);
	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A����
	rootSig.GetRootParam().SetRootParam(1,4);
	rootSig.Create();
	objDesc.rootSig = rootSig;

	//�V�F�[�_�[�ݒ�
	objDesc.shader.pShader = NShader::GetInstance()->GetShader("obj");

	//�J�����O�ݒ�
	objDesc.render.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
	//�����_�[�^�[�Q�b�g���ݒ�
	objDesc.render.NumRenderTargets = 2;

	//�[�x�e�X�g����
	objDesc.depth.isDepth = true;

	//�p�C�v���C������
	NGPipeline::Create(objDesc, "obj");
#pragma endregion
#pragma region �f�t�H���g2D
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteDesc;
	NGPipeline spritePipeLine;
	//���_���C�A�E�g�ݒ�
	spriteDesc.render.InputLayout.pInputElementDescs = spritePipeLine.vertLayoutSprite_;
	spriteDesc.render.InputLayout.NumElements = _countof(spritePipeLine.vertLayoutSprite_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSig;
	rootSig.SetSamplerDesc(false);
	//�e�N�X�`��1�A�s��A�}�e���A���A�F
	rootSig.GetRootParam().SetRootParam(1, 3);
	rootSig.Create();
	spriteDesc.rootSig = rootSig;

	//�V�F�[�_�[�ݒ�
	spriteDesc.shader.pShader = NShader::GetInstance()->GetShader("sprite");

	//�[�x���ݒ�
	spriteDesc.depth.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	spriteDesc.depth.isDepth = false;

	//�J�����O�ݒ�
	spriteDesc.render.CullMode = D3D12_CULL_MODE_NONE;

	//�p�C�v���C������
	NGPipeline::Create(spriteDesc, "sprite");
#pragma endregion
#pragma region �|�X�g�G�t�F�N�g
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("PostEffect", "CG4", false);
#pragma endregion
#pragma region �K�E�V�A���u���[
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("Gaussian", "GaussianBlur", false);
#pragma endregion
#pragma region ���W�A���u���[
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("Radial", "RadialBlur", false);
#pragma endregion
#pragma region �p�[�e�B�N��3D
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("Particle3d", "Particle3D", true);
#pragma endregion
}

PipelineDesc::Blend::BlendDesc BlendUtil::GetBlendMode(BlendMode blendMode)
{
	PipelineDesc::Blend::BlendDesc desc;
	switch (blendMode)
	{
	case BlendUtil::BlendMode::Alpha:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//���Z
		desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				//�\�[�X�̃A���t�@�l
		desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;			//1.0f-�\�[�X�̃A���t�@�l
		break;

	case BlendUtil::BlendMode::Add:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//���Z
		desc.SrcBlend = D3D12_BLEND_ONE;					//�\�[�X�̒l��100%�g��
		desc.DestBlend = D3D12_BLEND_ONE;					//�f�X�g�̒l��100%�g��
		break;

	case BlendUtil::BlendMode::Sub:
		desc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;			//���Z
		desc.SrcBlend = D3D12_BLEND_ONE;					//�\�[�X�̒l��100%�g��
		desc.DestBlend = D3D12_BLEND_ONE;					//�f�X�g�̒l��100%�g��

		desc.BlendOpAlpha = D3D12_BLEND_OP_REV_SUBTRACT;	//���Z
		desc.SrcBlendAlpha = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
		desc.DestBlendAlpha = D3D12_BLEND_ONE;				//�f�X�g�̒l��100%�g��
		break;

	case BlendUtil::BlendMode::Inv:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//���Z
		desc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;			//�F���](1-RGB)
		desc.DestBlend = D3D12_BLEND_ZERO;					//�f�X�g�̒l��0%�g��
		break;

	default:
		break;
	}

	return desc;
}
