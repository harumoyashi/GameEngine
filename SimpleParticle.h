#pragma once
#include "NParticle3D.h"

class SimpleParticle
	:public IEmitter3D
{
public:
	//������
	void Init() override;
	//�X�V����
	void Update() override;
	//�p�[�e�B�N���̒ǉ�
	void Add(uint32_t addNum, uint32_t life, NColor color, float minScale, float maxScale,
		NVector3 minVelo, NVector3 maxVelo,NVector3 accel = {}, NVector3 minRot = {}, NVector3 maxRot = {}) override;
};