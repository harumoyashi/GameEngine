#include "NPostEffect.h"
#include "NWindows.h"
#include "NVector4.h"
#include "NMathUtil.h"

NPostEffect::NPostEffect()
{
}

void NPostEffect::Init()
{
	HRESULT result;

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		(UINT)NWindows::kWin_width,
		(UINT)NWindows::kWin_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	D3D12_HEAP_PROPERTIES texHeapProp;
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;	//転送はCPU側(L0)から直接行う
	texHeapProp.CreationNodeMask = 0;							//単一アダプターだから0
	texHeapProp.VisibleNodeMask = 0;

	//テクスチャバッファの生成
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr, IID_PPV_ARGS(&texBuff_)
	);
	assert(SUCCEEDED(result));

	//------------------------------テクスチャ生成--------------------------------//
	//一辺のピクセル数
	const uint32_t rowPitch = sizeof(NVector4) * NWindows::kWin_width;
	const uint32_t depthPitch = rowPitch * NWindows::kWin_height;
	//配列の要素数
	const uint32_t imageDataCount = NWindows::kWin_width * NWindows::kWin_height;
	//画像イメージデータ配列
	std::vector<NVector4> imageData;

	//全ピクセルの色を初期化
	for (size_t i = 0; i < imageDataCount; i++)
	{
		imageData.emplace_back(NVector4(1, 0, 1, 1));
	}

	//テクスチャバッファにデータ転送
	result = texBuff_->WriteToSubresource(
		0,
		nullptr,	//全領域へコピー
		imageData.data(),	//元データアドレス
		rowPitch,	//1ラインサイズ
		depthPitch	//全サイズ
	);

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV用デスクリプタヒープを生成
	result = NDX12::GetInstance()->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV作成
	NDX12::GetInstance()->GetDevice()->CreateShaderResourceView(
		texBuff_.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);

	// ---------------------------頂点関連----------------------------- //
	//頂点情報格納用
	std::vector<NVertexUV> vert(4);

	//アンカーポイント設定
	float left = 0.0f * texresDesc.Width;
	float right = 1.0f * texresDesc.Width;
	float top = 0.0f * texresDesc.Height;
	float bottom = 1.0f * texresDesc.Height;

	//設定したアンカーポイントに合わせて頂点設定しなおす
	vert[0].pos = { left ,bottom,0.0f };	// 左下
	vert[1].pos = { left ,top   ,0.0f };	// 左上
	vert[2].pos = { right,bottom,0.0f };	// 右下
	vert[3].pos = { right,top   ,0.0f };	// 右上

	//テクスチャサイズをもとに切り取る部分のuvを計算
	float tex_left = texLeftTop_.x / texresDesc.Width;
	float tex_right = (texLeftTop_.x + texSize_.x) / texresDesc.Width;
	float tex_top = texLeftTop_.y / texresDesc.Height;
	float tex_bottom = (texLeftTop_.y + texSize_.y) / texresDesc.Height;

	//計算したuvに合わせて設定
	vert[0].uv = { tex_left ,tex_bottom };	// 左下
	vert[1].uv = { tex_left ,tex_top };	// 左上
	vert[2].uv = { tex_right,tex_bottom };	// 右下
	vert[3].uv = { tex_right,tex_top };	// 右上

	//頂点バッファへのデータ転送
	vertexBuff_.Init(vert);

	//平行投影を代入
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::kWin_width),
		static_cast<float>(NWindows::kWin_height)
	);
	Update();
}

void NPostEffect::Draw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSetSprite().pipelineState_.Get());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSetSprite().rootSig_.entity_.Get());

	// プリミティブ形状の設定コマンド
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形ストリップ

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { descHeapSRV.Get() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());

	//ポストエフェクト用に作ったSRVをルートパラメータ1番に設定
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBuff_.view_);

	//ルートパラメータ0番に定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbTrans_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}
