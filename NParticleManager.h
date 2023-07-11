#pragma once
#include "SimpleParticle.h"

class NParticleManager
{
private:
	SimpleParticle playerDeadParticle_;	//プレイヤーが死んだときのパーティクル

public:
	static NParticleManager *GetInstance();

	void Init();
	void Update();
	void Draw();

	void PlayerDeadEffect(const NVector3& pos, const NColor& color);
};