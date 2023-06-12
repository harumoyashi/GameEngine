#include "NIndexBuff.h"
#include "NDX12.h"

NIndexBuff::NIndexBuff(uint32_t* indices, uint32_t size)
{
	Init(indices, size);
}

NIndexBuff::NIndexBuff(std::vector<uint32_t> indices)
{
	Init(indices);
}

void NIndexBuff::Init(uint32_t* indices, uint32_t size)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	uint32_t dataSize = static_cast<UINT>(sizeof(UINT) * size);

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
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint32_t* indexMap = nullptr;
	result = buff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S�C���f�b�N�X�ɑ΂���
	//for (uint32_t i = 0; i < size; i++)
	//{
	//	indexMap[i] = indices_[i];	//�C���f�b�N�X���R�s�[
	//}
	memcpy(indexMap, indices, size);
	// �q���������
	buff_->Unmap(0, nullptr);

	// GPU���z�A�h���X
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	//�C���f�b�N�X1���̃T�C�Y
	view_.Format = DXGI_FORMAT_R32_UINT;
	// �C���f�b�N�X�o�b�t�@�̃T�C�Y
	view_.SizeInBytes = dataSize;
}

void NIndexBuff::Init(std::vector<uint32_t> indices)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	uint32_t dataSize = (uint32_t)(sizeof(uint32_t) * indices.size());

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (uint64_t)dataSize;
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
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint32_t* indexMap = nullptr;
	result = buff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S�C���f�b�N�X�ɑ΂���
	for (uint32_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];	//�C���f�b�N�X���R�s�[
	}
	// �q���������
	buff_->Unmap(0, nullptr);

	// GPU���z�A�h���X
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	//�C���f�b�N�X1���̃T�C�Y
	view_.Format = DXGI_FORMAT_R32_UINT;
	// �C���f�b�N�X�o�b�t�@�̃T�C�Y
	view_.SizeInBytes = dataSize;
}
