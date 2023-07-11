#pragma once
#include "NDX12.h"

#include <wrl.h>

class NPreDraw
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	uint32_t bbIndex_;
	D3D12_RESOURCE_BARRIER barrierDesc_{};		//���\�[�X�o���A
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_{};	//�����_�[�^�[�Q�b�g�r���[�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_{};	//�f�X�N���v�^�n���h��

	D3D12_VIEWPORT viewport_{};	//�r���[�|�[�g
	D3D12_RECT scissorRect_{};	//�V�U�[��`

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
	void BarrierReset(D3D12_RESOURCE_BARRIER& barrierDesc);
	//���߂̃N���[�Y
	//�����R�}���h���X�g�ɐςނ̂����܂�
	void CmdListClose();
	// �R�}���h���X�g�̎��s
	void ExecuteCmdList();
	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
	void BufferSwap();
	// �R�}���h�̎��s������҂�
	void CommandWait(UINT64& fenceVal);
	// �L���[���N���A
	void ClearQueue();
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	void CmdListReset();
};

