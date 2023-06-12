#include "NSprite.h"
#include "NMathUtil.h"

NSprite::NSprite()
{
	
}

NSprite::~NSprite()
{
}

void NSprite::CreateSprite(std::string texHandle)
{
	SetTexHandle(texHandle);
	//頂点
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff();
	VertMaping();
	CreateVertBuffView();
	//定数バッファ
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform2D>>();
	cbTrans_->Init();
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbColor_->Init();
	//平行投影を代入
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::sWin_width),
		static_cast<float>(NWindows::sWin_height)
	);
	Update();
}

void NSprite::CreateSprite(std::string texHandle,
	NVector2 anchorPoint_, bool isFlipX_, bool isFlipY_)
{
	SetTexHandle(texHandle);
	//頂点
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff();
	MatchTexSize(NTextureManager::GetInstance()->textureMap_[texHandle_].texBuff_);	//ここでテクスチャサイズに合わせてる
	SetAncor(anchorPoint_);
	SetIsFlip(isFlipX_, isFlipY_);
	SetClipRange();
	TransferVertex();
	VertMaping();
	CreateVertBuffView();
	//定数バッファ
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform2D>>();
	cbTrans_->Init();
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbColor_->Init();
	//平行投影を代入
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::sWin_width),
		static_cast<float>(NWindows::sWin_height)
	);
	Update();
}

void NSprite::CreateClipSprite(std::string texHandle,
	NVector2 texLeftTop_, NVector2 texSize_, NVector2 anchorPoint_, bool isFlipX_, bool isFlipY_)
{
	SetTexHandle(texHandle);
	//頂点
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff();
	MatchTexSize(NTextureManager::GetInstance()->textureMap_[texHandle_].texBuff_);	//ここでテクスチャサイズに合わせてる
	SetAncor(anchorPoint_);
	SetIsFlip(isFlipX_, isFlipY_);
	SetClipRange(texLeftTop_, texSize_);
	TransferVertex();
	VertMaping();
	CreateVertBuffView();
	//定数バッファ
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform2D>>();
	cbTrans_->Init();
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbColor_->Init();
	//平行投影を代入
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::sWin_width),
		static_cast<float>(NWindows::sWin_height)
	);
	Update();
}

void NSprite::SetVert()
{
	std::vector<NVertexUV> vert = {
		//	x		y		z	 	u	v
		{{   0.0f, 100.0f, 0.0f }, {0.0f,1.0f}},	// 左下
		{{   0.0f,   0.0f, 0.0f }, {0.0f,0.0f}},	// 左上
		{{ 100.0f, 100.0f, 0.0f }, {1.0f,1.0f}},	// 右下
		{{ 100.0f,   0.0f, 0.0f }, {1.0f,0.0f}},	// 右上
	};

	//設定したら他でも使える変数に代入
	std::copy(std::begin(vert), std::end(vert), vertices_);

	//頂点バッファのサイズを代入
	singleSizeVB_ = static_cast<uint32_t>(sizeof(vertices_[0]));
	singleVB_ = static_cast<uint32_t>(sizeof(vertices_));
}

void NSprite::SetVertHeap()
{
	//ヒープ設定
	heapPropVert_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	heapPropVert_.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapPropVert_.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
}

void NSprite::SetVertResource()
{
	//リソース設定
	resDescVert_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDescVert_.Width = singleVB_;			//頂点データ全体のサイズ
	resDescVert_.Height = 1;
	resDescVert_.DepthOrArraySize = 1;
	resDescVert_.MipLevels = 1;
	resDescVert_.Format = DXGI_FORMAT_UNKNOWN;
	resDescVert_.SampleDesc.Count = 1;
	resDescVert_.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDescVert_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NSprite::CreateVertBuff()
{
	HRESULT result;

	// 頂点バッファの生成
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapPropVert_, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDescVert_, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));
}

void NSprite::MatchTexSize(ComPtr<ID3D12Resource> texBuff_)
{
	resDescVert_ = texBuff_->GetDesc();
	size_ = { (float)resDescVert_.Width,(float)resDescVert_.Height };
}

void NSprite::SetAncor(NVector2 anchorPoint)
{
	anchorPoint_ = anchorPoint;
}

void NSprite::SetIsFlip(bool isFlipX, bool isFlipY)
{
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;
}

void NSprite::SetClipRange(NVector2 texLeftTop, NVector2 texSize)
{
	texLeftTop_ = texLeftTop;
	texSize_ = texSize;
	size_ = texSize;
}

void NSprite::SetClipRange()
{
	texLeftTop_ = { 0,0 };
	texSize_ = size_;
}

void NSprite::VertMaping()
{
	HRESULT result;

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	std::copy(std::begin(vertices_), std::end(vertices_), vertMap_);	//座標をコピー
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
}

void NSprite::CreateVertBuffView()
{
	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = singleVB_;
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = singleSizeVB_;
}

void NSprite::SetTexHandle(std::string texHandle)
{
	texHandle_ = texHandle;
	if (NTextureManager::GetInstance()->textureMap_[texHandle_].texBuff_ == nullptr)
	{
		texHandle_ = "error";
	}
}

void NSprite::Update()
{
	//ワールド行列
	NMatrix4 matRot;	//回転行列
	matRot = matRot.RotateZ(MathUtil::Degree2Radian(rotation_));	//Z軸周りに回転

	NMatrix4 matTrans;	//平行移動行列
	matTrans = matTrans.Translation({ position_.x, position_.y, 0 });

	matWorld_ = matWorld_.Identity();	//単位行列代入
	matWorld_ *= matRot;		//ワールド座標に回転を反映
	matWorld_ *= matTrans;	//ワールド座標に平行移動を反映

	// 定数バッファへデータ転送
	cbTrans_->constMap_->mat = matWorld_ * matProjection_;
	cbColor_->constMap_->color = color_;
}

void NSprite::TransferVertex()
{
	//UVだけデフォルトで設定
	std::vector<NVertexUV> vert = {
		//		u	v
		{{}, {0.0f,1.0f}},	// 左下
		{{}, {0.0f,0.0f}},	// 左上
		{{}, {1.0f,1.0f}},	// 右下
		{{}, {1.0f,0.0f}},	// 右上
	};

	//アンカーポイント設定
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	//左右反転
	if (isFlipX_)
	{
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY_)
	{
		top = -top;
		bottom = -bottom;
	}

	//設定したアンカーポイントに合わせて頂点設定しなおす
	vert[0].pos = { left ,bottom,0.0f };	// 左下
	vert[1].pos = { left ,top   ,0.0f };	// 左上
	vert[2].pos = { right,bottom,0.0f };	// 右下
	vert[3].pos = { right,top   ,0.0f };	// 右上

	resDescVert_ = NTextureManager::GetInstance()->textureMap_[texHandle_].texBuff_->GetDesc();

	//テクスチャサイズをもとに切り取る部分のuvを計算
	float tex_left = texLeftTop_.x / resDescVert_.Width;
	float tex_right = (texLeftTop_.x + texSize_.x) / resDescVert_.Width;
	float tex_top = texLeftTop_.y / resDescVert_.Height;
	float tex_bottom = (texLeftTop_.y + texSize_.y) / resDescVert_.Height;

	//計算したuvに合わせて設定しなおす
	vert[0].uv = { tex_left ,tex_bottom };	// 左下
	vert[1].uv = { tex_left ,tex_top };	// 左上
	vert[2].uv = { tex_right,tex_bottom };	// 右下
	vert[3].uv = { tex_right,tex_top };	// 右上

	//設定したら他でも使える変数に代入
	std::copy(std::begin(vert), std::end(vert), vertices_);

	//頂点バッファのサイズを代入
	singleSizeVB_ = static_cast<uint32_t>(sizeof(vertices_[0]));
	singleVB_ = static_cast<uint32_t>(sizeof(vertices_));

	//頂点バッファへのデータ転送
	VertMaping();
}

void NSprite::SetSize(float x, float y)
{
	size_ = { x,y };
	TransferVertex();
}

void NSprite::SetPos(float x, float y)
{
	position_ = { x,y };
	Update();
}

void NSprite::CommonBeginDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSetSprite().pipelineState_.Get());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSetSprite().rootSig_.entity_.Get());

	// プリミティブ形状の設定コマンド
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形ストリップ

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void NSprite::Draw()
{
	CommonBeginDraw();

	//非表示フラグ立ってたら描画せずに処理抜ける
	if (isInvisible_)
	{
		return;
	}

	//ハンドルを指定
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = NTextureManager::GetInstance()->textureMap_[texHandle_].gpuHandle_;

	//指定のヒープにあるSRVをルートパラメータ1番に設定
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);

	//ルートパラメータ0番に定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbTrans_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}