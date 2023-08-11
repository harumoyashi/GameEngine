#pragma once
#include "NParticle3D.h"

class SimpleParticle final
	:public IEmitter3D
{
public:
	//������
	void Init() override;
	//�X�V����
	void Update() override;
	//�p�[�e�B�N���̒ǉ�
	void Add(uint32_t addNum, float life, NColor color, float minScale, float maxScale,
		NVec3 minVelo, NVec3 maxVelo,NVec3 accel = {}, NVec3 minRot = {}, NVec3 maxRot = {}) override;
};