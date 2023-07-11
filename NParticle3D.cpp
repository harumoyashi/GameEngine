#include "NParticle3D.h"
#include "NGPipeLine.h"
#include "NCamera.h"
#include "NMathUtil.h"

#include "Player.h"

IEmitter3D::IEmitter3D()
{
	//定数バッファ
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform>>();
	cbTrans_->Init();

	addInterval_ = 0;
	maxScale_ = 0;
	minScale_ = 0;
	scaling_ = 0;
	texture_ = NTextureManager::GetInstance()->textureMap_["white"];	//とりま真っ白なテクスチャ割り当てとく

	//更新処理でサイズが変わっちゃうから、あらかじめ最大数分作る
	vertices_.resize(maxParticle_);
	//それによってバッファの初期化をする
	vertexBuff_.Init(vertices_);
}

void IEmitter3D::Init()
{
}

void IEmitter3D::Update()
{
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	//寿命が尽きたパーティクルを全削除
	for (size_t i = 0; i < particles_.size(); i++)
	{
		if (particles_[i].aliveTimer.GetisTimeOut())
		{
			particles_.erase(particles_.begin() + i);
			i = (size_t)-1;
		}
	}

	//全パーティクル更新
	for (size_t i = 0; i < particles_.size(); i++)
	{
		//生存時間とイージング用タイマーの更新
		particles_[i].aliveTimer.Update(elapseSpeed_);
		particles_[i].easeTimer.Update(elapseSpeed_);

		//スケールの線形補間
		particles_[i].scale = NEasing::lerp(particles_[i].startScale, particles_[i].endScale, particles_[i].easeTimer.GetTimeRate());
		//particles_[i].scale.y = NEasing::lerp(particles_[i].startScale.y, particles_[i].endScale.y, particles_[i].easeTimer.GetTimeRate());

		//加速度を速度に加算
		particles_[i].velo += particles_[i].accel;

		//初期のランダム角度をもとに回す
		if (isRotation_)
		{
			particles_[i].rot += particles_[i].plusRot * elapseSpeed_;

			//一回転したら0に戻してあげる
			if (abs(particles_[i].rot.x) >= PI2)
			{
				particles_[i].rot.x = 0.0f;
			}

			if (abs(particles_[i].rot.y) >= PI2)
			{
				particles_[i].rot.y = 0.0f;
			}

			if (abs(particles_[i].rot.z) >= PI2)
			{
				particles_[i].rot.z = 0.0f;
			}
		}

		//重力加算
		if (isGravity_)
		{
			particles_[i].velo.y += particles_[i].gravity * elapseSpeed_;
		}

		//速度による移動
		particles_[i].pos += particles_[i].velo * elapseSpeed_;
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
	// パイプラインステートとルートシグネチャの設定コマンド
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("Particle3d"));
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("Particle3d")->pRootSignature);

	// プリミティブ形状の設定コマンド
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 点

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void IEmitter3D::Draw()
{
	//ルートパラメータ1番に3D変換行列の定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTrans_->constBuff_->GetGPUVirtualAddress());

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuff_.GetView());

	//SRVの設定
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, texture_.gpuHandle_);

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

	matWorld_ = matWorld_.Identity();	//単位行列代入
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
	cbTrans_->constMap_->cameraPos = NCamera::sCurrentCamera->GetPos();

	cbTrans_->Unmap();
}

void IEmitter3D::Add(uint32_t addNum, uint32_t life, NColor color, float minScale, float maxScale,
	NVector3 minVelo, NVector3 maxVelo, NVector3 accel, NVector3 minRot, NVector3 maxRot)
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
		NVector3 randomPos(pX, pY, pZ);
		//引数の範囲から大きさランダムで決定
		float sX = MathUtil::Randomf(minScale, maxScale);
		float sY = MathUtil::Randomf(minScale, maxScale);
		float sZ = MathUtil::Randomf(minScale, maxScale);
		NVector3 randomScale(sX, sY, sZ);
		//引数の範囲から飛ばす方向ランダムで決定
		float vX = MathUtil::Randomf(minVelo.x, maxVelo.x);
		float vY = MathUtil::Randomf(minVelo.y, maxVelo.y);
		float vZ = MathUtil::Randomf(minVelo.z, maxVelo.z);
		NVector3 randomVelo(vX, vY, vZ);
		//引数の範囲から回転をランダムで決定
		float rX = MathUtil::Randomf(minRot.x, maxRot.x);
		float rY = MathUtil::Randomf(minRot.y, maxRot.y);
		float rZ = MathUtil::Randomf(minRot.z, maxRot.z);
		NVector3 randomRot(rX, rY, rZ);

		//決まった座標にエミッター自体の座標を足して正しい位置に
		p.pos = randomPos + pos_;
		//飛んでく方向に合わせて回転
		p.rot = randomRot;
		p.plusRot = p.rot;
		p.velo = randomVelo;
		p.accel = accel;
		p.aliveTimer = (float)life;
		p.scale = sX;
		p.startScale = p.scale;
		p.endScale = 0.0f;
		p.color = color;
		//イージング用のタイマーを設定、開始
		p.easeTimer.maxTime_ = (float)life / 60.0f;
		p.easeTimer.Start();
	}
}

void IEmitter3D::SetScale(NVector3& scale)
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
