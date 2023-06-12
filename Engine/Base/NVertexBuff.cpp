#include "NVertexBuff.h"

NVertexBuff::NVertexBuff(NVertex* vertices, const uint32_t size)
{
	Init(vertices, size);
}

NVertexBuff::NVertexBuff(NVertexPNU* vertices, const uint32_t size)
{
	Init(vertices, size);
}

NVertexBuff::NVertexBuff(const std::vector<NVertex>& vertices)
{
	Init(vertices);
}

NVertexBuff::NVertexBuff(const std::vector<NVertexPNU>& vertices)
{
	Init(vertices);
}

NVertexBuff::NVertexBuff(NVertexAssimp* vertices, const uint32_t size)
{
	Init(vertices, size);
}

void NVertexBuff::Init(NVertex* vertices, const uint32_t size)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertex) * size);

	//���_�o�b�t�@���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	NVertex* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (uint32_t i = 0; i < size; i++) {
		vertMap_[i] = vertices[i];
	}
	// �q���������
	buff_->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	view_.SizeInBytes = dataSize;
	// ���_1���̃f�[�^�T�C�Y
	view_.StrideInBytes = size;
}

void NVertexBuff::Init(NVertexPNU* vertices, const uint32_t size)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertexPNU) * size);

	//���_�o�b�t�@���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	NVertexPNU* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (uint32_t i = 0; i < size; i++) {
		vertMap_[i] = vertices[i];
	}
	// �q���������
	buff_->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	view_.SizeInBytes = dataSize;
	// ���_1���̃f�[�^�T�C�Y
	view_.StrideInBytes = size;
}

void NVertexBuff::Init(const std::vector<NVertex>& vertices)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertex) * vertices.size());

	//���_�o�b�t�@���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	NVertex* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (uint32_t i = 0; i < vertices.size(); i++) {
		vertMap_[i] = vertices[i];
	}
	// �q���������
	buff_->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	view_.SizeInBytes = dataSize;
	// ���_1���̃f�[�^�T�C�Y
	view_.StrideInBytes = sizeof(NVertex);
}

void NVertexBuff::Init(const std::vector<NVertexPNU>& vertices)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertexPNU) * vertices.size());

	//���_�o�b�t�@���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	NVertexPNU* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (uint32_t i = 0; i < vertices.size(); i++) {
		vertMap_[i] = vertices[i];
	}
	// �q���������
	buff_->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	view_.SizeInBytes = dataSize;
	// ���_1���̃f�[�^�T�C�Y
	view_.StrideInBytes = sizeof(NVertexPNU);
}

void NVertexBuff::Init(NVertexAssimp* vertices, const uint32_t size)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertexAssimp) * size);

	//���_�o�b�t�@���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	NVertexAssimp* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (uint32_t i = 0; i < size; i++) {
		vertMap_[i] = vertices[i];
	}
	// �q���������
	buff_->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	view_.SizeInBytes = dataSize;
	// ���_1���̃f�[�^�T�C�Y
	view_.StrideInBytes = size;
}

void NVertexBuff::Init(const std::vector<NVertexAssimp>& vertices)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertexAssimp) * vertices.size());

	//���_�o�b�t�@���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	NVertexAssimp* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (uint32_t i = 0; i < vertices.size(); i++) {
		vertMap_[i] = vertices[i];
	}
	// �q���������
	buff_->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	view_.SizeInBytes = dataSize;
	// ���_1���̃f�[�^�T�C�Y
	view_.StrideInBytes = sizeof(NVertexAssimp);
}