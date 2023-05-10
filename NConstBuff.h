#pragma once
#include "NDX12.h"
#include "NConstBuffData.h"

#include<memory>
#include <wrl.h>

class NConstBuff
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_HEAP_PROPERTIES heapProp{};	//�q�[�v
	D3D12_RESOURCE_DESC resDesc{};		//���\�[�X

	ConstBufferDataTransform* constMapTransform;	//3D�ϊ��s��
	ComPtr<ID3D12Resource> constBuff;		//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^

public:
	void Init();

	//�萔�o�b�t�@�֑��M
	void TransferMatrix(NMatrix4 matWorld);

private:
	//�q�[�v�ݒ�
	void SetHeap();
	//���\�[�X�ݒ�
	void SetResource();
	//�萔�o�b�t�@�̐���
	void Create();
	//�萔�o�b�t�@�̃}�b�s���O
	void Mapping();
};