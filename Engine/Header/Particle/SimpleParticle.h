#pragma once
#include "NParticle3D.h"

// シンプルな3Dパーティクル //

class SimpleParticle final
	:public IEmitter3D
{
public:
	//初期化
	void Init() override;
	//更新処理
	void Update() override;
	//パーティクルの追加
	void Add(uint32_t addNum, float life, NColor color, float minScale, float maxScale,
		NVec3 minVelo, NVec3 maxVelo, float accelPower = 0.f, NVec3 minRot = {}, NVec3 maxRot = {}, float growingTimer = 0.f) override;
};