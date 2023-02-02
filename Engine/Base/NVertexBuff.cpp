#include "NVertexBuff.h"

NVertexBuff::NVertexBuff(NVertex* vertices, unsigned int size)
{
	Init(vertices, size);
}

NVertexBuff::NVertexBuff(NVertexPNU* vertices, unsigned int size)
{
	Init(vertices, size);
}

NVertexBuff::NVertexBuff(std::vector<NVertex> vertices)
{
	Init(vertices);
}

NVertexBuff::NVertexBuff(std::vector<NVertexPNU> vertices)
{
	Init(vertices);
}

void NVertexBuff::Init(NVertex* vertices, unsigned int size)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	UINT dataSize = static_cast<UINT>(sizeof(NVertex) * size);

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
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	NVertex* vertMap = nullptr;
	result = buff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (UINT i = 0; i < size; i++) {
		vertMap[i] = vertices[i];
	}
	// �q���������
	buff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	view.BufferLocation = buff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	view.SizeInBytes = dataSize;
	// ���_1���̃f�[�^�T�C�Y
	view.StrideInBytes = size;
}

void NVertexBuff::Init(NVertexPNU* vertices, unsigned int size)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	UINT dataSize = static_cast<UINT>(sizeof(NVertexPNU) * size);

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
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	NVertexPNU* vertMap = nullptr;
	result = buff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (UINT i = 0; i < size; i++) {
		vertMap[i] = vertices[i];
	}
	// �q���������
	buff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	view.BufferLocation = buff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	view.SizeInBytes = dataSize;
	// ���_1���̃f�[�^�T�C�Y
	view.StrideInBytes = size;
}

void NVertexBuff::Init(std::vector<NVertex> vertices)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	UINT dataSize = static_cast<UINT>(sizeof(NVertex) * vertices.size());

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
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	NVertex* vertMap = nullptr;
	result = buff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (UINT i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i];
	}
	// �q���������
	buff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	view.BufferLocation = buff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	view.SizeInBytes = dataSize;
	// ���_1���̃f�[�^�T�C�Y
	view.StrideInBytes = sizeof(NVertex);
}

void NVertexBuff::Init(std::vector<NVertexPNU> vertices)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	UINT dataSize = static_cast<UINT>(sizeof(NVertexPNU) * vertices.size());

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
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	NVertexPNU* vertMap = nullptr;
	result = buff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (UINT i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i];
	}
	// �q���������
	buff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	view.BufferLocation = buff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	view.SizeInBytes = dataSize;
	// ���_1���̃f�[�^�T�C�Y
	view.StrideInBytes = sizeof(NVertexPNU);
}