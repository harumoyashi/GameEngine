#include "NParticleManager.h"
#include "NParticle3D.h"

NParticleManager* NParticleManager::GetInstance()
{
	static NParticleManager instance;
	return &instance;
}

void NParticleManager::Init()
{
	for (auto& emitter : emitters_)
	{
		emitter.second->ClearParticles();
		emitter.second->Init();
	}
}

void NParticleManager::Update()
{
	for (auto& emitter : emitters_)
	{
		emitter.second->Update();
	}
}

void NParticleManager::Draw()
{
	for (auto& emitter : emitters_)
	{
		emitter.second->Draw();
	}
}