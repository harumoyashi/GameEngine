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

void NGPipeline::SetVertLayoutFbx()
{
	// ���_���C�A�E�g
	//���W
	vertLayoutFbx_[0] = {
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
	vertLayoutFbx_[1] = {
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	//UV
	vertLayoutFbx_[2] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	//���̎��ʔԍ�
	vertLayoutFbx_[3] = {
		"BONEINDICES",0,
		DXGI_FORMAT_R32G32B32A32_UINT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	//�����^����e���x
	vertLayoutFbx_[4] = {
		"BONEWEIGHTS",0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,0,
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
	pipeLine.SetVertLayoutFbx();
	pipeLine.SetVertLayoutSprite();
	pipeLine.SetVertLayoutPostEffect();
	pipeLine.SetVertLayoutParticle();
#pragma region �f�t�H���g3D
	//�V�F�[�_�[����
	NShader::CreateShader("Obj", "Obj", false);

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
	objDesc.shader.pShader = NShader::GetShader("Obj");

	//�J�����O�ݒ�
	objDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
	//�����_�[�^�[�Q�b�g���ݒ�
	objDesc.render.NumRenderTargets = 2;

	//�[�x�e�X�g����
	objDesc.depth.DepthStencilState.DepthEnable = true;

	//�u�����h���[�h�ݒ�
	objDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//�p�C�v���C������
	NGPipeline::Create(objDesc, "ObjNone");
#pragma endregion
#pragma region ���u�����h3D
	//�V�F�[�_�[����
	NShader::CreateShader("Obj", "Obj", false);

	PipelineDesc objAlphaDesc;
	//���_���C�A�E�g�ݒ�
	objAlphaDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	objAlphaDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigObjAlpha;
	rootSigObjAlpha.SetSamplerDesc(false);
	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A����
	rootSigObjAlpha.SetRootParam(1, 4);
	rootSigObjAlpha.Create();
	objAlphaDesc.rootSig = rootSigObjAlpha;

	//�V�F�[�_�[�ݒ�
	objAlphaDesc.shader.pShader = NShader::GetShader("Obj");

	//�J�����O�ݒ�
	objAlphaDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
	//�����_�[�^�[�Q�b�g���ݒ�
	objAlphaDesc.render.NumRenderTargets = 2;

	//�[�x�e�X�g����
	objAlphaDesc.depth.DepthStencilState.DepthEnable = true;

	//�u�����h���[�h�ݒ�
	objAlphaDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Alpha);

	//�p�C�v���C������
	NGPipeline::Create(objAlphaDesc, "ObjAlpha");
#pragma endregion
#pragma region ���Z3D
	//�V�F�[�_�[����
	NShader::CreateShader("Obj", "Obj", false);

	PipelineDesc objAddDesc;
	//���_���C�A�E�g�ݒ�
	objAddDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	objAddDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigObjAdd;
	rootSigObjAdd.SetSamplerDesc(false);
	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A����
	rootSigObjAdd.SetRootParam(1, 4);
	rootSigObjAdd.Create();
	objAddDesc.rootSig = rootSigObjAdd;

	//�V�F�[�_�[�ݒ�
	objAddDesc.shader.pShader = NShader::GetShader("Obj");

	//�J�����O�ݒ�
	objAddDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
	//�����_�[�^�[�Q�b�g���ݒ�
	objAddDesc.render.NumRenderTargets = 2;

	//�[�x�e�X�g����
	objAddDesc.depth.DepthStencilState.DepthEnable = true;

	//�u�����h���[�h�ݒ�
	objAddDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Add);

	//�p�C�v���C������
	NGPipeline::Create(objAddDesc, "ObjAdd");
#pragma endregion
#pragma region ���Z3D
	//�V�F�[�_�[����
	NShader::CreateShader("Obj", "Obj", false);

	PipelineDesc objSubDesc;
	//���_���C�A�E�g�ݒ�
	objSubDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	objSubDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigObjSub;
	rootSigObjSub.SetSamplerDesc(false);
	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A����
	rootSigObjSub.SetRootParam(1, 4);
	rootSigObjSub.Create();
	objSubDesc.rootSig = rootSigObjSub;

	//�V�F�[�_�[�ݒ�
	objSubDesc.shader.pShader = NShader::GetShader("Obj");

	//�J�����O�ݒ�
	objSubDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
	//�����_�[�^�[�Q�b�g���ݒ�
	objSubDesc.render.NumRenderTargets = 2;

	//�[�x�e�X�g����
	objSubDesc.depth.DepthStencilState.DepthEnable = true;

	//�u�����h���[�h�ݒ�
	objSubDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Sub);

	//�p�C�v���C������
	NGPipeline::Create(objSubDesc, "ObjSub");
#pragma endregion
#pragma region �F���]3D
	//�V�F�[�_�[����
	NShader::CreateShader("Obj", "Obj", false);

	PipelineDesc objInvDesc;
	//���_���C�A�E�g�ݒ�
	objInvDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	objInvDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigObjInv;
	rootSigObjInv.SetSamplerDesc(false);
	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A����
	rootSigObjInv.SetRootParam(1, 4);
	rootSigObjInv.Create();
	objInvDesc.rootSig = rootSigObjInv;

	//�V�F�[�_�[�ݒ�
	objInvDesc.shader.pShader = NShader::GetShader("Obj");

	//�J�����O�ݒ�
	objInvDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
	//�����_�[�^�[�Q�b�g���ݒ�
	objInvDesc.render.NumRenderTargets = 2;

	//�[�x�e�X�g����
	objInvDesc.depth.DepthStencilState.DepthEnable = true;

	//�u�����h���[�h�ݒ�
	objInvDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Inv);

	//�p�C�v���C������
	NGPipeline::Create(objInvDesc, "ObjInv");
#pragma endregion
#pragma region �f�t�H���gFBX
	//�V�F�[�_�[����
	NShader::CreateShader("Fbx", "Fbx", false);

	PipelineDesc fbxDesc;
	//���_���C�A�E�g�ݒ�
	fbxDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutFbx_;
	fbxDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutFbx_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigFbx;
	rootSigFbx.SetSamplerDesc(false);
	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A�����A�X�L��
	rootSigFbx.SetRootParam(1, 5);
	rootSigFbx.Create();
	fbxDesc.rootSig = rootSigFbx;

	//�V�F�[�_�[�ݒ�
	fbxDesc.shader.pShader = NShader::GetShader("Fbx");

	//�J�����O�ݒ�
	fbxDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
	//�����_�[�^�[�Q�b�g���ݒ�
	fbxDesc.render.NumRenderTargets = 2;

	//�[�x�e�X�g����
	fbxDesc.depth.DepthStencilState.DepthEnable = true;

	//�u�����h���[�h�ݒ�
	fbxDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//�p�C�v���C������
	NGPipeline::Create(fbxDesc, "FbxNone");
#pragma endregion
//#pragma region ���u�����hFBX
//	//�V�F�[�_�[����
//	NShader::CreateShader("Fbx", "Fbx", false);
//
//	PipelineDesc fbxAlphaDesc;
//	//���_���C�A�E�g�ݒ�
//	fbxAlphaDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutFbx_;
//	fbxAlphaDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutFbx_);
//
//	//���[�g�V�O�l�`���ݒ�
//	NRootSignature rootSigFbxAlpha;
//	rootSigFbxAlpha.SetSamplerDesc(false);
//	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A�����A�X�L��
//	rootSigFbx.SetRootParam(1, 5);
//	rootSigFbxAlpha.Create();
//	fbxAlphaDesc.rootSig = rootSigFbxAlpha;
//
//	//�V�F�[�_�[�ݒ�
//	fbxAlphaDesc.shader.pShader = NShader::GetShader("Fbx");
//
//	//�J�����O�ݒ�
//	fbxAlphaDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
//	//�����_�[�^�[�Q�b�g���ݒ�
//	fbxAlphaDesc.render.NumRenderTargets = 2;
//
//	//�[�x�e�X�g����
//	fbxAlphaDesc.depth.DepthStencilState.DepthEnable = true;
//
//	//�u�����h���[�h�ݒ�
//	fbxAlphaDesc.blend.blendDesc =
//		bDesc::GetBlendMode(BlendMode::Alpha);
//
//	//�p�C�v���C������
//	NGPipeline::Create(fbxAlphaDesc, "FbxAlpha");
//#pragma endregion
//#pragma region ���ZFBX
//	//�V�F�[�_�[����
//	NShader::CreateShader("Fbx", "Fbx", false);
//
//	PipelineDesc fbxAddDesc;
//	//���_���C�A�E�g�ݒ�
//	fbxAddDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutFbx_;
//	fbxAddDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutFbx_);
//
//	//���[�g�V�O�l�`���ݒ�
//	NRootSignature rootSigFbxAdd;
//	rootSigFbxAdd.SetSamplerDesc(false);
//	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A�����A�X�L��
//	rootSigFbx.SetRootParam(1, 5);
//	rootSigFbxAdd.Create();
//	fbxAddDesc.rootSig = rootSigFbxAdd;
//
//	//�V�F�[�_�[�ݒ�
//	fbxAddDesc.shader.pShader = NShader::GetShader("Fbx");
//
//	//�J�����O�ݒ�
//	fbxAddDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
//	//�����_�[�^�[�Q�b�g���ݒ�
//	fbxAddDesc.render.NumRenderTargets = 2;
//
//	//�[�x�e�X�g����
//	fbxAddDesc.depth.DepthStencilState.DepthEnable = true;
//
//	//�u�����h���[�h�ݒ�
//	fbxAddDesc.blend.blendDesc =
//		bDesc::GetBlendMode(BlendMode::Add);
//
//	//�p�C�v���C������
//	NGPipeline::Create(fbxAddDesc, "FbxAdd");
//#pragma endregion
//#pragma region ���ZFBX
//	//�V�F�[�_�[����
//	NShader::CreateShader("Fbx", "Fbx", false);
//
//	PipelineDesc fbxSubDesc;
//	//���_���C�A�E�g�ݒ�
//	fbxSubDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutFbx_;
//	fbxSubDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutFbx_);
//
//	//���[�g�V�O�l�`���ݒ�
//	NRootSignature rootSigFbxSub;
//	rootSigFbxSub.SetSamplerDesc(false);
//	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A�����A�X�L��
//	rootSigFbx.SetRootParam(1, 5);
//	rootSigFbxSub.Create();
//	fbxSubDesc.rootSig = rootSigFbxSub;
//
//	//�V�F�[�_�[�ݒ�
//	fbxSubDesc.shader.pShader = NShader::GetShader("Fbx");
//
//	//�J�����O�ݒ�
//	fbxSubDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
//	//�����_�[�^�[�Q�b�g���ݒ�
//	fbxSubDesc.render.NumRenderTargets = 2;
//
//	//�[�x�e�X�g����
//	fbxSubDesc.depth.DepthStencilState.DepthEnable = true;
//
//	//�u�����h���[�h�ݒ�
//	fbxSubDesc.blend.blendDesc =
//		bDesc::GetBlendMode(BlendMode::Sub);
//
//	//�p�C�v���C������
//	NGPipeline::Create(fbxSubDesc, "FbxSub");
//#pragma endregion
//#pragma region �F���]FBX
//	//�V�F�[�_�[����
//	NShader::CreateShader("Fbx", "Fbx", false);
//
//	PipelineDesc fbxInvDesc;
//	//���_���C�A�E�g�ݒ�
//	fbxInvDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutFbx_;
//	fbxInvDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutFbx_);
//
//	//���[�g�V�O�l�`���ݒ�
//	NRootSignature rootSigFbxInv;
//	rootSigFbxInv.SetSamplerDesc(false);
//	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A�����A�X�L��
//	rootSigFbx.SetRootParam(1, 5);
//	rootSigFbxInv.Create();
//	fbxInvDesc.rootSig = rootSigFbxInv;
//
//	//�V�F�[�_�[�ݒ�
//	fbxInvDesc.shader.pShader = NShader::GetShader("Fbx");
//
//	//�J�����O�ݒ�
//	fbxInvDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
//	//�����_�[�^�[�Q�b�g���ݒ�
//	fbxInvDesc.render.NumRenderTargets = 2;
//
//	//�[�x�e�X�g����
//	fbxInvDesc.depth.DepthStencilState.DepthEnable = true;
//
//	//�u�����h���[�h�ݒ�
//	fbxInvDesc.blend.blendDesc =
//		bDesc::GetBlendMode(BlendMode::Inv);
//
//	//�p�C�v���C������
//	NGPipeline::Create(fbxInvDesc, "FbxInv");
//#pragma endregion
#pragma region �^�C�����O3D(�w�i�I�u�W�F�N�g�Ƃ��Ɏg��)
	//�V�F�[�_�[����
	NShader::CreateShader("Tile", "Tile", false);

	PipelineDesc tileDesc;
	//���_���C�A�E�g�ݒ�
	tileDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	tileDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigTile;
	rootSigTile.SetSamplerDesc(true);
	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A�����A�^�C���p�̏��
	rootSigTile.SetRootParam(1, 5);
	rootSigTile.Create();
	tileDesc.rootSig = rootSigTile;

	//�V�F�[�_�[�ݒ�
	tileDesc.shader.pShader = NShader::GetShader("Tile");

	//�J�����O�ݒ�
	tileDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//�w�ʃJ�����O����
	//�����_�[�^�[�Q�b�g���ݒ�
	tileDesc.render.NumRenderTargets = 2;

	//�[�x�e�X�g����
	tileDesc.depth.DepthStencilState.DepthEnable = true;

	//�u�����h���[�h�ݒ�
	tileDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//�p�C�v���C������
	NGPipeline::Create(tileDesc, "TileObjNone");
#pragma endregion
#pragma region �f�t�H���g2D
	//�V�F�[�_�[����
	NShader::CreateShader("Sprite", "Sprite", false);

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
	spriteDesc.shader.pShader = NShader::GetShader("Sprite");

	//�[�x���ݒ�
	spriteDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	spriteDesc.depth.DepthStencilState.DepthEnable = false;

	//�����_�[�^�[�Q�b�g���ݒ�
	spriteDesc.render.NumRenderTargets = 2;

	//�J�����O�ݒ�
	spriteDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h���[�h�ݒ�
	spriteDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//�p�C�v���C������
	NGPipeline::Create(spriteDesc, "SpriteNone");
#pragma endregion
#pragma region ���u�����h2D
	//�V�F�[�_�[����
	NShader::CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteAlphaDesc;
	//���_���C�A�E�g�ݒ�
	spriteAlphaDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutSprite_;
	spriteAlphaDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutSprite_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigSpriteAlpha;
	rootSigSpriteAlpha.SetSamplerDesc(true);
	//�e�N�X�`��1�A�s��A�F
	rootSigSpriteAlpha.SetRootParam(1, 2);
	rootSigSpriteAlpha.Create();
	spriteAlphaDesc.rootSig = rootSigSpriteAlpha;

	//�V�F�[�_�[�ݒ�
	spriteAlphaDesc.shader.pShader = NShader::GetShader("Sprite");

	//�[�x���ݒ�
	spriteAlphaDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	spriteAlphaDesc.depth.DepthStencilState.DepthEnable = false;

	//�����_�[�^�[�Q�b�g���ݒ�
	spriteAlphaDesc.render.NumRenderTargets = 2;

	//�J�����O�ݒ�
	spriteAlphaDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h���[�h�ݒ�
	spriteAlphaDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Alpha);

	//�p�C�v���C������
	NGPipeline::Create(spriteAlphaDesc, "SpriteAlpha");
#pragma endregion
#pragma region ���Z2D
	//�V�F�[�_�[����
	NShader::CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteAddDesc;
	//���_���C�A�E�g�ݒ�
	spriteAddDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutSprite_;
	spriteAddDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutSprite_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigSpriteAdd;
	rootSigSpriteAdd.SetSamplerDesc(true);
	//�e�N�X�`��1�A�s��A�F
	rootSigSpriteAdd.SetRootParam(1, 2);
	rootSigSpriteAdd.Create();
	spriteAddDesc.rootSig = rootSigSpriteAdd;

	//�V�F�[�_�[�ݒ�
	spriteAddDesc.shader.pShader = NShader::GetShader("Sprite");

	//�[�x���ݒ�
	spriteAddDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	spriteAddDesc.depth.DepthStencilState.DepthEnable = false;

	//�����_�[�^�[�Q�b�g���ݒ�
	spriteAddDesc.render.NumRenderTargets = 2;

	//�J�����O�ݒ�
	spriteAddDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h���[�h�ݒ�
	spriteAddDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Add);

	//�p�C�v���C������
	NGPipeline::Create(spriteAddDesc, "SpriteAdd");
#pragma endregion
#pragma region ���Z2D
	//�V�F�[�_�[����
	NShader::CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteSubDesc;
	//���_���C�A�E�g�ݒ�
	spriteSubDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutSprite_;
	spriteSubDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutSprite_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigSpriteSub;
	rootSigSpriteSub.SetSamplerDesc(true);
	//�e�N�X�`��1�A�s��A�F
	rootSigSpriteSub.SetRootParam(1, 2);
	rootSigSpriteSub.Create();
	spriteSubDesc.rootSig = rootSigSpriteSub;

	//�V�F�[�_�[�ݒ�
	spriteSubDesc.shader.pShader = NShader::GetShader("Sprite");

	//�[�x���ݒ�
	spriteSubDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	spriteSubDesc.depth.DepthStencilState.DepthEnable = false;

	//�����_�[�^�[�Q�b�g���ݒ�
	spriteSubDesc.render.NumRenderTargets = 2;

	//�J�����O�ݒ�
	spriteSubDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h���[�h�ݒ�
	spriteSubDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Sub);

	//�p�C�v���C������
	NGPipeline::Create(spriteSubDesc, "SpriteSub");
#pragma endregion
#pragma region �F���]2D
	//�V�F�[�_�[����
	NShader::CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteInvDesc;
	//���_���C�A�E�g�ݒ�
	spriteInvDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutSprite_;
	spriteInvDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutSprite_);

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigSpriteInv;
	rootSigSpriteInv.SetSamplerDesc(true);
	//�e�N�X�`��1�A�s��A�F
	rootSigSpriteInv.SetRootParam(1, 2);
	rootSigSpriteInv.Create();
	spriteInvDesc.rootSig = rootSigSpriteInv;

	//�V�F�[�_�[�ݒ�
	spriteInvDesc.shader.pShader = NShader::GetShader("Sprite");

	//�[�x���ݒ�
	spriteInvDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	spriteInvDesc.depth.DepthStencilState.DepthEnable = false;

	//�����_�[�^�[�Q�b�g���ݒ�
	spriteInvDesc.render.NumRenderTargets = 2;

	//�J�����O�ݒ�
	spriteInvDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h���[�h�ݒ�
	spriteInvDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Inv);

	//�p�C�v���C������
	NGPipeline::Create(spriteInvDesc, "SpriteInv");
#pragma endregion
#pragma region �|�X�g�G�t�F�N�g
	//�V�F�[�_�[����
	NShader::CreateShader("Bloom", "Bloom", false);

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
	postEffectDesc.shader.pShader = NShader::GetShader("Bloom");

	//�[�x���ݒ�
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//�����_�[�^�[�Q�b�g���ݒ�
	postEffectDesc.render.NumRenderTargets = 2;

	//�J�����O�ݒ�
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h���[�h�ݒ�
	postEffectDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//�p�C�v���C������
	NGPipeline::Create(postEffectDesc, "Bloom");
#pragma endregion
#pragma region �K�E�V�A���u���[
	//�V�F�[�_�[����
	NShader::CreateShader("Gaussian", "GaussianBlur", false);

	//���_���C�A�E�g�ݒ�
	postEffectDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutPostEffect_;
	postEffectDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutPostEffect_);

	//���[�g�V�O�l�`���ݒ�
	postEffectDesc.rootSig = rootSigPostEffect;

	//�V�F�[�_�[�ݒ�
	postEffectDesc.shader.pShader = NShader::GetShader("Gaussian");

	//�[�x���ݒ�
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//�J�����O�ݒ�
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h���[�h�ݒ�
	postEffectDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//�p�C�v���C������
	NGPipeline::Create(postEffectDesc, "Gaussian");
#pragma endregion
#pragma region ���W�A���u���[
	//�V�F�[�_�[����
	NShader::CreateShader("Radial", "RadialBlur", false);

	//���_���C�A�E�g�ݒ�
	postEffectDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutPostEffect_;
	postEffectDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutPostEffect_);

	//���[�g�V�O�l�`���ݒ�
	postEffectDesc.rootSig = rootSigPostEffect;

	//�V�F�[�_�[�ݒ�
	postEffectDesc.shader.pShader = NShader::GetShader("Radial");

	//�[�x���ݒ�
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�[�x�e�X�g���Ȃ�
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//�J�����O�ݒ�
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h���[�h�ݒ�
	postEffectDesc.blend.blendDesc = 
		bDesc::GetBlendMode(BlendMode::None);

	//�p�C�v���C������
	NGPipeline::Create(postEffectDesc, "Radial");
#pragma endregion
#pragma region �f�t�H���g�p�[�e�B�N��3D
	//�V�F�[�_�[����
	NShader::CreateShader("Particle3d", "Particle3D", true);

	PipelineDesc particleDesc;
	//���_���C�A�E�g�ݒ�
	particleDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutParticle_;
	particleDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutParticle_);

	particleDesc.render.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigParticle;
	rootSigParticle.SetSamplerDesc(false);
	//�e�N�X�`��2�A�s��A����
	rootSigParticle.SetRootParam(2, 2);
	rootSigParticle.Create();
	particleDesc.rootSig = rootSigParticle;

	//�V�F�[�_�[�ݒ�
	particleDesc.shader.pShader = NShader::GetShader("Particle3d");

	//�[�x�e�X�g����
	particleDesc.depth.DepthStencilState.DepthEnable = true;

	//�J�����O�ݒ�
	particleDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h���[�h�ݒ�
	particleDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//�p�C�v���C������
	NGPipeline::Create(particleDesc, "Particle3dNone");
#pragma endregion
#pragma region ���u�����h�p�[�e�B�N��3D
	//�V�F�[�_�[����
	NShader::CreateShader("Particle3d", "Particle3D", true);

	PipelineDesc particleAlphaDesc;
	//���_���C�A�E�g�ݒ�
	particleAlphaDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutParticle_;
	particleAlphaDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutParticle_);

	particleAlphaDesc.render.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigParticleAlpha;
	rootSigParticleAlpha.SetSamplerDesc(false);
	//�e�N�X�`��2�A�s��A����
	rootSigParticleAlpha.SetRootParam(2, 2);
	rootSigParticleAlpha.Create();
	particleAlphaDesc.rootSig = rootSigParticleAlpha;

	//�V�F�[�_�[�ݒ�
	particleAlphaDesc.shader.pShader = NShader::GetShader("Particle3d");

	//�[�x�e�X�g����
	particleAlphaDesc.depth.DepthStencilState.DepthEnable = true;

	//�J�����O�ݒ�
	particleAlphaDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h���[�h�ݒ�
	particleAlphaDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Alpha);

	//�p�C�v���C������
	NGPipeline::Create(particleAlphaDesc, "Particle3dAlpha");
#pragma endregion
#pragma region ���Z�p�[�e�B�N��3D
	//�V�F�[�_�[����
	NShader::CreateShader("Particle3d", "Particle3D", true);

	PipelineDesc particleAddDesc;
	//���_���C�A�E�g�ݒ�
	particleAddDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutParticle_;
	particleAddDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutParticle_);

	particleAddDesc.render.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//���[�g�V�O�l�`���ݒ�
	NRootSignature rootSigParticleAdd;
	rootSigParticleAdd.SetSamplerDesc(false);
	//�e�N�X�`��2�A�s��A����
	rootSigParticleAdd.SetRootParam(2, 2);
	rootSigParticleAdd.Create();
	particleAddDesc.rootSig = rootSigParticleAdd;

	//�V�F�[�_�[�ݒ�
	particleAddDesc.shader.pShader = NShader::GetShader("Particle3d");

	//�[�x�e�X�g����
	particleAddDesc.depth.DepthStencilState.DepthEnable = true;

	//�J�����O�ݒ�
	particleAddDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h���[�h�ݒ�
	particleAddDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Add);

	//�p�C�v���C������
	NGPipeline::Create(particleAddDesc, "Particle3dAdd");
#pragma endregion
}

bDesc bDesc::GetBlendMode(BlendMode blendMode)
{
	bDesc desc;
	switch (blendMode)
	{
	case BlendMode::None:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//��2�̒l�𑫂�
		desc.SrcBlend = D3D12_BLEND_ONE;					//�\�[�X�̒l�S���g��
		desc.DestBlend = D3D12_BLEND_ZERO;					//�f�X�g�̒l�g��Ȃ�
		break;

	case BlendMode::Alpha:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//��2�̒l�𑫂�
		desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				//�\�[�X�̃A���t�@�l
		desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;			//1.0f-�\�[�X�̃A���t�@�l
		break;

	case BlendMode::Add:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//��2�̒l�𑫂�
		desc.SrcBlend = D3D12_BLEND_ONE;					//�\�[�X�̒l��100%�g��
		desc.DestBlend = D3D12_BLEND_ONE;					//�f�X�g�̒l��100%�g��
		break;

	case BlendMode::Sub:
		desc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;			//��2�̒l������
		desc.SrcBlend = D3D12_BLEND_ONE;					//�\�[�X�̒l��100%�g��
		desc.DestBlend = D3D12_BLEND_ONE;					//�f�X�g�̒l��100%�g��

		desc.BlendOpAlpha = D3D12_BLEND_OP_REV_SUBTRACT;	//��2�̒l������
		desc.SrcBlendAlpha = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
		desc.DestBlendAlpha = D3D12_BLEND_ONE;				//�f�X�g�̒l��100%�g��
		break;

	case BlendMode::Inv:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//��2�̒l�𑫂�
		desc.SrcBlend = D3D12_BLEND_INV_SRC_COLOR;			//�F���](1-RGB)
		desc.DestBlend = D3D12_BLEND_ZERO;					//�f�X�g�̒l��0%�g��
		break;

	default:
		break;
	}

	return desc;
}