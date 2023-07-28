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
	NVector3 minVelo, NVector3 maxVelo, NVector3 accel, NVector3 minRot, NVector3 maxRot)
{
	IEmitter3D::Add(addNum, life, color, minScale, maxScale, minVelo, maxVelo, accel, minRot, maxRot);
}
