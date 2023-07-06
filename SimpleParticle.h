#pragma once
#include "NParticle3D.h"

class SimpleParticle
	:public IEmitter3D
{
public:
	//初期化
	void Init() override;
	//更新処理
	void Update() override;
	//パーティクルの追加
	void Add(uint32_t addNum, uint32_t life, NColor color, float minScale, float maxScale,
		NVector3 minVelo, NVector3 maxVelo,NVector3 accel = {}, NVector3 minRot = {}, NVector3 maxRot = {}) override;
};