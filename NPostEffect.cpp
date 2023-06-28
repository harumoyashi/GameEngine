#include "NPostEffect.h"
#include "NWindows.h"
#include "NVector4.h"
#include "NMathUtil.h"
#include "NInput.h"

const float NPostEffect::kClearColor[4] = { 0.25f,0.5f,0.1f,0.0f };

NPostEffect::NPostEffect()
{
}

void NPostEffect::Init()
{
	CreateTexture();
	CreateRTV();
	CreateDepthBuff();
	CreateDSV();
}

void NPostEffect::TexChange()
{
	//SRV設定
	if (NInput::GetInstance()->IsKeyDown(DIK_0))
	{
		//テクスチャ番号を01で切り替え
		static uint32_t texNum = 0;
		texNum = (texNum + 1) % 2;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = 1;

		//デスクリプタヒープにSRV作成
		NDX12::GetInstance()->GetDevice()->CreateShaderResourceView(
			texBuff_[texNum].Get(),
			&srvDesc,
			descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
		);
	}
}

void NPostEffect::Draw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSetPostEffect().pipelineState_.Get());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSetPostEffect().rootSig_.entity_.Get());

	// プリミティブ形状の設定コマンド
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形ストリップ

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { descHeapSRV_.Get() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());

	//ポストエフェクト用に作ったSRVをルートパラメータ1番に設定
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV_->GetGPUDescriptorHandleForHeapStart());

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBuff_.view_);

	//ルートパラメータ0番に定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbTrans_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}

void NPostEffect::CreateTexture()
{
	HRESULT result;

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
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
	CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kClearColor);

	for (uint32_t i = 0; i < 2; i++)
	{
		result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(&texBuff_[i])
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
		for (size_t j = 0; j < imageDataCount; j++)
		{
			imageData.emplace_back(NVector4(1, 0, 1, 1));
		}

		//テクスチャバッファにデータ転送
		result = texBuff_[i]->WriteToSubresource(
			0,
			nullptr,	//全領域へコピー
			imageData.data(),	//元データアドレス
			rowPitch,	//1ラインサイズ
			depthPitch	//全サイズ
		);
	}

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV用デスクリプタヒープを生成
	result = NDX12::GetInstance()->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV_));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV作成
	NDX12::GetInstance()->GetDevice()->CreateShaderResourceView(
		texBuff_[0].Get(),
		&srvDesc,
		descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
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
	//vert[0].uv = { tex_left ,tex_bottom };	// 左下
	//vert[1].uv = { tex_left ,tex_top };	// 左上
	//vert[2].uv = { tex_right,tex_bottom };	// 右下
	//vert[3].uv = { tex_right,tex_top };	// 右上

	vert[0].uv = { 0 ,1 };	// 左下
	vert[1].uv = { 0 ,0 };	// 左上
	vert[2].uv = { 1,1 };	// 右下
	vert[3].uv = { 1,0 };	// 右上

	//頂点バッファへのデータ転送
	vertexBuff_.Init(vert);

	//平行投影を代入
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::kWin_width),
		static_cast<float>(NWindows::kWin_height)
	);
	Update();
}

void NPostEffect::CreateRTV()
{
	HRESULT result;

	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;
	//RTV用デスクリプタヒープを生成
	result = NDX12::GetInstance()->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
	assert(SUCCEEDED(result));

	//レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//デスクリプタヒープにRTV作成
	for (uint32_t i = 0; i < 2; i++)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle(
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), i,
				NDX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));

		NDX12::GetInstance()->GetDevice()->CreateRenderTargetView(
			texBuff_[i].Get(),
			nullptr,
			cpuHandle
		);
	}
}

void NPostEffect::CreateDepthBuff()
{
	HRESULT result;

	//リソース設定
	D3D12_RESOURCE_DESC depthResDesc{};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = NWindows::kWin_width;	//レンダーターゲットに合わせる
	depthResDesc.Height = NWindows::kWin_height;	//レンダーターゲットに合わせる
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//デプスステンシル

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	//深度バッファ生成
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_)
	);
	assert(SUCCEEDED(result));
}

void NPostEffect::CreateDSV()
{
	HRESULT result;

	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;	//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//デプスステンシルビュー
	//DSV用デスクリプタヒープ作成
	result = NDX12::GetInstance()->GetDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	NDX12::GetInstance()->GetDevice()->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc, descHeapDSV_->GetCPUDescriptorHandleForHeapStart()
	);
}

void NPostEffect::PreDrawScene()
{
	// ------------------リソースバリアを書き込み専用状態に変更----------------------- //
	D3D12_RESOURCE_BARRIER barrierDesc{};
	for (uint32_t i = 0; i < 2; i++)
	{
		barrierDesc.Transition.pResource = texBuff_[i].Get();
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;	//読み込み専用状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;				//書き込み専用状態へ
		NDX12::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrierDesc);
	}

	//レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle[2];
	for (uint32_t i = 0; i < 2; i++)
	{
		rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV_->GetCPUDescriptorHandleForHeapStart(),
			NDX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}
	//デプスステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart();
	//レンダーテクスチャをレンダーターゲットに指定
	NDX12::GetInstance()->GetCommandList()->OMSetRenderTargets(2, rtvHandle, false, &dsvHandle);

	// -----------------------その他もろもろ-------------------------- //
	//ビューポートの設定
	CD3DX12_VIEWPORT viewport[2];
	for (uint32_t i = 0; i < 2; i++)
	{
		viewport[i] = CD3DX12_VIEWPORT(0.0f, 0.0f,
			NWindows::kWin_width, NWindows::kWin_height);
	}

	NDX12::GetInstance()->GetCommandList()->RSSetViewports(2, viewport);

	//シザリング矩形の設定
	CD3DX12_RECT rect[2];
	for (uint32_t i = 0; i < 2; i++)
	{
		rect[i] = CD3DX12_RECT(0, 0,
			NWindows::kWin_width, NWindows::kWin_height);
	}

	NDX12::GetInstance()->GetCommandList()->RSSetScissorRects(2, rect);

	//全画面クリア
	for (uint32_t i = 0; i < 2; i++)
	{
		NDX12::GetInstance()->GetCommandList()->ClearRenderTargetView(rtvHandle[i], kClearColor, 0, nullptr);
	}
	//深度バッファのクリア
	NDX12::GetInstance()->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void NPostEffect::PostDrawScene()
{
	//リソースバリアを変更(書き込み専用状態から読み取り専用状態に)
	D3D12_RESOURCE_BARRIER barrierDesc{};
	for (uint32_t i = 0; i < 2; i++)
	{
		barrierDesc.Transition.pResource = texBuff_[i].Get();
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;		//書き込み専用状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;	//読み込み専用状態へ
		NDX12::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrierDesc);
	}
}
