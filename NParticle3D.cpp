#include "NParticle3D.h"
#include "NMathUtil.h"

Emitter3D::Emitter3D()
{
	addInterval_ = 0;
	maxScale_ = 0;
	minScale_ = 0;
	scaling_ = 0;
}

void Emitter3D::Init()
{
}

void Emitter3D::Update(bool isGravity)
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
}

void Emitter3D::Draw()
{
	//ルートパラメータ1番にマテリアルの定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, cbMaterial_->constBuff_->GetGPUVirtualAddress());
	//ルートパラメータ2番に色情報の定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	//ルートパラメータ3番に3D変換行列の定数バッファを渡す
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, cbTrans_->constBuff_->GetGPUVirtualAddress());
}

void Emitter3D::Add(uint32_t addNum, uint32_t life, float minScale, float maxScale, NVector3 minVelo, NVector3 maxVelo, NVector3 accel, float minRot, float maxRot, NColor color)
{
	for (uint32_t i = 0; i < addNum; i++)
	{
		//指定した最大数超えてたら生成しない
		if (particles_.size() >= maxCount_)
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

void Emitter3D::SetScale(NVector3& scale)
{
	scale_ = scale;
	originalScale_ = scale_;			//拡縮用に元のサイズを保管
}

void Emitter3D::SetScalingTimer(float timer)
{
	scalingTimer_.maxTime_ = timer;
}

void Emitter3D::StartScalingTimer(bool isRun)
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
