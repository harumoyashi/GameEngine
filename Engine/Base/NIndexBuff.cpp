#include "NIndexBuff.h"
#include "NDX12.h"

NIndexBuff::NIndexBuff(unsigned int* indices, unsigned int size)
{
	Init(indices, size);
}

NIndexBuff::NIndexBuff(std::vector<unsigned int> indices)
{
	Init(indices);
}

void NIndexBuff::Init(unsigned int* indices, unsigned int size)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	UINT dataSize = static_cast<UINT>(sizeof(UINT) * size);

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �C���f�b�N�X�o�b�t�@�̐���
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	UINT* indexMap = nullptr;
	result = buff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S�C���f�b�N�X�ɑ΂���
	for (UINT i = 0; i < size; i++)
	{
		indexMap[i] = indices[i];	//�C���f�b�N�X���R�s�[
	}
	// �q���������
	buff->Unmap(0, nullptr);

	// GPU���z�A�h���X
	view.BufferLocation = buff->GetGPUVirtualAddress();
	//�C���f�b�N�X1���̃T�C�Y
	view.Format = DXGI_FORMAT_R32_UINT;
	// �C���f�b�N�X�o�b�t�@�̃T�C�Y
	view.SizeInBytes = dataSize;
}

void NIndexBuff::Init(std::vector<unsigned int> indices)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	UINT dataSize = static_cast<UINT>(sizeof(UINT) * indices.size());

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �C���f�b�N�X�o�b�t�@�̐���
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	UINT* indexMap = nullptr;
	result = buff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S�C���f�b�N�X�ɑ΂���
	for (UINT i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];	//�C���f�b�N�X���R�s�[
	}
	// �q���������
	buff->Unmap(0, nullptr);

	// GPU���z�A�h���X
	view.BufferLocation = buff->GetGPUVirtualAddress();
	//�C���f�b�N�X1���̃T�C�Y
	view.Format = DXGI_FORMAT_R32_UINT;
	// �C���f�b�N�X�o�b�t�@�̃T�C�Y
	view.SizeInBytes = dataSize;
}
