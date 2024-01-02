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
		emitter.second->Init();			//ゲームシーンにパーティクル持たせたら情報保持できないときがあって初期化時にエラー起こった
		//プレイヤーに持たせておけば大丈夫そうだけどなんでなのかわからないから先生に聞く
	}

	for (auto& emitter : polygonEmitters_)
	{
		emitter.second->ClearParticles();
		emitter.second->Init();
	}

	for (auto& eneEmitter : enemyEmitters_)
	{
		eneEmitter->ClearParticles();
	}
	enemyEmitters_.clear();

	for (auto& bulEmitter : bulletEmitters_)
	{
		bulEmitter->ClearParticles();
	}
	bulletEmitters_.clear();
}

void NParticleManager::Update()
{
	for (auto& emitter : emitters_)
	{
		//パーティクルがあるときだけ更新処理回す
		if (emitter.second->GetParticlesDead() == false)
		{
			emitter.second->Update();
		}
	}

	for (auto& emitter : polygonEmitters_)
	{
		//パーティクルがあるときだけ更新処理回す
		if (emitter.second->GetParticlesDead() == false)
		{
			emitter.second->Update();
		}
	}

	for (auto& eneEmitter : enemyEmitters_)
	{
		eneEmitter->Update();
	}

	for (auto& bulEmitter : bulletEmitters_)
	{
		bulEmitter->Update();
	}
}

void NParticleManager::Draw()
{
	for (auto& emitter : emitters_)
	{
		//パーティクルがあるときだけ描画処理回す
		if (emitter.second->GetParticlesDead() == false)
		{
			emitter.second->Draw();
		}
	}

	//ポリゴンの場合はパイプライン変える
	IEmitter3D::SetBlendMode(BlendMode::Alpha, false);
	for (auto& emitter : polygonEmitters_)
	{
		emitter.second->Draw();
	}
	IEmitter3D::SetBlendMode(BlendMode::None, true);

	for (auto& eneEmitter : enemyEmitters_)
	{
		eneEmitter->Draw();
	}

	for (auto& bulEmitter : bulletEmitters_)
	{
		bulEmitter->Draw();
	}
}
