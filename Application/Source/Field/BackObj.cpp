#include "BackObj.h"

bool BackObj::Init()
{
	//定数バッファ
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform>>();
	cbMaterial_ = std::make_unique<NConstBuff<ConstBuffDataMaterial>>();
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbBackObj_ = std::make_unique<NConstBuff<ConstBuffDataBackObj>>();
	cbTrans_->Init();
	cbMaterial_->Init();
	cbColor_->Init();
	cbBackObj_->Init();
	color_ = NColor::kWhite;

	objName_ = typeid(*this).name();
	return true;
}

void BackObj::Update()
{
	UpdateMatrix();
	TransferMaterial();
	TransferColor();
	TransferCBBackObj();
}

void BackObj::CommonBeginDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("BackObjNone"));
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("BackObjNone")->pRootSignature);

	// プリミティブ形状の設定コマンド
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void BackObj::Draw()
{
	SetCBV();
	//タイル用の定数バッファ渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cbBackObj_->constBuff_->GetGPUVirtualAddress());
	SetVB(model_->mesh.vertexBuff.GetView());
	SetIB(*model_->mesh.indexBuff.GetView());
	SetSRVHeap(texture_->gpuHandle_);
	//ライトの描画
	sLightGroup->Draw(4);
	DrawCommand((uint32_t)model_->mesh.indices.size());
}

void BackObj::TransferCBBackObj()
{
	cbBackObj_->constMap_->isAvoid = isAvoid_;
}