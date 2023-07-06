#include "NParticleManager.h"

NParticleManager* NParticleManager::GetInstance()
{
	static NParticleManager instance;
	return &instance;
}

void NParticleManager::Init()
{
	playerDeadParticle_.ClearParticles();
}

void NParticleManager::Update()
{
	playerDeadParticle_.Update();
}

void NParticleManager::Draw()
{
	playerDeadParticle_.Draw();
}

void NParticleManager::PlayerDeadEffect(const NVector3& pos, const NColor& color)
{
	playerDeadParticle_.SetPos(pos.x,pos.y,pos.z);
	playerDeadParticle_.Add(50, 200, color,1.0f, 5.0f, { -1,-1,-1 }, { 1,1,1 });
}
