#include "NGPipeline.h"

std::map<std::string, NGPipeline> psoMap;	//�p�C�v���C���X�e�[�g�̃}�b�v

using bDesc = PipelineDesc::Blend::BlendDesc;	//�������邩��ȗ�

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
	psDesc_.VS.pShaderBytecode = desc.shader.pShader->GetVSBlob()->GetBufferPointer();
	psDesc_.VS.BytecodeLength = desc.shader.pShader->GetVSBlob()->GetBufferSize();
	psDesc_.PS.pShaderBytecode = desc.shader.pShader->GetPSBlob()->GetBufferPointer();
	psDesc_.PS.BytecodeLength = desc.shader.pShader->GetPSBlob()->GetBufferSize();
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
	psDesc_.RasterizerState.CullMode = desc.render.RasterizerState.CullMode;

	// �|���S�����h��Ԃ����邩
	psDesc_.RasterizerState.FillMode = desc.render.RasterizerState.FillMode;
	// �[�x�N���b�s���O��L���ɂ��邩
	psDesc_.RasterizerState.DepthClipEnable = desc.render.RasterizerState.DepthClipEnable;

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
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = psDesc_.BlendState.RenderTarget[0];
	blendDesc.BlendEnable = desc.blend.isBlend;				//�u�����h��L���ɂ��邩
	blendDesc.LogicOpEnable = desc.blend.isLogicOp;			//�_�����Z���邩
	blendDesc.RenderTargetWriteMask =
		desc.blend.RenderTargetWriteMask;											//�}�X�N�l

	//�w�肵�Ă��u�����h����ݒ�
	blendDesc.BlendOpAlpha = desc.blend.blendDesc.BlendOpAlpha;
	blendDesc.SrcBlendAlpha = desc.blend.blendDesc.SrcBlendAlpha;
	blendDesc.DestBlendAlpha = desc.blend.blendDesc.DestBlendAlpha;

	blendDesc.BlendOp = desc.blend.blendDesc.BlendOp;
	blendDesc.SrcBlend = desc.blend.blendDesc.SrcBlend;
	blendDesc.DestBlend = desc.blend.blendDesc.DestBlend;

	//�ݒ肵���u�����h��K�p(�����_�[�^�[�Q�b�g�̐�����)
	//�����_�[�^�[�Q�b�g���ƂɃu�����h���[�h�ς����悤�ɂ������ȁ`(��])
	for (uint32_t i = 0; i < (uint32_t)psDesc_.NumRenderTargets; i++)
	{
		psDesc_.BlendState.RenderTarget[i] = blendDesc;
	}

	//-------------------------------- �[�x���̐ݒ� --------------------------------//
	psDesc_.DepthStencilState.DepthEnable = desc.depth.DepthStencilState.DepthEnable;
	psDesc_.DepthStencilState.DepthWriteMask = desc.depth.DepthStencilState.DepthWriteMask;
	psDesc_.DepthStencilState.DepthFunc = desc.depth.DepthStencilState.DepthFunc;
	psDesc_.DSVFormat = desc.depth.DSVFormat;

	//-------------------------------- ���[�g�V�O�l�`���̐ݒ�@--------------------------------//
	psDesc_.pRootSignature = desc.rootSig.GetRootSignature();
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
	vertLayoutPostEffect_[1] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
}

void NGPipeline::SetVertLayoutParticle()
{
	// ���_���C�A�E�g
	//���W
	vertLayoutParticle_[0] = {
	"POSITION",										//�Z�}���e�B�b�N��
	0,												//�����̃Z�}���e�B�b�N������Ƃ��g���C���f�b�N�X
	DXGI_FORMAT_R32G32B32_FLOAT,					//�v�f���ƃr�b�g����\��
	0,												//���̓X���b�g�C���f�b�N�X
	D3D12_APPEND_ALIGNED_ELEMENT,					//�f�[�^�̃I�t�Z�b�g�n(���͎̂����ݒ�)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//���̓f�[�^���
	0												//��x�ɕ`�悷��C���X�^���X��(0�ł悢)
	};// (1�s�ŏ������ق������₷��)
	//���W�ȊO�ɐF�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	//��]���
	vertLayoutParticle_[1] = {
		"ROT",0,
		DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	//�F
	vertLayoutParticle_[2] = {
		"COLOR",0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	//�傫��
	vertLayoutParticle_[3] = {
		"TEXCOORD", 0,
		DXGI_FORMAT_R32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
}

PipeLineManager* PipeLineManager::GetInstance()
{
	static PipeLineManager instance;
	return &instance;
}

void PipeLineManager::CreateAll()
{
	NGPipeline pipeLine;		//���_���C�A�E�g�ݒ�p
	pipeLine.SetVertLayoutObj();
	pipeLine.SetVertLayoutSprite();
	pipeLine.SetVertLayoutPostEffect();
	pipeLine.SetVertLayoutParticle();
#pragma region �f�t�H���g3D
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("Obj", "Obj", false);

	PipelineDesc objDesc;
	//���_���C�A�E�g�ݒ�
	objDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	objDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigObj;
	rootSigObj.SetSamplerDesc(false);
	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A����
	rootSigObj.SetRootParam(1,4);
	rootSigObj.Create();
	objDesc.rootSig = rootSigObj;

	//�V�F�[�_�[�ݒ�
	objDesc.shader.pShader = NShader::GetInstance()->GetShader("Obj");

	//�J�����O�ݒ�
	objDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
	//�����_�[�^�[�Q�b�g���ݒ�
	objDesc.render.NumRenderTargets = 2;

	//�[�x�e�X�g����
	objDesc.depth.DepthStencilState.DepthEnable = true;

	//�p�C�v���C������
	NGPipeline::Create(objDesc, "Obj");
#pragma endregion
#pragma region �^�C�����O3D(�w�i�I�u�W�F�N�g�Ƃ��Ɏg��)
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("Tile", "Tile", false);

	PipelineDesc tileDesc;
	//���_���C�A�E�g�ݒ�
	tileDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	tileDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigTile;
	rootSigTile.SetSamplerDesc(true);
	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A����
	rootSigTile.SetRootParam(1, 4);
	rootSigTile.Create();
	tileDesc.rootSig = rootSigTile;

	//�V�F�[�_�[�ݒ�
	tileDesc.shader.pShader = NShader::GetInstance()->GetShader("Tile");

	//�J�����O�ݒ�
	tileDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
	//�����_�[�^�[�Q�b�g���ݒ�
	tileDesc.render.NumRenderTargets = 2;

	//�[�x�e�X�g����
	tileDesc.depth.DepthStencilState.DepthEnable = true;
	
	//�p�C�v���C������
	NGPipeline::Create(tileDesc, "TileObj");
#pragma endregion
#pragma region �f�t�H���g2D
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteDesc;
	//���_���C�A�E�g�ݒ�
	spriteDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutSprite_;
	spriteDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutSprite_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigSprite;
	rootSigSprite.SetSamplerDesc(true);
	//�e�N�X�`��1�A�s��A�F
	rootSigSprite.SetRootParam(1, 2);
	rootSigSprite.Create();
	spriteDesc.rootSig = rootSigSprite;

	//�V�F�[�_�[�ݒ�
	spriteDesc.shader.pShader = NShader::GetInstance()->GetShader("Sprite");

	//�[�x���ݒ�
	spriteDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	spriteDesc.depth.DepthStencilState.DepthEnable = false;

	//�����_�[�^�[�Q�b�g���ݒ�
	spriteDesc.render.NumRenderTargets = 2;

	//�J�����O�ݒ�
	spriteDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�p�C�v���C������
	NGPipeline::Create(spriteDesc, "Sprite");
#pragma endregion
#pragma region �|�X�g�G�t�F�N�g
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("PostEffect", "CG4", false);

	PipelineDesc postEffectDesc;
	//���_���C�A�E�g�ݒ�
	postEffectDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutPostEffect_;
	postEffectDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutPostEffect_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigPostEffect;
	rootSigPostEffect.SetSamplerDesc(false);
	//�e�N�X�`��2�A�s��A�F
	rootSigPostEffect.SetRootParam(2, 2);
	rootSigPostEffect.Create();
	postEffectDesc.rootSig = rootSigPostEffect;

	//�V�F�[�_�[�ݒ�
	postEffectDesc.shader.pShader = NShader::GetInstance()->GetShader("Sprite");

	//�[�x���ݒ�
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//�����_�[�^�[�Q�b�g���ݒ�
	postEffectDesc.render.NumRenderTargets = 2;

	//�J�����O�ݒ�
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�p�C�v���C������
	NGPipeline::Create(postEffectDesc, "PostEffect");
#pragma endregion
#pragma region �K�E�V�A���u���[
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("Gaussian", "GaussianBlur", false);

	//���_���C�A�E�g�ݒ�
	postEffectDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutPostEffect_;
	postEffectDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutPostEffect_);

	//���[�g�V�O�l�`���ݒ�
	postEffectDesc.rootSig = rootSigPostEffect;

	//�V�F�[�_�[�ݒ�
	postEffectDesc.shader.pShader = NShader::GetInstance()->GetShader("Sprite");

	//�[�x���ݒ�
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//�J�����O�ݒ�
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�p�C�v���C������
	NGPipeline::Create(postEffectDesc, "Gaussian");
#pragma endregion
#pragma region ���W�A���u���[
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("Radial", "RadialBlur", false);

	//���_���C�A�E�g�ݒ�
	postEffectDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutPostEffect_;
	postEffectDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutPostEffect_);

	//���[�g�V�O�l�`���ݒ�
	postEffectDesc.rootSig = rootSigPostEffect;

	//�V�F�[�_�[�ݒ�
	postEffectDesc.shader.pShader = NShader::GetInstance()->GetShader("Sprite");

	//�[�x���ݒ�
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//�J�����O�ݒ�
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�p�C�v���C������
	NGPipeline::Create(postEffectDesc, "Radial");
#pragma endregion
#pragma region �p�[�e�B�N��3D
	//�V�F�[�_�[����
	NShader::GetInstance()->CreateShader("Particle3d", "Particle3D", true);

	PipelineDesc particleDesc;
	//���_���C�A�E�g�ݒ�
	particleDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutParticle_;
	particleDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutParticle_);

	particleDesc.render.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//particleDesc.blend.blendDesc = 
	//	bDesc::GetBlendMode(bDesc::BlendMode::Add);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigParticle;
	rootSigParticle.SetSamplerDesc(false);
	//�e�N�X�`��2�A�s��A����
	rootSigParticle.SetRootParam(2, 2);
	rootSigParticle.Create();
	particleDesc.rootSig = rootSigParticle;

	//�V�F�[�_�[�ݒ�
	particleDesc.shader.pShader = NShader::GetInstance()->GetShader("Particle3d");

	//�[�x�e�X�g����
	particleDesc.depth.DepthStencilState.DepthEnable = true;

	//�J�����O�ݒ�
	particleDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�p�C�v���C������
	NGPipeline::Create(particleDesc, "Particle3d");
#pragma endregion
}

bDesc bDesc::GetBlendMode(BlendMode blendMode)
{
	bDesc desc;
	switch (blendMode)
	{
	case BlendMode::Alpha:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//���Z
		desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				//�\�[�X�̃A���t�@�l
		desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;			//1.0f-�\�[�X�̃A���t�@�l
		break;

	case BlendMode::Add:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//���Z
		desc.SrcBlend = D3D12_BLEND_ONE;					//�\�[�X�̒l��100%�g��
		desc.DestBlend = D3D12_BLEND_ONE;					//�f�X�g�̒l��100%�g��
		break;

	case BlendMode::Sub:
		desc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;			//���Z
		desc.SrcBlend = D3D12_BLEND_ONE;					//�\�[�X�̒l��100%�g��
		desc.DestBlend = D3D12_BLEND_ONE;					//�f�X�g�̒l��100%�g��

		desc.BlendOpAlpha = D3D12_BLEND_OP_REV_SUBTRACT;	//���Z
		desc.SrcBlendAlpha = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
		desc.DestBlendAlpha = D3D12_BLEND_ONE;				//�f�X�g�̒l��100%�g��
		break;

	case BlendMode::Inv:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//���Z
		desc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;			//�F���](1-RGB)
		desc.DestBlend = D3D12_BLEND_ZERO;					//�f�X�g�̒l��0%�g��
		break;

	default:
		break;
	}

	return desc;
}