#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include "NVertex.h"

template<typename T> class NVertexBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> buff_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW view_{};
	T* vertMap_;		//�}�b�s���O�p

public:
	NVertexBuff() {};	//����̃R���X�g���N�^

	//���_�̃|�C���^�ƃT�C�Y�ŏ�����
	void Init(T* vertices, const uint32_t size)
	{
		HRESULT result;

		// �q�[�v�v���p�e�B
		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

		uint32_t dataSize = static_cast<uint32_t>(sizeof(T) * size);

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
		vertMap_ = nullptr;
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
	//���_�̔z��ŏ�����
	void Init(const std::vector<T>& vertices)
	{
		HRESULT result;

		// �q�[�v�v���p�e�B
		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

		uint32_t dataSize = static_cast<unsigned int>(sizeof(T) * vertices.size());

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
		vertMap_ = nullptr;
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
		view_.StrideInBytes = sizeof(vertices[0]);
	}

	//���_���̓]��
	void TransferBuffer(const std::vector<T>& vertices)
	{
		// �S���_�ɑ΂��ăR�s�[
		copy(vertices.begin(), vertices.end(), vertMap_);
	}

	// �Q�b�^�[ //
	//�o�b�t�@�[�擾
	inline ID3D12Resource* GetBuffer() { return buff_.Get(); }
	//�r���[�擾
	inline D3D12_VERTEX_BUFFER_VIEW* GetView() { return &view_; }
};