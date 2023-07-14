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
		emitter->ClearParticles();
		emitter->Init();
	}
}

void NParticleManager::Update()
{
	for (auto& emitter : emitters_)
	{
		emitter->Update();
	}
}

void NParticleManager::Draw()
{
	for (auto& emitter : emitters_)
	{
		emitter->Draw();
	}
}