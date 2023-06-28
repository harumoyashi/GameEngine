#include "NGPipeline.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

void NGPipeline::LoadVertShader3d()
{
	HRESULT result;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ObjVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob_, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob_����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::LoadPixelShader3d()
{
	HRESULT result;

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ObjPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob_����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::LoadVertShaderSprite()
{
	HRESULT result;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob_, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob_����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::LoadPixelShaderSprite()
{
	HRESULT result;

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob_����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::LoadVertShaderPostEffect()
{
	HRESULT result;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/GaussianBlurVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob_, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob_����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::LoadPixelShaderPostEffect()
{
	HRESULT result;

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/GaussianBlurPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob_����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

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

void NGPipeline::SetRenderTarget(const bool is3d)
{
	if (is3d)
	{
		pipelineDesc_.NumRenderTargets = 2;								//�`��Ώۂ�2��
		pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255�w���RGBA
		pipelineDesc_.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255�w���RGBA
	}
	else
	{
		pipelineDesc_.NumRenderTargets = 1;								//�`��Ώۂ�2��
		pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255�w���RGBA
	}
}

void NGPipeline::SetAntiAliasing()
{
	pipelineDesc_.SampleDesc.Count = 1;		//1�s�N�Z���ɂ�1��T���v�����O
	pipelineDesc_.SampleDesc.Quality = 0;	//�Œ�N�I���e�B
}

void NGPipeline::SetRootSignature()
{
	rootParams_.SetDescRange();
	rootParams_.SetRootParam();

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

void NGPipeline::SetTexSampler()
{
	samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;					//���J��Ԃ����Ȃ��i�^�C�����O�j
	samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;					//�c�J��Ԃ����Ȃ��i�^�C�����O�j
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
	SetRenderTarget(true);
	SetAntiAliasing();
	SetDepth(true);

	//�e�N�X�`���T���v���[�̐ݒ�
	SetTexSampler();

	//���[�g�V�O�l�`��
	SetRootSignature();

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
	SetRenderTarget(false);
	SetAntiAliasing();
	SetDepth(false);

	//�e�N�X�`���T���v���[�̐ݒ�
	SetTexSampler();

	//���[�g�V�O�l�`��
	SetRootSignature();

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
	SetRenderTarget(false);
	SetAntiAliasing();
	SetDepth(false);

	//�e�N�X�`���T���v���[�̐ݒ�
	SetTexSampler();

	//���[�g�V�O�l�`��
	SetRootSignature();

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

void PipeLineManager::Init()
{
	pipelineSet3d_ = pipeline3d_.CreatePipeline3d();
	pipelineSetSprite_ = pipelineSprite_.CreatePipelineSprite();
	pipelineSetPostEffect_ = pipelinePostEffect_.CreatePipelinePostEffect();
}
