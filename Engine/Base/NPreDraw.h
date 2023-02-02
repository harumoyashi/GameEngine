#pragma once
#include "NDX12.h"

#include <wrl.h>

class NPreDraw
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	UINT bbIndex;
	D3D12_RESOURCE_BARRIER barrierDesc{};		//���\�[�X�o���A
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{};	//�����_�[�^�[�Q�b�g�r���[�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle{};	//�f�X�N���v�^�n���h��

	D3D12_VIEWPORT viewport{};	//�r���[�|�[�g
	D3D12_RECT scissorRect{};	//�V�U�[��`

public:
	//���\�[�X�o���A�ŏ������݉\�ɕύX
	void SetResBarrier();
	//�`���̕ύX
	void SetRenderTarget();
	//��ʃN���ARGBA
	void ClearScreen();
	//�r���[�|�[�g�ݒ�
	void SetViewport();
	//�V�U�[��`�ݒ�
	void SetScissorRect();

	//�o���A����
	void BarrierReset(D3D12_RESOURCE_BARRIER barrierDesc);
	//���߂̃N���[�Y
	//�����R�}���h���X�g�ɐςނ̂����܂�
	void CmdListClose();
	// �R�}���h���X�g�̎��s
	void ExecuteCmdList();
	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
	void BufferSwap();
	// �R�}���h�̎��s������҂�
	void CommandWait(UINT64 fenceVal);
	// �L���[���N���A
	void ClearQueue();
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	void CmdListReset();
};

