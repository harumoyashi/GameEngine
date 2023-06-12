#pragma once
#include <DirectXMath.h>
#include <memory>

#include "NVector2.h"
#include "NVector3.h"
#include "NMatrix4.h"

using namespace DirectX;

class NCamera
{
public:
	static std::unique_ptr<NCamera> sNCamera;
	static NCamera* sNowCamera;

private:
	//�J����
	NMatrix4 matProjection_{};
	NMatrix4 matView_{};

	//�J����
	NVector3 eye_ = { 0, 10.0f, -30.0f };	//���_���W
	NVector3 target_ = { 0, 0, 0 };			//�����_���W
	NVector3 up_ = { 0, 1.0f, 0 };			//������x�N�g��
	float angle_ = 0.0f;					//�J�����̉�]�p

public:
	//�r���[�ϊ��s��쐬
	void CreateMatView();
	//�ˉe���e�ϊ�//
	void ProjectiveProjection();

	//�Z�b�^�[
	inline void SetEye(const NVector3& eye) { eye_ = eye; }
	inline void SetTarget(const NVector3& target) { target_ = target; }
	inline void SetUp(const NVector3& up) { up_ = up; }

	//�Q�b�^�[
	inline NMatrix4 GetMatProjection() { return matProjection_; }
	inline NMatrix4 GetMatView() { return matView_; }
	//eye���W�擾
	inline NVector3 GetPos() { return eye_; }
};