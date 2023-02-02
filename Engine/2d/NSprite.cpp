#include "NSprite.h"
#include "NMathUtil.h"

NSprite::~NSprite()
{
}

void NSprite::CreateSprite(std::string texHandle)
{
	SetTexHandle(texHandle);
	//���_
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff();
	VertMaping();
	CreateVertBuffView();
	//�萔�o�b�t�@
	SetConstHeap();
	SetConstResource();
	CreateCB();
	MappingCB();
	SetColor();
	Unmap();
	//���s���e����
	matProjection = constMapTransform->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::win_width),
		static_cast<float>(NWindows::win_height)
	);
	UpdateMatrix();
}

void NSprite::CreateSprite(std::string texHandle,
	NVector2 anchorPoint, bool isFlipX, bool isFlipY)
{
	SetTexHandle(texHandle);
	//���_
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff();
	MatchTexSize(NTextureManager::GetInstance()->textureMap[this->texHandle].texBuff);	//�����Ńe�N�X�`���T�C�Y�ɍ��킹�Ă�
	SetAncor(anchorPoint);
	SetIsFlip(isFlipX, isFlipY);
	SetClipRange();
	TransferVertex();
	VertMaping();
	CreateVertBuffView();
	//�萔�o�b�t�@
	SetConstHeap();
	SetConstResource();
	CreateCB();
	MappingCB();
	SetColor();
	Unmap();
	//���s���e����
	matProjection = constMapTransform->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::win_width),
		static_cast<float>(NWindows::win_height)
	);
	UpdateMatrix();
}

void NSprite::CreateClipSprite(std::string texHandle,
	NVector2 texLeftTop, NVector2 texSize, NVector2 anchorPoint, bool isFlipX, bool isFlipY)
{
	SetTexHandle(texHandle);
	//���_
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff();
	MatchTexSize(NTextureManager::GetInstance()->textureMap[texHandle].texBuff);	//�����Ńe�N�X�`���T�C�Y�ɍ��킹�Ă�
	SetAncor(anchorPoint);
	SetIsFlip(isFlipX, isFlipY);
	SetClipRange(texLeftTop, texSize);
	TransferVertex();
	VertMaping();
	CreateVertBuffView();
	//�萔�o�b�t�@
	SetConstHeap();
	SetConstResource();
	CreateCB();
	MappingCB();
	SetColor();
	Unmap();
	//���s���e����
	matProjection = constMapTransform->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::win_width),
		static_cast<float>(NWindows::win_height)
	);
	UpdateMatrix();
}

void NSprite::SetVert()
{
	VertexUV vert[] = {
		//	x		y		z	 	u	v
		{{   0.0f, 100.0f, 0.0f }, {0.0f,1.0f}},	// ����
		{{   0.0f,   0.0f, 0.0f }, {0.0f,0.0f}},	// ����
		{{ 100.0f, 100.0f, 0.0f }, {1.0f,1.0f}},	// �E��
		{{ 100.0f,   0.0f, 0.0f }, {1.0f,0.0f}},	// �E��
	};

	//�ݒ肵���瑼�ł��g����ϐ��ɑ��
	std::copy(std::begin(vert), std::end(vert), vertices);

	//���_�o�b�t�@�̃T�C�Y����
	singleSizeVB = static_cast<UINT>(sizeof(vertices[0]));
	sizeVB = static_cast<UINT>(sizeof(vertices));
}

void NSprite::SetVertHeap()
{
	//�q�[�v�ݒ�
	heapPropVert.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	heapPropVert.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapPropVert.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
}

void NSprite::SetVertResource()
{
	//���\�[�X�ݒ�
	resDescVert.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDescVert.Width = sizeVB;			//���_�f�[�^�S�̂̃T�C�Y
	resDescVert.Height = 1;
	resDescVert.DepthOrArraySize = 1;
	resDescVert.MipLevels = 1;
	resDescVert.Format = DXGI_FORMAT_UNKNOWN;
	resDescVert.SampleDesc.Count = 1;
	resDescVert.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDescVert.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NSprite::CreateVertBuff()
{
	HRESULT result;

	// ���_�o�b�t�@�̐���
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapPropVert, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDescVert, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));
}

void NSprite::MatchTexSize(ComPtr<ID3D12Resource> texBuff)
{
	resDescVert = texBuff->GetDesc();
	size = { (float)resDescVert.Width,(float)resDescVert.Height };
}

void NSprite::SetAncor(NVector2 anchorPoint)
{
	this->anchorPoint = anchorPoint;
}

void NSprite::SetIsFlip(bool isFlipX, bool isFlipY)
{
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;
}

void NSprite::SetClipRange(NVector2 texLeftTop, NVector2 texSize)
{
	this->texLeftTop = texLeftTop;
	this->texSize = texSize;
	size = texSize;
}

void NSprite::SetClipRange()
{
	texLeftTop = { 0,0 };
	texSize = size;
}

void NSprite::VertMaping()
{
	HRESULT result;

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	std::copy(std::begin(vertices), std::end(vertices), vertMap);	//���W���R�s�[
	// �q���������
	vertBuff->Unmap(0, nullptr);
}

void NSprite::CreateVertBuffView()
{
	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = singleSizeVB;
}

void NSprite::SetConstHeap()
{
	heapPropConst.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
}

void NSprite::SetConstResource()
{
	resDescConst.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDescConst.Width = (sizeof(constMapTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	resDescConst.Height = 1;
	resDescConst.DepthOrArraySize = 1;
	resDescConst.MipLevels = 1;
	resDescConst.SampleDesc.Count = 1;
	resDescConst.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NSprite::CreateCB()
{
	HRESULT result;

	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapPropConst,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDescConst,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));
}

void NSprite::MappingCB()
{
	HRESULT result;

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);	//�}�b�s���O

	assert(SUCCEEDED(result));
}

void NSprite::SetTexHandle(std::string texHandle)
{
	this->texHandle = texHandle;
	if (NTextureManager::GetInstance()->textureMap[texHandle].texBuff == nullptr)
	{
		this->texHandle = "error";
	}
}

void NSprite::SetColor(int R, int G, int B, int A)
{
	XMFLOAT4 color = {};

	color.x = static_cast<float>(R) / 255.0f;
	color.y = static_cast<float>(G) / 255.0f;
	color.z = static_cast<float>(B) / 255.0f;
	color.w = static_cast<float>(A) / 255.0f;
	constMapTransform->color = color;
}

void NSprite::Unmap()
{
	constBuffTransform->Unmap(0, nullptr);
}

void NSprite::UpdateMatrix()
{
	//���[���h�s��
	NMatrix4 matRot;	//��]�s��
	matRot = matRot.RotateZ(MathUtil::Degree2Radian(rotation));	//Z������ɉ�]

	NMatrix4 matTrans;	//���s�ړ��s��
	matTrans = matTrans.Translation({ position.x, position.y, 0 });

	matWorld = matWorld.Identity();	//�P�ʍs����
	matWorld *= matRot;		//���[���h���W�ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h���W�ɕ��s�ړ��𔽉f

	TransferMatrix();
}

void NSprite::TransferMatrix()
{
	constMapTransform->mat = matWorld * matProjection;
}

void NSprite::TransferVertex()
{
	//UV�����f�t�H���g�Őݒ�
	VertexUV vert[] = {
		//		u	v
		{{}, {0.0f,1.0f}},	// ����
		{{}, {0.0f,0.0f}},	// ����
		{{}, {1.0f,1.0f}},	// �E��
		{{}, {1.0f,0.0f}},	// �E��
	};

	//�A���J�[�|�C���g�ݒ�
	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	//���E���]
	if (isFlipX)
	{
		left = -left;
		right = -right;
	}
	//�㉺���]
	if (isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}

	//�ݒ肵���A���J�[�|�C���g�ɍ��킹�Ē��_�ݒ肵�Ȃ���
	vert[0].pos = { left ,bottom,0.0f };	// ����
	vert[1].pos = { left ,top   ,0.0f };	// ����
	vert[2].pos = { right,bottom,0.0f };	// �E��
	vert[3].pos = { right,top   ,0.0f };	// �E��

	//�e�N�X�`���T�C�Y�����Ƃɐ؂��镔����uv���v�Z
	float tex_left = texLeftTop.x / resDescVert.Width;
	float tex_right = (texLeftTop.x + texSize.x) / resDescVert.Width;
	float tex_top = texLeftTop.y / resDescVert.Height;
	float tex_bottom = (texLeftTop.y + texSize.y) / resDescVert.Height;

	//�v�Z����uv�ɍ��킹�Đݒ肵�Ȃ���
	vert[0].uv = { tex_left ,tex_bottom };	// ����
	vert[1].uv = { tex_left ,tex_top };	// ����
	vert[2].uv = { tex_right,tex_bottom };	// �E��
	vert[3].uv = { tex_right,tex_top };	// �E��

	//�ݒ肵���瑼�ł��g����ϐ��ɑ��
	std::copy(std::begin(vert), std::end(vert), vertices);

	//���_�o�b�t�@�̃T�C�Y����
	singleSizeVB = static_cast<UINT>(sizeof(vertices[0]));
	sizeVB = static_cast<UINT>(sizeof(vertices));

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertMaping();
}

void NSprite::SetSize(float x, float y)
{
	size = { x,y };
	TransferVertex();
}

void NSprite::SetPos(float x, float y)
{
	position = { x,y };
	UpdateMatrix();
}

void NSprite::CommonBeginDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSetSprite().pipelineState.Get());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSetSprite().rootSig.entity.Get());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`�X�g���b�v

	ID3D12DescriptorHeap* ppHeaps[] = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NSprite::Draw()
{
	CommonBeginDraw();

	//��\���t���O�����Ă���`�悹���ɏ���������
	if (isInvisible)
	{
		return;
	}

	//�n���h�����w��
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = NTextureManager::GetInstance()->textureMap[texHandle].gpuHandle;

	//�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}