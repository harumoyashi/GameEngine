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
	SetTexSize(NTextureManager::GetInstance()->textureMap_[texHandle_].texBuff_);
	TransferVertex();

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
	SetTexSize(NTextureManager::GetInstance()->textureMap_[texHandle_].texBuff_);
	SetAncor(anchorPoint_);
	SetIsFlip(isFlipX_, isFlipY_);
	TransferVertex();

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
	SetTexSize(NTextureManager::GetInstance()->textureMap_[texHandle_].texBuff_);
	SetAncor(anchorPoint_);
	SetIsFlip(isFlipX_, isFlipY_);
	SetClipRange(texLeftTop_, texSize_);
	TransferVertex();

	//���s���e����
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::kWin_width),
		static_cast<float>(NWindows::kWin_height)
	);
	Update();
}

void NSprite::SetTexSize(const ComPtr<ID3D12Resource>& texBuff)
{
	resDescVert_ = texBuff->GetDesc();
	texSize_ = { (float)resDescVert_.Width,(float)resDescVert_.Height };
	//�e�N�X�`���̃T�C�Y�ƃX�v���C�g�̃T�C�Y�ꏏ��
	size_ = texSize_;
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
	//texSize_ = size_;
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
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("Sprite"));
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("Sprite")->pRootSignature);

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`�X�g���b�v

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void NSprite::Draw()
{
	//��\���t���O�����Ă���`�悹���ɏ���������
	if (isInvisible_)
	{
		return;
	}

	//�n���h�����w��
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = NTextureManager::GetInstance()->textureMap_[texHandle_].gpuHandle_;

	//�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^0�Ԃɐݒ�
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvGpuHandle);

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuff_.GetView());

	//���[�g�p�����[�^�ɒ萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, cbTrans_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}