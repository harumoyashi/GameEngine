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

	for (auto& eneEmitter : enemyEmitters_)
	{
		eneEmitter->ClearParticles();
	}
	enemyEmitters_.clear();
}

void NParticleManager::Update()
{
	for (auto& emitter : emitters_)
	{
		emitter.second->Update();
	}

	for (auto& eneEmitter : enemyEmitters_)
	{
		eneEmitter->Update();
	}
}

void NParticleManager::Draw()
{
	for (auto& emitter : emitters_)
	{
		emitter.second->Draw();
	}

	for (auto& eneEmitter : enemyEmitters_)
	{
		eneEmitter->Draw();
	}
}
