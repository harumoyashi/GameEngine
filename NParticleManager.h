#pragma once
#include "SimpleParticle.h"
#include <unordered_map>

class IEmitter3D;

class NParticleManager final
{
public:
	std::unordered_map<std::string,IEmitter3D*> emitters_;	//パーティクルエミッター群

public:
	static NParticleManager *GetInstance();

	//エミッターをunordered_mapに追加
	inline void AddEmitter(IEmitter3D* emitter, std::string key)
	{
		emitter->Init();					//初期化してから登録
		emitters_.emplace(std::make_pair(key,emitter));
	}
	//エミッターをunordered_mapから削除
	inline void EraseEmitter(std::string key)
	{
		emitters_[key]->ClearParticles();	//残ってるパーティクル全部消してから削除
		emitters_.erase(key);
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