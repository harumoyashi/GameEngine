#pragma once
#include "NObj3d.h"
#include "NEasing.h"

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

public:
	static Wave* GetInstance();

	void Init();
	void Update();
	void Draw();

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