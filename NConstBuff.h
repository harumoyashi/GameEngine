#pragma once
#include "NDX12.h"
#include "NMatrix4.h"

#include<memory>
#include <wrl.h>

//�萔�o�b�t�@�p�f�[�^�\���́i3D�ϊ��s��j
struct  ConstBufferDataTransform
{
	//NMatrix4 mat;	//3D�ϊ��s��
	NMatrix4 viewproj;	// �r���[�v���W�F�N�V�����s��
	NMatrix4 world;		// ���[���h�s��
	NVector3 cameraPos;	// �J�������W(���[���h���W)
};

class NConstBuff
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_HEAP_PROPERTIES heapProp{};	//�q�[�v
	D3D12_RESOURCE_DESC resDesc{};		//���\�[�X

	ConstBufferDataTransform* constMapTransform;	//3D�ϊ��s��
	ComPtr<ID3D12Resource> constBuff;		//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^

public:
	//�q�[�v�ݒ�
	void SetHeap();
	//���\�[�X�ݒ�
	void SetResource();
	//�萔�o�b�t�@�̐���
	void Create();
	//�萔�o�b�t�@�̃}�b�s���O
	void Mapping();

	//�萔�o�b�t�@�֑��M
	void TransferMatrix(NMatrix4 matWorld);
};