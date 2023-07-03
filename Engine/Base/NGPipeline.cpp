#include "NGPipeline.h"
#include "NShader.h"

void NGPipeline::SetVertLayout3d()
{
	// ���_���C�A�E�g
	//���W
	vertLayout3d_[0] = {
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
	vertLayout3d_[1] = {
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	//UV
	vertLayout3d_[2] = {
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

void NGPipeline::SetShader()
{
	// �V�F�[�_�[�̐ݒ�
	pipelineDesc_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = vsBlob_->GetBufferSize();
	pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();
}

void NGPipeline::SetRasterizer(bool isCull)
{
	// �T���v���}�X�N�̐ݒ�
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	if (isCull)
	{
		pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// �w�ʂ��J�����O
	}
	else
	{
		pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// �J�����O���Ȃ�
	}
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// �|���S�����h��Ԃ�(D3D12_FILL_MODE_WIREFRAME�ɂ���ƃ��C���[�t���[����)
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����
}

void NGPipeline::SetBlend(const bool is3d)
{
	pipelineDesc_.BlendState.AlphaToCoverageEnable = false;			//�ԗ����l�����ău�����h���邩
	pipelineDesc_.BlendState.IndependentBlendEnable = false;			//���ꂼ��̃����_�[�^�[�Q�b�g�ɕʁX�̃u�����h���邩

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	blendDesc_.BlendEnable = true;					//�u�����h��L���ɂ���
	blendDesc_.LogicOpEnable = false;				//�_�����Z���邩
	blendDesc_.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//�}�X�N�l�FRBGA�S�Ẵ`�����l����`��

	blendDesc_.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blendDesc_.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
	blendDesc_.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��

	//�A���t�@�u�����h
	blendDesc_.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blendDesc_.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blendDesc_.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-�\�[�X�̃A���t�@�l

	//�ݒ肵���u�����h��K�p
	pipelineDesc_.BlendState.RenderTarget[0] = blendDesc_;
	if (is3d)
	{
		pipelineDesc_.BlendState.RenderTarget[1] = blendDesc_;
	}
}

void NGPipeline::SetInputLayout(const bool is3d)
{
	if (is3d)
	{
		pipelineDesc_.InputLayout.pInputElementDescs = vertLayout3d_;
		pipelineDesc_.InputLayout.NumElements = _countof(vertLayout3d_);
	}
	else
	{
		pipelineDesc_.InputLayout.pInputElementDescs = vertLayoutSprite_;
		pipelineDesc_.InputLayout.NumElements = _countof(vertLayoutSprite_);
	}
}

void NGPipeline::SetInputLayoutPostEffect()
{
	pipelineDesc_.InputLayout.pInputElementDescs = vertLayoutPostEffect_;
	pipelineDesc_.InputLayout.NumElements = _countof(vertLayoutPostEffect_);
}

void NGPipeline::SetTopology()
{
	//�g���C�A���O���X�g���b�v��؂藣�����ǂ���
	pipelineDesc_.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;	//�J�b�g�Ȃ�
	//�g�|���W�[�w��
	pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
}

void NGPipeline::SetDepth(const bool isDepth)
{
	if (isDepth)
	{
		pipelineDesc_.DepthStencilState.DepthEnable = true;						//�[�x�e�X�g���邩
	}
	else
	{
		pipelineDesc_.DepthStencilState.DepthEnable = false;						//�[�x�e�X�g���邩
	}
	pipelineDesc_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
	pipelineDesc_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;			//��������΍��i
	pipelineDesc_.DSVFormat = DXGI_FORMAT_D32_FLOAT;								//�[�x�l�t�H�[�}�b�g
}

void NGPipeline::SetRenderTarget(const uint32_t RTNum)
{
	pipelineDesc_.NumRenderTargets = RTNum;								//�`��Ώۂ�2��

	for (uint32_t i = 0; i < RTNum; i++)
	{
		pipelineDesc_.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255�w���RGBA
	}
}

void NGPipeline::SetAntiAliasing()
{
	pipelineDesc_.SampleDesc.Count = 1;		//1�s�N�Z���ɂ�1��T���v�����O
	pipelineDesc_.SampleDesc.Quality = 0;	//�Œ�N�I���e�B
}

void NGPipeline::SetRootSignature(const uint32_t texNum, const uint32_t constantNum)
{
	rootParams_.SetDescRange(texNum);
	rootParams_.SetRootParam(texNum, constantNum);

	pipelineSet_.rootSig_.SetRootSignature(errorBlob_, rootParams_.entity_, samplerDesc_);
	pipelineSet_.rootSig_.CreateRootSignature();

	pipelineDesc_.pRootSignature = pipelineSet_.rootSig_.entity_.Get();
}

void NGPipeline::CreatePS()
{
	HRESULT result;

	result = NDX12::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineSet_.pipelineState_));
	assert(SUCCEEDED(result));
}

void NGPipeline::SetTexSampler(const bool isTiling)
{
	if (isTiling)
	{
		samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�����i�^�C�����O�j
		samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�����i�^�C�����O�j
	}
	else
	{
		samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;					//���J��Ԃ����Ȃ��i�^�C�����O�Ȃ��j
		samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;					//�c�J��Ԃ����Ȃ��i�^�C�����O�Ȃ��j
	}
	samplerDesc_.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ��i�^�C�����O�j
	samplerDesc_.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc_.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//�S�ă��j�A���
	samplerDesc_.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc_.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc_.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc_.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_����̂ݎg�p�\
}

PipelineSet NGPipeline::CreatePipeline3d()
{
	//�V�F�[�_�[
	LoadVertShader3d();
	LoadPixelShader3d();

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
	SetTexSampler(false);

	//���[�g�V�O�l�`��
	//�e�N�X�`��1�A�s��A�}�e���A���A�F�A����
	SetRootSignature(1, 4);

	//�p�C�v���C���X�e�[�g����
	CreatePS();

	return pipelineSet_;
}

PipelineSet NGPipeline::CreatePipelineSprite()
{
	//�V�F�[�_�[
	LoadVertShaderSprite();
	LoadPixelShaderSprite();

	//���_���C�A�E�g�ݒ�
	SetVertLayoutSprite();

	//�p�C�v���C���X�e�[�g
	SetShader();
	SetRasterizer(false);
	SetBlend(false);
	SetInputLayout(false);
	SetTopology();
	SetRenderTarget(2);
	SetAntiAliasing();
	SetDepth(false);

	//�e�N�X�`���T���v���[�̐ݒ�
	SetTexSampler(false);

	//���[�g�V�O�l�`��
	//�e�N�X�`��1�A�s��A�}�e���A���A�F
	SetRootSignature(1, 3);

	//�p�C�v���C���X�e�[�g����
	CreatePS();

	return pipelineSet_;
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
	NShader::GetInstance()->CreateShader("3d","Obj",false);
#pragma endregion
#pragma region �f�t�H���g2D
	NShader::GetInstance()->CreateShader("Sprite", "Sprite", false);
#pragma endregion
#pragma region �|�X�g�G�t�F�N�g
	NShader::GetInstance()->CreateShader("PostEffect", "CG4", false);
#pragma endregion
#pragma region �K�E�V�A���u���[
	NShader::GetInstance()->CreateShader("Gaussian", "GaussianBlur", false);
#pragma endregion
#pragma region ���W�A���u���[
	NShader::GetInstance()->CreateShader("Radial", "RadialBlur", false);
#pragma endregion
#pragma region �p�[�e�B�N��3D
	NShader::GetInstance()->CreateShader("Particle3d", "Particle3D", true);
#pragma endregion
}

void PipeLineManager::Init()
{
	pipelineSet3d_ = pipeline3d_.CreatePipeline3d();
	pipelineSetSprite_ = pipelineSprite_.CreatePipelineSprite();
	pipelineSetPostEffect_ = pipelinePostEffect_.CreatePipelinePostEffect();
	pipelineSetGaussian_ = pipelineGaussian_.CreatePipelineGaussian();
	pipelineSetRadial_ = pipelineRadial_.CreatePipelineRadial();
	pipelineSetTile_ = pipelineTile_.CreatePipelineTile();
}

const PipelineSet& PipeLineManager::GetPipelineSet(std::string name) const
{
	if (name == "3d")
	{
		return pipelineSet3d_;
	}
	else if (name == "Sprite")
	{
		return pipelineSetSprite_;
	}
	else if (name == "PostEffect")
	{
		return pipelineSetPostEffect_;
	}
	else if (name == "Gaussian")
	{
		return pipelineSetGaussian_;
	}
	else if (name == "Radial")
	{
		return pipelineSetRadial_;
	}
	else if (name == "Tile")
	{
		return pipelineSetTile_;
	}

	//�����Y�����閼�O���Ȃ���Έُ�I��
	std::exit(EXIT_FAILURE);
	//�ꉞ�Ԃ�l�ݒ�
	return pipelineSet3d_;
}
