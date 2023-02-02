#include "NPreDraw.h"

void NPreDraw::SetResBarrier()
{
	// �o�b�N�o�b�t�@�̔ԍ����擾(2�Ȃ̂�0�Ԃ�1��)
	bbIndex = NDX12::GetInstance()->GetSwapchain()->GetCurrentBackBufferIndex();
	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc.Transition.pResource = NDX12::GetInstance()->backBuffers[bbIndex].Get(); // �o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
	NDX12::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrierDesc);
}

void NPreDraw::SetRenderTarget()
{
	// 2.�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	rtvHandle = NDX12::GetInstance()->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * NDX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(
		NDX12::GetInstance()->GetRTVHeapDesc().Type);
	dsvHandle = NDX12::GetInstance()->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
	NDX12::GetInstance()->GetCommandList()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
}

void NPreDraw::ClearScreen()
{
	// 3.��ʃN���A R G B A
	FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f }; // ���ۂ��F
	NDX12::GetInstance()->GetCommandList()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	NDX12::GetInstance()->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void NPreDraw::SetViewport()
{
	viewport.Width = NWindows::win_width;
	viewport.Height = NWindows::win_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;	//�ŏ��k�x
	viewport.MaxDepth = 1.0f;	//�ő�[�x
	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	NDX12::GetInstance()->GetCommandList()->RSSetViewports(1, &viewport);
}

void NPreDraw::SetScissorRect()
{
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + NWindows::win_width; // �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + NWindows::win_height; // �؂蔲�����W��
	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	NDX12::GetInstance()->GetCommandList()->RSSetScissorRects(1, &scissorRect);
}

void NPreDraw::BarrierReset(D3D12_RESOURCE_BARRIER barrierDesc)
{
	// 5.���\�[�X�o���A��߂�
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//�`���Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//�\����Ԃ�
	NDX12::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrierDesc);
}

void NPreDraw::CmdListClose()
{
	HRESULT result;

	result = NDX12::GetInstance()->GetCommandList()->Close();
	assert(SUCCEEDED(result));
}

void NPreDraw::ExecuteCmdList()
{
	ID3D12CommandList* commandLists[] = { NDX12::GetInstance()->GetCommandList()};
	NDX12::GetInstance()->GetCommandQueue()->ExecuteCommandLists(1, commandLists);
}

void NPreDraw::BufferSwap()
{
	HRESULT result;

	result = NDX12::GetInstance()->GetSwapchain()->Present(1, 0);
	assert(SUCCEEDED(result));
}

void NPreDraw::CommandWait(UINT64 fenceVal)
{
	NDX12::GetInstance()->GetCommandQueue()->Signal(NDX12::GetInstance()->GetFence(), ++fenceVal);
	if (NDX12::GetInstance()->GetFence()->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		NDX12::GetInstance()->GetFence()->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}

void NPreDraw::ClearQueue()
{
	HRESULT result;

	result = NDX12::GetInstance()->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));
}

void NPreDraw::CmdListReset()
{
	HRESULT result;

	result = NDX12::GetInstance()->GetCommandList()->Reset(NDX12::GetInstance()->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));
}