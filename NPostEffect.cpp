#include "NPostEffect.h"

NPostEffect::NPostEffect()
{
}

void NPostEffect::Draw()
{
	CommonBeginDraw();

	//非表示フラグ立ってたら描画せずに処理抜ける
	if (isInvisible_)
	{
		return;
	}

	//ハンドルを指定
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = NTextureManager::GetInstance()->textureMap_["error"].gpuHandle_;

	//指定のヒープにあるSRVをルートパラメータ1番に設定
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBuff_.view_);

	//ルートパラメータ0番に定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbTrans_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}
