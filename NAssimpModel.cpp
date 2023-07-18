#include "NAssimpModel.h"
#include "NCamera.h"
#include "NGPipeline.h"
#include "NUtil.h"

NLightGroup* NAssimpModel::sLightGroup = nullptr;

void NAssimpModel::Load(const wchar_t* filename)
{
	importSetting_.filename = filename;

	//model読み込み
	if (!loader_.Load(importSetting_))
	{
		//なんか警告出したほうがよさそう
	}

	//テクスチャ読み込み
	for (size_t i = 0; i < meshes_.size(); i++)
	{
		NTextureManager::GetInstance()->LoadTexture(
			NUtil::ToUTF8(meshes_[i].textureName), NUtil::ToUTF8(meshes_[i].textureName));
	}
}

void NAssimpModel::Init()
{
	// メッシュの数だけ頂点バッファを用意する
	vertexBuffers_.reserve(meshes_.size());
	for (size_t i = 0; i < meshes_.size(); i++)
	{
		NVertexBuff<NVertexPNU> pVB;
		pVB.Init(meshes_[i].vertices);

		vertexBuffers_.emplace_back(pVB);
	}

	// メッシュの数だけインデックスバッファを用意する
	indexBuffers_.reserve(meshes_.size());
	for (size_t i = 0; i < meshes_.size(); i++)
	{
		NIndexBuff pIB = NIndexBuff(meshes_[i].indices);

		indexBuffers_.emplace_back(pIB);
	}

	//定数バッファ初期化
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform>>();
	cbMaterial_ = std::make_unique<NConstBuff<ConstBuffDataMaterial>>();
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbTrans_->Init();
	cbMaterial_->Init();
	cbColor_->Init();
}

void NAssimpModel::Update()
{
	//3D変換行列情報転送
	HRESULT result;

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

	matWorld_ = matWorld_.Identity();	//単位行列代入
	matWorld_ *= matScale;	//ワールド座標にスケーリングを反映
	matWorld_ *= matRot;	//ワールド座標に回転を反映
	matWorld_ *= matTrans;	//ワールド座標に平行移動を反映

	// 定数バッファへデータ転送
	cbTrans_->constMap_ = nullptr;
	result = cbTrans_->constBuff_->Map(0, nullptr, (void**)&cbTrans_->constMap_);

	cbTrans_->constMap_->viewproj = NCamera::sCurrentCamera->GetMatView() * NCamera::sCurrentCamera->GetMatProjection();
	cbTrans_->constMap_->world = matWorld_;
	cbTrans_->constMap_->cameraPos = NCamera::sCurrentCamera->GetPos();

	cbTrans_->Unmap();

	//マテリアル情報転送
	cbMaterial_->constMap_->ambient = material_.ambient;
	cbMaterial_->constMap_->diffuse = material_.diffuse;
	cbMaterial_->constMap_->specular = material_.specular;
	cbMaterial_->constMap_->alpha = material_.alpha;
	//色情報転送
	cbColor_->constMap_->color = color_;
}

void NAssimpModel::Draw()
{
	//メッシュの数だけインデックス分の描画を行う処理を回す
	for (size_t i = 0; i < meshes_.size(); i++)
	{
		// パイプラインステートとルートシグネチャの設定コマンド
		NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("Obj"));
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("Obj")->pRootSignature);

		// プリミティブ形状の設定コマンド
		NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
		NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());

		//ルートパラメータ1番にマテリアルの定数バッファを渡す
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, cbMaterial_->constBuff_->GetGPUVirtualAddress());
		//ルートパラメータ2番に色情報の定数バッファを渡す
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
		//ルートパラメータ2番に3D変換行列の定数バッファを渡す
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, cbTrans_->constBuff_->GetGPUVirtualAddress());

		NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffers_[i].GetView());
		NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(indexBuffers_[i].GetView());

		//テクスチャの名前取得
		std::string texName = NUtil::ToUTF8(meshes_[i].textureName);
		NTexture tex;
		//何もテクスチャないなら真っ白で描画
		if (texName == "")
		{
			tex = NTextureManager::GetInstance()->textureMap_["white"];
		}
		else
		{
			tex = NTextureManager::GetInstance()->textureMap_[texName];
		}

		//指定のヒープにあるSRVをルートパラメータ0番に設定
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, tex.gpuHandle_);

		//ライト描画
		sLightGroup->Draw(4);

		NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced((UINT)meshes_[i].indices.size(), 1, 0, 0, 0); // インデックスの数分描画する
	}
}
