#include "NParticle3D.h"
#include "NGPipeLine.h"
#include "NMathUtil.h"

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

	vertices_.resize(maxParticle_);
	vertexBuff_.Init(vertices_);
}

void IEmitter3D::Init()
{
}

void IEmitter3D::Update(bool isGravity)
{
	//寿命が尽きたパーティクルを全削除
	for (size_t i = 0; i < particles_.size(); i++)
	{
		if (particles_[i].frame >= particles_[i].num_frame)
		{
			particles_.erase(particles_.begin() + i);
			i = (size_t)-1;
		}
	}

	//全パーティクル更新
	for (size_t i = 0; i < particles_.size(); i++)
	{
		particles_[i].timer.Update();

		//経過フレームをカウント
		particles_[i].frame++;

		//スケールの線形補間
		particles_[i].scale.x = NEasing::lerp(particles_[i].startScale.x, particles_[i].endScale.x, particles_[i].timer.GetTimeRate());
		particles_[i].scale.y = NEasing::lerp(particles_[i].startScale.y, particles_[i].endScale.y, particles_[i].timer.GetTimeRate());

		//加速度を速度に加算
		particles_[i].velo += particles_[i].accel;

		//重力加算
		if (isGravity)
		{
			particles_[i].velo.y += particles_[i].gravity;
		}

		//速度による移動
		particles_[i].pos += particles_[i].velo;
	}

	//頂点バッファへデータ転送
	//パーティクルの情報を1つずつ反映
	for (size_t i = 0; i < particles_.size(); i++)
	{
		NVertexParticle vertex;

		//座標
		vertex.pos = particles_[i].pos;
		//スケール
		vertex.scale = particles_[i].scale;
		//色
		vertex.color = particles_[i].color;

		vertices_.at(i) = vertex;
	}

	//毎回頂点数が変わるので初期化しなおす
	vertexBuff_.TransferBuffer(vertices_);
}

void IEmitter3D::CommonBeginDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("Particle"));
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("Particle")->pRootSignature);

	// プリミティブ形状の設定コマンド
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 点

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void IEmitter3D::Draw()
{
	//ルートパラメータ1番に3D変換行列の定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, cbTrans_->constBuff_->GetGPUVirtualAddress());

	//SRVの設定
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, texture_.gpuHandle_);

	// 描画コマンド
	NDX12::GetInstance()->GetCommandList()->DrawInstanced((UINT)std::distance(particles_.begin(), particles_.end()),1,0,0);
}

void IEmitter3D::Add(uint32_t addNum, uint32_t life, float minScale, float maxScale, NVector3 minVelo, NVector3 maxVelo, NVector3 accel, float minRot, float maxRot, NColor color)
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
		float x = (float)MathUtil::Random((uint32_t)-scale_.x, (uint32_t)scale_.x);
		float y = (float)MathUtil::Random((uint32_t)-scale_.y, (uint32_t)scale_.y);
		float z = (float)MathUtil::Random((uint32_t)-scale_.z, (uint32_t)scale_.z);
		NVector3 randomPos(x, y, z);
		//引数の範囲から大きさランダムで決定
		float scale_ = (float)MathUtil::Random((uint32_t)minScale, (uint32_t)maxScale);
		NVector3 randomScale(scale_, scale_, scale_);
		//引数の範囲から飛ばす方向ランダムで決定
		NVector3 velo = {
			(float)MathUtil::Random((uint32_t)minVelo.x,(uint32_t)maxVelo.x),
			(float)MathUtil::Random((uint32_t)minVelo.y,(uint32_t)maxVelo.y),
			(float)MathUtil::Random((uint32_t)minVelo.z,(uint32_t)maxVelo.z) };
		//引数の範囲から回転をランダムで決定
		float rot = (float)MathUtil::Random((uint32_t)minRot, (uint32_t)maxRot);

		//決まった座標にエミッター自体の座標を足して正しい位置に
		p.pos = randomPos + pos_;
		//飛んでく方向に合わせて回転
		p.rot = rot;
		p.velo = velo;
		p.accel = accel;
		p.num_frame = life;
		p.scale = randomScale;
		p.startScale = randomScale;
		p.endScale = { 0,0,0 };
		p.color = color;
		//イージング用のタイマーを設定、開始
		p.timer.maxTime_ = (float)life / 60.0f;
		p.timer.Start();
	}
}

void IEmitter3D::SetScale(NVector3& scale)
{
	scale_ = scale;
	originalScale_ = scale_;			//拡縮用に元のサイズを保管
}

void IEmitter3D::SetScalingTimer(float timer)
{
	scalingTimer_.maxTime_ = timer;
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
