#pragma once
#include "SimpleParticle.h"
#include <unordered_map>

class IEmitter3D;

class NParticleManager final
{
public:
	std::unordered_map<std::string,IEmitter3D*> emitters_;	//�p�[�e�B�N���G�~�b�^�[�Q

public:
	static NParticleManager *GetInstance();

	//�G�~�b�^�[��unordered_map�ɒǉ�
	inline void AddEmitter(IEmitter3D* emitter, std::string key)
	{
		emitter->Init();					//���������Ă���o�^
		emitters_.emplace(std::make_pair(key,emitter));
	}
	//�G�~�b�^�[��unordered_map����폜
	inline void EraseEmitter(std::string key)
	{
		emitters_[key]->ClearParticles();	//�c���Ă�p�[�e�B�N���S�������Ă���폜
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