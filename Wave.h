#pragma once
#include "NObj3d.h"
#include "NEasing.h"

class Wave
{
private:
	std::vector<std::unique_ptr<NObj3d>> obj_;	//�I�u�W�F�N�g�Q
	const uint32_t waveDivide_ = 1;				//�g�̕�����

	float scaleZ_;								//�g�̑傫��(z����)
	float posZ_;								//�g�S�̂̋��ʍ��W(z���W)
	float moveSpeed_;							//�ړ����x

public:
	static Wave* GetInstance();

	void Init();
	void Update();
	void Draw();

	// �Q�b�^�[ //
	//�g�̑O�����W�擾
	inline float GetFrontPosZ() const { return posZ_ + scaleZ_; }

	// �Z�b�^�[ //

};