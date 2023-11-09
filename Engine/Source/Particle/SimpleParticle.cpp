#include "SimpleParticle.h"

void SimpleParticle::Init()
{
	IEmitter3D::Init();
	IEmitter3D::SetIsRotation(true);
}

void SimpleParticle::Update()
{
	IEmitter3D::Update();
}

void SimpleParticle::Add(uint32_t addNum, float life, NColor color, float minScale, float maxScale,
	NVec3 minVelo, NVec3 maxVelo, float accelPower, NVec3 minRot, NVec3 maxRot, float growingTimer)
{
	IEmitter3D::Add(addNum, life, color, minScale, maxScale, minVelo, maxVelo, accelPower, minRot, maxRot, growingTimer);
}
