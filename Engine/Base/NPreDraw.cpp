#include "NPreDraw.h"

void NPreDraw::SetResBarrier()
{
	// バックバッファの番号を取得(2つなので0番か1番)
	bbIndex_ = NDX12::GetInstance()->GetSwapchain()->GetCurrentBackBufferIndex();
	// 1.リソースバリアで書き込み可能に変更
	barrierDesc_.Transition.pResource = NDX12::GetInstance()->backBuffers_[bbIndex_].Get(); // バックバッファを指定
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
	NDX12::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrierDesc_);
}

void NPreDraw::SetRenderTarget()
{
	// 2.描画先の変更
	// レンダーターゲットビューのハンドルを取得
	rtvHandle_ = NDX12::GetInstance()->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
	rtvHandle_.ptr += bbIndex_ * NDX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(
		NDX12::GetInstance()->GetRTVHeapDesc().Type);
	dsvHandle_ = NDX12::GetInstance()->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
	NDX12::GetInstance()->GetCommandList()->OMSetRenderTargets(1, &rtvHandle_, false, &dsvHandle_);
}

void NPreDraw::ClearScreen()
{
	// 3.画面クリア R G B A
	std::vector<FLOAT> clearColor = { 0.1f,0.25f,0.5f,0.0f }; // 青っぽい色
	NDX12::GetInstance()->GetCommandList()->ClearRenderTargetView(rtvHandle_, clearColor.data(), 0, nullptr);
	NDX12::GetInstance()->GetCommandList()->ClearDepthStencilView(dsvHandle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void NPreDraw::SetViewport()
{
	viewport_.Width = NWindows::sWin_width;
	viewport_.Height = NWindows::sWin_height;
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;	//最小震度
	viewport_.MaxDepth = 1.0f;	//最大深度
	// ビューポート設定コマンドを、コマンドリストに積む
	NDX12::GetInstance()->GetCommandList()->RSSetViewports(1, &viewport_);
}

void NPreDraw::SetScissorRect()
{
	scissorRect_.left = 0; // 切り抜き座標左
	scissorRect_.right = scissorRect_.left + NWindows::sWin_width; // 切り抜き座標右
	scissorRect_.top = 0; // 切り抜き座標上
	scissorRect_.bottom = scissorRect_.top + NWindows::sWin_height; // 切り抜き座標下
	// シザー矩形設定コマンドを、コマンドリストに積む
	NDX12::GetInstance()->GetCommandList()->RSSetScissorRects(1, &scissorRect_);
}

void NPreDraw::BarrierReset(D3D12_RESOURCE_BARRIER barrierDesc)
{
	// 5.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//表示状態へ
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
	std::vector<ID3D12CommandList*> commandLists = { NDX12::GetInstance()->GetCommandList()};
	NDX12::GetInstance()->GetCommandQueue()->ExecuteCommandLists(1, commandLists.data());
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