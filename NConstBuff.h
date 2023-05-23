#pragma once
#include "NDX12.h"
#include "NConstBuffData.h"

#include<memory>
#include <wrl.h>

template<typename T>
class NConstBuff
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> constBuff;		//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	T* constMap;	// �}�b�s���O�p

public:
	NConstBuff() :constMap(nullptr) {}
	~NConstBuff()
	{

	}

	void Init()
	{
		HRESULT result;
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES heapProp{};	//�q�[�v
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
		
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC resDesc{};		//���\�[�X
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = (sizeof(T) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//�萔�o�b�t�@�̐���
		result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
			&heapProp,	//�q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&resDesc,	//���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff)
		);
		assert(SUCCEEDED(result));
		
		//�萔�o�b�t�@�̃}�b�s���O
		result = constBuff->Map(0, nullptr, (void**)&constMap);	//�}�b�s���O
		assert(SUCCEEDED(result));

		constBuff->Unmap(0, nullptr);
	};

	inline void Unmap(){ constBuff->Unmap(0, nullptr); }
};