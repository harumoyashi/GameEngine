#include "NTile.h"

bool NTile::Init()
{
	//�萔�o�b�t�@
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
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("TileObjNone"));
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("TileObjNone")->pRootSignature);

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void NTile::Draw()
{
	SetCBV();
	//�^�C���p�̒萔�o�b�t�@�n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cbTile_->constBuff_->GetGPUVirtualAddress());
	SetVB(model_.vertexBuff.GetView());
	SetIB(*model_.indexBuff.GetView());
	SetSRVHeap(model_.material.texture.gpuHandle_);
	//���C�g�̕`��
	sLightGroup->Draw(4);
	DrawCommand((uint32_t)model_.indices.size());
}

void NTile::TransferCBTile()
{
	cbTile_->constMap_->divide = divide_;
	cbTile_->constMap_->activityArea = activityArea_;
}
