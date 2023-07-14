#pragma once
#include "SimpleParticle.h"
#include <forward_list>

class IEmitter3D;

class NParticleManager
{
private:
	std::forward_list<IEmitter3D*> emitters_;	//パーティクルエミッター群

public:
	static NParticleManager *GetInstance();

	//コライダーをunordered_mapに追加
	inline void AddEmitter(IEmitter3D* emitter)
	{
		emitters_.emplace_front(emitter);
		emitters_.front()->Init();
	}
	//コライダーをunordered_mapから削除
	inline void RemoveEmitter(IEmitter3D* emitter)
	{
		emitters_.remove(emitter);
	}

	void Init();
	void Update();
	void Draw();

private:
	NParticleManager() = default;
	NParticleManager(const NParticleManager&) = delete;
	~NParticleManager() = default;
	NParticleManager& operator=(const NParticleManager&) = delete;
};