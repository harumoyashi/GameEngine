#pragma once
#include "SimpleParticle.h"
#include <unordered_map>

class IEmitter3D;

class NParticleManager final
{
public:
	std::unordered_map<std::string, IEmitter3D*> emitters_;	//�p�[�e�B�N���G�~�b�^�[�Q
	std::vector<IEmitter3D*> enemyEmitters_;				//�G�p�p�[�e�B�N���G�~�b�^�[�Q

public:
	static NParticleManager* GetInstance();
	void Init();
	void Update();
	void Draw();

	//�G�~�b�^�[��unordered_map�ɒǉ�
	void AddEmitter(IEmitter3D* emitter, const std::string& key)
	{
		emitter->Init();					//���������Ă���o�^
		emitters_.emplace(std::make_pair(key, emitter));
	}
	//�G�~�b�^�[��unordered_map����폜
	void EraseEmitter(const std::string& key)
	{
		emitters_.erase(key);
	}
	//�L���t���O��ݒ�
	void SetIsActive(const std::string& key, bool isActive)
	{
		emitters_[key]->SetIsActive(isActive);
	}

private:
	NParticleManager() = default;
	NParticleManager(const NParticleManager&) = delete;
	~NParticleManager() = default;
	NParticleManager& operator=(const NParticleManager&) = delete;
};