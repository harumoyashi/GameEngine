#include "NParticle3D.h"
#include "NGPipeLine.h"
#include "NCamera.h"
#include "NMathUtil.h"

#include "Player.h"

NLightGroup* IEmitter3D::sLightGroup = nullptr;

IEmitter3D::IEmitter3D()
{
	Init();
}

void IEmitter3D::Init()
{
	//定数バッファ
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform>>();
	cbTrans_->Init();

	pos_ = NVec3::zero;
	rot_ = NVec3::zero;
	scale_ = { 0.1f,0.1f,0.1f };

	addInterval_ = 0;
	maxScale_ = 0;
	minScale_ = 0;
	scaling_ = 0;
	texture_ = NTextureManager::GetInstance()->textureMap_["white"];	//とりま真っ白なテクスチャ割り当てとく

	//更新処理でサイズが変わっちゃうから、あらかじめ最大数分作る
	vertices_.resize(maxParticle_);
	//それによってバッファの初期化をする
	vertexBuff_.Init(vertices_);

	isActive_ = true;	//生成時には有効フラグ立てる
}

void IEmitter3D::Update()
{
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	//寿命が尽きたパーティクルを全削除
	for (uint32_t i = 0; i < particles_.size(); i++)
	{
		if (particles_[i].aliveTimer.GetEnd())
		{
			particles_.erase(particles_.begin() + i);
			i--;
		}
	}

	//全パーティクル更新
	for (auto& particle : particles_)
	{
		//生存時間とイージング用タイマーの更新
		particle.aliveTimer.Update(elapseSpeed_);
		particle.easeTimer.Update(elapseSpeed_);

		//スケールの線形補間
		particle.scale = NEasing::lerp(particle.startScale, particle.endScale, particle.easeTimer.GetTimeRate());

		//加速度を速度に加算
		particle.velo += particle.accel;

		//初期のランダム角度をもとに回す
		if (isRotation_)
		{
			particle.rot += particle.plusRot * elapseSpeed_;

			//一回転したら0に戻してあげる
			if (abs(particle.rot.x) >= PI2)
			{
				particle.rot.x = 0.0f;
			}

			if (abs(particle.rot.y) >= PI2)
			{
				particle.rot.y = 0.0f;
			}

			if (abs(particle.rot.z) >= PI2)
			{
				particle.rot.z = 0.0f;
			}
		}

		//重力加算
		if (isGravity_)
		{
			particle.velo.y += particle.gravity * elapseSpeed_;
		}

		//速度による移動
		particle.pos += particle.velo * elapseSpeed_;
	}

	//頂点バッファへデータ転送
	//パーティクルの情報を1つずつ反映
	for (size_t i = 0; i < particles_.size(); i++)
	{
		NVertexParticle vertex;

		//座標
		vertex.pos = particles_[i].pos;
		//回転
		vertex.rot = particles_[i].rot;
		//色
		vertex.color = particles_[i].color;
		//スケール
		vertex.scale = particles_[i].scale;

		vertices_.at(i) = vertex;
	}

	//毎回頂点数が変わるので初期化しなおす
	vertexBuff_.TransferBuffer(vertices_);

	UpdateMatrix();
}

void IEmitter3D::CommonBeginDraw()
{
	// プリミティブ形状の設定コマンド
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 点

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void IEmitter3D::SetBlendMode(BlendMode blendMode)
{
	// パイプラインステートとルートシグネチャの設定コマンド
	switch (blendMode)
	{
	case BlendMode::None:
		NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("Particle3dNone"));
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("Particle3dNone")->pRootSignature);
		break;
	case BlendMode::Alpha:
		NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("Particle3dAlpha"));
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("Particle3dAlpha")->pRootSignature);
		break;
	case BlendMode::Add:
		NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("Particle3dAdd"));
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("Particle3dAdd")->pRootSignature);
		break;
	default:
		break;
	}
}

void IEmitter3D::Draw()
{
	//ルートパラメータ2番に3D変換行列の定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTrans_->constBuff_->GetGPUVirtualAddress());

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuff_.GetView());

	//SRVの設定
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, texture_.gpuHandle_);

	//ライトの描画
	sLightGroup->Draw(3);

	// 描画コマンド
	NDX12::GetInstance()->GetCommandList()->DrawInstanced((UINT)std::distance(particles_.begin(), particles_.end()), 1, 0, 0);
}

void IEmitter3D::UpdateMatrix()
{
	//ワールド行列
	NMatrix4 matScale;	//スケーリング行列
	matScale = matScale.Scale(scale_);

	NMatrix4 matRot;		//回転行列
	NMatrix4 matZ = matZ.RotateZ(MathUtil::Degree2Radian(rot_.z));
	NMatrix4 matX = matX.RotateX(MathUtil::Degree2Radian(rot_.x));
	NMatrix4 matY = matY.RotateY(MathUtil::Degree2Radian(rot_.y));
	matRot *= matZ;	//Z軸周りに回転してから
	matRot *= matX;	//X軸周りに回転して
	matRot *= matY;	//Y軸周りに回転

	NMatrix4 matTrans;	//平行移動行列
	matTrans = matTrans.Translation(pos_);

	matWorld_ = NMatrix4::Identity();	//単位行列代入
	matWorld_ *= matScale;	//ワールド座標にスケーリングを反映
	matWorld_ *= matRot;		//ワールド座標に回転を反映
	matWorld_ *= matTrans;	//ワールド座標に平行移動を反映

	// 定数バッファへデータ転送
	TransferMatrix();
}

void IEmitter3D::TransferMatrix()
{
	// 定数バッファへデータ転送
	cbTrans_->constMap_ = nullptr;
	cbTrans_->constBuff_->Map(0, nullptr, (void**)&cbTrans_->constMap_);

	cbTrans_->constMap_->viewproj = NCamera::sCurrentCamera->GetMatView() * NCamera::sCurrentCamera->GetMatProjection();
	cbTrans_->constMap_->world = matWorld_;
	cbTrans_->constMap_->cameraPos = NCamera::sCurrentCamera->GetEye();

	cbTrans_->Unmap();
}

void IEmitter3D::Add(uint32_t addNum, float life, NColor color, float minScale, float maxScale,
	NVec3 minVelo, NVec3 maxVelo, NVec3 accel, NVec3 minRot, NVec3 maxRot)
{
	for (uint32_t i = 0; i < addNum; i++)
	{
		//指定した最大数超えてたら生成しない
		if (particles_.size() >= maxParticle_)
		{
			return;
		}

		//リストに要素を追加
		particles_.emplace_back();
		//追加した要素の参照
		Particle3D& p = particles_.back();
		//エミッターの中からランダムで座標を決定
		float pX = MathUtil::Randomf(-scale_.x, scale_.x);
		float pY = MathUtil::Randomf(-scale_.y, scale_.y);
		float pZ = MathUtil::Randomf(-scale_.z, scale_.z);
		NVec3 randomPos(pX, pY, pZ);
		//引数の範囲から大きさランダムで決定
		float sX = MathUtil::Randomf(minScale, maxScale);
		float sY = MathUtil::Randomf(minScale, maxScale);
		float sZ = MathUtil::Randomf(minScale, maxScale);
		NVec3 randomScale(sX, sY, sZ);
		//引数の範囲から飛ばす方向ランダムで決定
		float vX = MathUtil::Randomf(minVelo.x, maxVelo.x);
		float vY = MathUtil::Randomf(minVelo.y, maxVelo.y);
		float vZ = MathUtil::Randomf(minVelo.z, maxVelo.z);
		NVec3 randomVelo(vX, vY, vZ);
		//引数の範囲から回転をランダムで決定
		float rX = MathUtil::Randomf(minRot.x, maxRot.x);
		float rY = MathUtil::Randomf(minRot.y, maxRot.y);
		float rZ = MathUtil::Randomf(minRot.z, maxRot.z);
		NVec3 randomRot(rX, rY, rZ);

		//決まった座標にエミッター自体の座標を足して正しい位置に
		p.pos = randomPos + pos_;
		//飛んでく方向に合わせて回転
		p.rot = randomRot;
		p.plusRot = p.rot;
		p.velo = randomVelo;
		p.accel = accel;
		p.aliveTimer = life;
		p.aliveTimer.Start();
		p.scale = sX;
		p.startScale = p.scale;
		p.endScale = 0.0f;
		p.color = color;
		//イージング用のタイマーを設定、開始
		p.easeTimer.maxTime_ = life;
		p.easeTimer.Start();
	}
}

void IEmitter3D::SetScale(const NVec3& scale)
{
	scale_ = scale;
	originalScale_ = scale_;			//拡縮用に元のサイズを保管
}

void IEmitter3D::SetScalingTimer(float easeTimer)
{
	scalingTimer_.maxTime_ = easeTimer;
}

void IEmitter3D::StartScalingTimer(bool isRun)
{
	if (isRun)
	{
		scalingTimer_.Start();	//設定と同時にタイマーもスタート
	}
	else
	{
		scalingTimer_.ReverseStart();	//設定と同時にタイマーもスタート
	}
}