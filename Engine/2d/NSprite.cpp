#include "NSprite.h"
#include "NMathUtil.h"

NSprite::NSprite()
{
	//定数バッファ
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform2D>>();
	cbTrans_->Init();
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbColor_->Init();
}

NSprite::~NSprite()
{
}

void NSprite::CreateSprite(const std::string& texHandle)
{
	SetTexHandle(texHandle);
	//頂点
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff();
	CreateVertBuffView();
	
	//平行投影を代入
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::kWin_width),
		static_cast<float>(NWindows::kWin_height)
	);
	Update();
}

void NSprite::CreateSprite(const std::string& texHandle,
	const NVector2& anchorPoint_, const bool isFlipX_, const bool isFlipY_)
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
	CreateVertBuffView();
	
	//平行投影を代入
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::kWin_width),
		static_cast<float>(NWindows::kWin_height)
	);
	Update();
}

void NSprite::CreateClipSprite(const std::string& texHandle,
	const NVector2& texLeftTop_, const NVector2& texSize_, const NVector2& anchorPoint_,
	const bool isFlipX_, const bool isFlipY_)
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
	CreateVertBuffView();
	
	//平行投影を代入
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::kWin_width),
		static_cast<float>(NWindows::kWin_height)
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
	vertices_ = vert;

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
	vertexBuff_.Init(vertices_);
}

void NSprite::MatchTexSize(const ComPtr<ID3D12Resource>& texBuff_)
{
	resDescVert_ = texBuff_->GetDesc();
	size_ = { (float)resDescVert_.Width,(float)resDescVert_.Height };
}

void NSprite::SetAncor(const NVector2& anchorPoint)
{
	anchorPoint_ = anchorPoint;
}

void NSprite::SetIsFlip(bool isFlipX, bool isFlipY)
{
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;
}

void NSprite::SetClipRange(const NVector2& texLeftTop, const NVector2& texSize)
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

void NSprite::CreateVertBuffView()
{
	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	vertexBuff_.view_.BufferLocation = vertexBuff_.buff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vertexBuff_.view_.SizeInBytes = singleVB_;
	// 頂点1つ分のデータサイズ
	vertexBuff_.view_.StrideInBytes = singleSizeVB_;
}

void NSprite::SetTexHandle(const std::string& texHandle)
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
	vertices_ = vert;

	//頂点バッファのサイズを代入
	singleSizeVB_ = static_cast<uint32_t>(sizeof(vertices_[0]));
	singleVB_ = static_cast<uint32_t>(sizeof(vertices_));

	//頂点バッファへのデータ転送
	vertexBuff_.Init(vert);
}

void NSprite::SetSize(const float x, const float y)
{
	size_ = { x,y };
	TransferVertex();
}

void NSprite::SetPos(const float x, const float y)
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

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBuff_.view_);

	//ルートパラメータ0番に定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbTrans_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}