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
	void Init();
	void Update();
	void Draw();

	//エミッターをunordered_mapに追加
	inline void AddEmitter(IEmitter3D* emitter, const std::string& key)
	{
		if (emitters_.max_size() > emitters_.size())
		{
			emitter->Init();					//初期化してから登録
			emitters_.emplace(std::make_pair(key, emitter));
		}
		else
		{
			emitter->Init();
		}
	}
	//エミッターをunordered_mapから削除
	inline void EraseEmitter(const std::string& key)
	{
		emitters_.erase(key);
	}
	//有効フラグを設定
	inline void SetIsActive(const std::string& key,bool isActive)
	{
		emitters_[key]->SetIsActive(isActive);
	}

private:
	NParticleManager() = default;
	NParticleManager(const NParticleManager&) = delete;
	~NParticleManager() = default;
	NParticleManager& operator=(const NParticleManager&) = delete;
};