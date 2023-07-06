#pragma once
#include "NParticle3D.h"

class SimpleParticle
	:public IEmitter3D
{
public:
	//更新処理
	void Update() override;
	//パーティクルの追加
	void Add(uint32_t addNum, uint32_t life, NColor color, NVector3 minScale, NVector3 maxScale,
		NVector3 minVelo, NVector3 maxVelo,NVector3 accel = { 0,0,0 }, NVector3 minRot = {}, NVector3 maxRot = {}) override;
};