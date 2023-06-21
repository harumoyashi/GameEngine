#include "NSprite.h"
#include "NMathUtil.h"

NSprite::NSprite()
{
	//�萔�o�b�t�@
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform2D>>();
	cbTrans_->Init();
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbColor_->Init();
}

NSprite::~NSprite()
{
}

void NSprite::CreateSprite(const std::string& texHandle)
{
	SetTexHandle(texHandle);
	//���_
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff();
	CreateVertBuffView();
	
	//���s���e����
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::kWin_width),
		static_cast<float>(NWindows::kWin_height)
	);
	Update();
}

void NSprite::CreateSprite(const std::string& texHandle,
	const NVector2& anchorPoint_, const bool isFlipX_, const bool isFlipY_)
{
	SetTexHandle(texHandle);
	//���_
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff();
	MatchTexSize(NTextureManager::GetInstance()->textureMap_[texHandle_].texBuff_);	//�����Ńe�N�X�`���T�C�Y�ɍ��킹�Ă�
	SetAncor(anchorPoint_);
	SetIsFlip(isFlipX_, isFlipY_);
	SetClipRange();
	TransferVertex();
	CreateVertBuffView();
	
	//���s���e����
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::kWin_width),
		static_cast<float>(NWindows::kWin_height)
	);
	Update();
}

void NSprite::CreateClipSprite(const std::string& texHandle,
	const NVector2& texLeftTop_, const NVector2& texSize_, const NVector2& anchorPoint_,
	const bool isFlipX_, const bool isFlipY_)
{
	SetTexHandle(texHandle);
	//���_
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff();
	MatchTexSize(NTextureManager::GetInstance()->textureMap_[texHandle_].texBuff_);	//�����Ńe�N�X�`���T�C�Y�ɍ��킹�Ă�
	SetAncor(anchorPoint_);
	SetIsFlip(isFlipX_, isFlipY_);
	SetClipRange(texLeftTop_, texSize_);
	TransferVertex();
	CreateVertBuffView();
	
	//���s���e����
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::kWin_width),
		static_cast<float>(NWindows::kWin_height)
	);
	Update();
}

void NSprite::SetVert()
{
	std::vector<NVertexUV> vert = {
		//	x		y		z	 	u	v
		{{   0.0f, 100.0f, 0.0f }, {0.0f,1.0f}},	// ����
		{{   0.0f,   0.0f, 0.0f }, {0.0f,0.0f}},	// ����
		{{ 100.0f, 100.0f, 0.0f }, {1.0f,1.0f}},	// �E��
		{{ 100.0f,   0.0f, 0.0f }, {1.0f,0.0f}},	// �E��
	};

	//�ݒ肵���瑼�ł��g����ϐ��ɑ��
	vertices_ = vert;

	//���_�o�b�t�@�̃T�C�Y����
	singleSizeVB_ = static_cast<uint32_t>(sizeof(vertices_[0]));
	singleVB_ = static_cast<uint32_t>(sizeof(vertices_));
}

void NSprite::SetVertHeap()
{
	//�q�[�v�ݒ�
	heapPropVert_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	heapPropVert_.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapPropVert_.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
}

void NSprite::SetVertResource()
{
	//���\�[�X�ݒ�
	resDescVert_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDescVert_.Width = singleVB_;			//���_�f�[�^�S�̂̃T�C�Y
	resDescVert_.Height = 1;
	resDescVert_.DepthOrArraySize = 1;
	resDescVert_.MipLevels = 1;
	resDescVert_.Format = DXGI_FORMAT_UNKNOWN;
	resDescVert_.SampleDesc.Count = 1;
	resDescVert_.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDescVert_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NSprite::CreateVertBuff()
{
	vertexBuff_.Init(vertices_);
}

void NSprite::MatchTexSize(const ComPtr<ID3D12Resource>& texBuff_)
{
	resDescVert_ = texBuff_->GetDesc();
	size_ = { (float)resDescVert_.Width,(float)resDescVert_.Height };
}

void NSprite::SetAncor(const NVector2& anchorPoint)
{
	anchorPoint_ = anchorPoint;
}

void NSprite::SetIsFlip(bool isFlipX, bool isFlipY)
{
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;
}

void NSprite::SetClipRange(const NVector2& texLeftTop, const NVector2& texSize)
{
	texLeftTop_ = texLeftTop;
	texSize_ = texSize;
	size_ = texSize;
}

void NSprite::SetClipRange()
{
	texLeftTop_ = { 0,0 };
	texSize_ = size_;
}

void NSprite::CreateVertBuffView()
{
	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	vertexBuff_.view_.BufferLocation = vertexBuff_.buff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vertexBuff_.view_.SizeInBytes = singleVB_;
	// ���_1���̃f�[�^�T�C�Y
	vertexBuff_.view_.StrideInBytes = singleSizeVB_;
}

void NSprite::SetTexHandle(const std::string& texHandle)
{
	texHandle_ = texHandle;
	if (NTextureManager::GetInstance()->textureMap_[texHandle_].texBuff_ == nullptr)
	{
		texHandle_ = "error";
	}
}

void NSprite::Update()
{
	//���[���h�s��
	NMatrix4 matRot;	//��]�s��
	matRot = matRot.RotateZ(MathUtil::Degree2Radian(rotation_));	//Z������ɉ�]

	NMatrix4 matTrans;	//���s�ړ��s��
	matTrans = matTrans.Translation({ position_.x, position_.y, 0 });

	matWorld_ = matWorld_.Identity();	//�P�ʍs����
	matWorld_ *= matRot;		//���[���h���W�ɉ�]�𔽉f
	matWorld_ *= matTrans;	//���[���h���W�ɕ��s�ړ��𔽉f

	// �萔�o�b�t�@�փf�[�^�]��
	cbTrans_->constMap_->mat = matWorld_ * matProjection_;
	cbColor_->constMap_->color = color_;
}

void NSprite::TransferVertex()
{
	//UV�����f�t�H���g�Őݒ�
	std::vector<NVertexUV> vert = {
		//		u	v
		{{}, {0.0f,1.0f}},	// ����
		{{}, {0.0f,0.0f}},	// ����
		{{}, {1.0f,1.0f}},	// �E��
		{{}, {1.0f,0.0f}},	// �E��
	};

	//�A���J�[�|�C���g�ݒ�
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	//���E���]
	if (isFlipX_)
	{
		left = -left;
		right = -right;
	}
	//�㉺���]
	if (isFlipY_)
	{
		top = -top;
		bottom = -bottom;
	}

	//�ݒ肵���A���J�[�|�C���g�ɍ��킹�Ē��_�ݒ肵�Ȃ���
	vert[0].pos = { left ,bottom,0.0f };	// ����
	vert[1].pos = { left ,top   ,0.0f };	// ����
	vert[2].pos = { right,bottom,0.0f };	// �E��
	vert[3].pos = { right,top   ,0.0f };	// �E��

	resDescVert_ = NTextureManager::GetInstance()->textureMap_[texHandle_].texBuff_->GetDesc();

	//�e�N�X�`���T�C�Y�����Ƃɐ؂��镔����uv���v�Z
	float tex_left = texLeftTop_.x / resDescVert_.Width;
	float tex_right = (texLeftTop_.x + texSize_.x) / resDescVert_.Width;
	float tex_top = texLeftTop_.y / resDescVert_.Height;
	float tex_bottom = (texLeftTop_.y + texSize_.y) / resDescVert_.Height;

	//�v�Z����uv�ɍ��킹�Đݒ肵�Ȃ���
	vert[0].uv = { tex_left ,tex_bottom };	// ����
	vert[1].uv = { tex_left ,tex_top };	// ����
	vert[2].uv = { tex_right,tex_bottom };	// �E��
	vert[3].uv = { tex_right,tex_top };	// �E��

	//�ݒ肵���瑼�ł��g����ϐ��ɑ��
	vertices_ = vert;

	//���_�o�b�t�@�̃T�C�Y����
	singleSizeVB_ = static_cast<uint32_t>(sizeof(vertices_[0]));
	singleVB_ = static_cast<uint32_t>(sizeof(vertices_));

	//���_�o�b�t�@�ւ̃f�[�^�]��
	vertexBuff_.Init(vert);
}

void NSprite::SetSize(const float x, const float y)
{
	size_ = { x,y };
	TransferVertex();
}

void NSprite::SetPos(const float x, const float y)
{
	position_ = { x,y };
	Update();
}

void NSprite::CommonBeginDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSetSprite().pipelineState_.Get());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSetSprite().rootSig_.entity_.Get());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`�X�g���b�v

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void NSprite::Draw()
{
	CommonBeginDraw();

	//��\���t���O�����Ă���`�悹���ɏ���������
	if (isInvisible_)
	{
		return;
	}

	//�n���h�����w��
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = NTextureManager::GetInstance()->textureMap_[texHandle_].gpuHandle_;

	//�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBuff_.view_);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbTrans_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}