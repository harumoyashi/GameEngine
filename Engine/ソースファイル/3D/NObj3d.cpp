#include "NObj3d.h"
#include "NInput.h"
#include "NTexture.h"
#include "NCamera.h"
#include "NMathUtil.h"
#include "NBaseCollider.h"
#include "NCollisionManager.h"

NLightGroup* NObj3d::sLightGroup = nullptr;

NObj3d::NObj3d()
{
	Init();
}

NObj3d::~NObj3d()
{
}

bool NObj3d::Init()
{
	//定数バッファ
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform>>();
	cbMaterial_ = std::make_unique<NConstBuff<ConstBuffDataMaterial>>();
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbSkin_ = std::make_unique<NConstBuff<ConstBuffDataSkin>>();
	cbTrans_->Init();
	cbMaterial_->Init();
	cbColor_->Init();
	cbSkin_->Init();
	color_ = NColor::kWhite;
	isElapseAnime_ = true;	//経過時間に影響を受ける状態で生成

	objName_ = typeid(*this).name();
	return true;
}

void NObj3d::Update()
{
	UpdateMatrix();
	TransferMaterial();
	TransferColor();
	if (model_->format == ModelFormat::Fbx)
	{
		TransferSkin();
	}
}

void NObj3d::MoveKey()
{
	//いずれかのキーを押したとき
	if (NInput::IsKey(DIK_W) || NInput::IsKey(DIK_S) || NInput::IsKey(DIK_D) || NInput::IsKey(DIK_A))
	{
		if (NInput::IsKey(DIK_W)) { position_.y += 0.5f; }
		else if (NInput::IsKey(DIK_S)) { position_.y -= 0.5f; }
		if (NInput::IsKey(DIK_D)) { position_.x += 0.5f; }
		else if (NInput::IsKey(DIK_A)) { position_.x -= 0.5f; }
	}

	if (NInput::IsKey(DIK_Q)) { position_.z += 0.5f; }
	else if (NInput::IsKey(DIK_E)) { position_.z -= 0.5f; }

	if (NInput::IsKey(DIK_U)) { rotation_.y += 0.5f; }
	else if (NInput::IsKey(DIK_I)) { rotation_.y -= 0.5f; }
	if (NInput::IsKey(DIK_J)) { rotation_.x += 0.5f; }
	else if (NInput::IsKey(DIK_K)) { rotation_.x -= 0.5f; }
}

void NObj3d::UpdateMatrix()
{
	//ワールド行列
	NMatrix4 matScale;	//スケーリング行列
	matScale = matScale.Scale(scale_);

	NMatrix4 matRot;		//回転行列
	NMatrix4 matZ = matZ.RotateZ(MathUtil::Degree2Radian(rotation_.z));
	NMatrix4 matX = matX.RotateX(MathUtil::Degree2Radian(rotation_.x));
	NMatrix4 matY = matY.RotateY(MathUtil::Degree2Radian(rotation_.y));
	matRot *= matZ;	//Z軸周りに回転してから
	matRot *= matX;	//X軸周りに回転して
	matRot *= matY;	//Y軸周りに回転

	NMatrix4 matTrans;	//平行移動行列
	matTrans = matTrans.Translation(position_);

	matWorld_ = NMatrix4::Identity();	//単位行列代入
	matWorld_ *= matScale;	//ワールド座標にスケーリングを反映
	matWorld_ *= matRot;	//ワールド座標に回転を反映
	matWorld_ *= matTrans;	//ワールド座標に平行移動を反映

	//親オブジェクトがあれば
	if (parent_ != nullptr)
	{
		//親オブジェクトのワールド行列をかける
		matWorld_ *= parent_->matWorld_;
	}

	// 定数バッファへデータ転送
	TransferMatrix();
}

void NObj3d::TransferMatrix()
{
	HRESULT result;
	// 定数バッファへデータ転送
	cbTrans_->constMap_ = nullptr;
	result = cbTrans_->constBuff_->Map(0, nullptr, (void**)&cbTrans_->constMap_);

	cbTrans_->constMap_->viewproj = NCamera::sCurrentCamera->GetMatView() * NCamera::sCurrentCamera->GetMatProjection();
	cbTrans_->constMap_->world = matWorld_;
	cbTrans_->constMap_->cameraPos = NCamera::sCurrentCamera->GetPos();

	cbTrans_->Unmap();
}

void NObj3d::TransferColor()
{
	cbColor_->constMap_->color = color_;
}

void NObj3d::TransferMaterial()
{
	cbMaterial_->constMap_->ambient = model_->material.ambient;
	cbMaterial_->constMap_->diffuse = model_->material.diffuse;
	cbMaterial_->constMap_->specular = model_->material.specular;
	cbMaterial_->constMap_->alpha = model_->material.alpha;
}

void NObj3d::TransferSkin()
{
	auto fbxModel = static_cast<FbxModel*>(model_);
	fbxModel->SetIsPlayAnime(true);
	fbxModel->PlayAnimation(isElapseAnime_);

	ConstBuffDataSkin skinData{};
	for (uint32_t i = 0; i < fbxModel->bones.size(); i++)
	{
		skinData.bones[i] = fbxModel->bones[i].currentMat;
	}
	cbSkin_->constMap_->bones = skinData.bones;
}

void NObj3d::CommonBeginDraw()
{
	// プリミティブ形状の設定コマンド
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void NObj3d::SetBlendMode(BlendMode blendMode)
{
	// パイプラインステートとルートシグネチャの設定コマンド
	if (model_->format == ModelFormat::Obj)
	{
		switch (blendMode)
		{
		case BlendMode::None:
			NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("ObjNone"));
			NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("ObjNone")->pRootSignature);
			break;
		case BlendMode::Alpha:
			NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("ObjAlpha"));
			NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("ObjAlpha")->pRootSignature);
			break;
		case BlendMode::Add:
			NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("ObjAdd"));
			NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("ObjAdd")->pRootSignature);
			break;
		case BlendMode::Sub:
			NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("ObjSub"));
			NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("ObjSub")->pRootSignature);
			break;
		case BlendMode::Inv:
			NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("ObjInv"));
			NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("ObjInv")->pRootSignature);
			break;
		default:
			break;
		}
	}
	else if (model_->format == ModelFormat::Fbx)
	{
		switch (blendMode)
		{
		case BlendMode::None:
			NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("FbxNone"));
			NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("FbxNone")->pRootSignature);
			break;
		case BlendMode::Alpha:
			NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("FbxAlpha"));
			NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("FbxAlpha")->pRootSignature);
			break;
		case BlendMode::Add:
			NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("FbxAdd"));
			NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("FbxAdd")->pRootSignature);
			break;
		case BlendMode::Sub:
			NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("FbxSub"));
			NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("FbxSub")->pRootSignature);
			break;
		case BlendMode::Inv:
			NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("FbxInv"));
			NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("FbxInv")->pRootSignature);
			break;
		default:
			break;
		}
	}
}

void NObj3d::Draw()
{
	SetCBV();
	SetVB(model_->mesh.vertexBuff.GetView());
	SetIB(*model_->mesh.indexBuff.GetView());
	SetSRVHeap(texture_->gpuHandle_);
	//ライトの描画
	sLightGroup->Draw(4);
	DrawCommand((uint32_t)model_->mesh.indices.size());
}

void NObj3d::SetSRVHeap(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
{
	//指定のヒープにあるSRVをルートパラメータ0番に設定
	if (gpuHandle.ptr == 0)
	{
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, NTextureManager::GetInstance()->textureMap_["error"].gpuHandle_);
	}
	else
	{
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, gpuHandle);
	}
}

void NObj3d::SetVB(const D3D12_VERTEX_BUFFER_VIEW* vbView)
{
	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, vbView);
}

void NObj3d::SetIB(const D3D12_INDEX_BUFFER_VIEW& ibView)
{
	NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);
}

void NObj3d::SetCBV()
{
	//ルートパラメータ1番にマテリアルの定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, cbMaterial_->constBuff_->GetGPUVirtualAddress());
	//ルートパラメータ2番に色情報の定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	//ルートパラメータ3番に3D変換行列の定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, cbTrans_->constBuff_->GetGPUVirtualAddress());
	if (model_->format == ModelFormat::Fbx)
	{
		//ルートパラメータ5番に3D変換行列の定数バッファを渡す
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cbSkin_->constBuff_->GetGPUVirtualAddress());
	}
}

void NObj3d::DrawCommand(const uint32_t indexSize)
{
	NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced(indexSize, 1, 0, 0, 0); //インデックスを使って描画
}

void NObj3d::SetModel(const std::string& modelname)
{
	model_ = NModelManager::GetModel(modelname);
	//他で使う用にテクスチャとモデル形式を別で保存
	texture_ = &model_->material.texture;
}

void NObj3d::SetTexture(const std::string& texname)
{
	texture_ = &NTextureManager::GetInstance()->textureMap_[texname];
}
