#include "SimpleParticle.h"

void SimpleParticle::Update()
{
	IEmitter3D::Update();
}

void SimpleParticle::Add(uint32_t addNum, uint32_t life, NColor color, NVector3 minScale, NVector3 maxScale,
	NVector3 minVelo, NVector3 maxVelo, NVector3 accel, NVector3 minRot, NVector3 maxRot)
{
	IEmitter3D::Add(addNum, life, color, minScale, maxScale, minVelo, maxVelo, accel, minRot, maxRot);
}
