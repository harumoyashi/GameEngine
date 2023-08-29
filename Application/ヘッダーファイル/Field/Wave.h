#pragma once
#include "NObj3d.h"
#include "NSprite.h"
#include "NEasing.h"
#include "NumDrower.h"

class Wave final
{
private:
	std::vector<std::unique_ptr<NObj3d>> obj_;	//�I�u�W�F�N�g�Q
	std::vector<float> randomPosZ_;				//�e�g�̂��ɂ傤�ɂ�
	const uint32_t waveDivide_ = 200;			//�g�̕�����

	float scaleZ_;								//�g�̑傫��(z����)
	float posZ_;								//�g�S�̂̋��ʍ��W(z���W)
	float moveSpeed_;							//�ړ����x

	std::vector<NEasing::EaseTimer> waveTimer_;	//�g�̂��˂�p�^�C�}�[

	std::unique_ptr<NSprite> waveUI_;			//�g�������Ă邩�m�点��UI
	std::unique_ptr<NSprite> meterUI_;			//�g���ǂ̂��炢�����Ă邩�m�点��UI
	uint32_t player2WaveLen_;					//�v���C���[�Ɣg�̋���
	NumDrower meterTex_;						//�v���C���[�Ɣg�̋����\��

public:
	static Wave* GetInstance();

	void Init();
	void Update();
	void DrawObj();
	void DrawSprite();

	// �Q�b�^�[ //
	//�g�̑O�����W�擾
	float GetFrontPosZ() const { return posZ_ + scaleZ_; }

	// �Z�b�^�[ //

private:
	Wave() = default;
	Wave(const Wave&) = delete;
	~Wave() = default;
	Wave& operator=(const Wave&) = delete;
};