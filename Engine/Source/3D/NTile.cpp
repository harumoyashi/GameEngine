#include "NTile.h"

bool NTile::Init()
{
	//定数バッファ
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform>>();
	cbMaterial_ = std::make_unique<NConstBuff<ConstBuffDataMaterial>>();
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbTile_ = std::make_unique<NConstBuff<ConstBuffDataTile>>();
	cbTrans_->Init();
	cbMaterial_->Init();
	cbColor_->Init();
	cbTile_->Init();
	color_ = NColor::kWhite;

	objName_ = typeid(*this).name();
	return true;
}

void NTile::Update()
{
	UpdateMatrix();
	TransferMaterial();
	TransferColor();
	TransferCBTile();
}

void NTile::CommonBeginDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("TileObjAlpha"));
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("TileObjAlpha")->pRootSignature);

	// プリミティブ形状の設定コマンド
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void NTile::Draw()
{
	SetCBV();
	//タイル用の定数バッファ渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cbTile_->constBuff_->GetGPUVirtualAddress());
	SetVB(model_->mesh.vertexBuff.GetView());
	SetIB(*model_->mesh.indexBuff.GetView());
	SetSRVHeap(texture_->gpuHandle_);
	//ライトの描画
	sLightGroup->Draw(4);
	DrawCommand((uint32_t)model_->mesh.indices.size());
}

void NTile::SetObjPos(NVec3 objPos, uint32_t eleNum)
{
	objPos_[eleNum] = objPos;
}

void NTile::TransferCBTile()
{
	cbTile_->constMap_->divide = divide_;
	cbTile_->constMap_->activityArea = activityArea_;
	cbTile_->constMap_->objPos = objPos_;
	cbTile_->constMap_->isAvoid = isAvoid_;
	cbTile_->constMap_->avoidArea = avoidArea_;
}
