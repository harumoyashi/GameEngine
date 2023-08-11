#pragma once
#include <DirectXMath.h>
#include <memory>

#include "NVec2.h"
#include "NVec3.h"
#include "NMatrix4.h"

class NCamera final
{
public:
	static std::unique_ptr<NCamera> sNCamera;
	static NCamera* sCurrentCamera;

private:
	//�ϊ��p�s��
	NMatrix4 matProjection_{};
	NMatrix4 matView_{};

	//�J����
	NVec3 eye_ = { 0, 10.0f, -30.0f };	//���_���W
	NVec3 target_ = { 0, 0, 0 };			//�����_���W
	NVec3 up_ = { 0, 1.0f, 0 };			//������x�N�g��
	NVec3 rot_ = NVec3::zero;				//�J�����̉�]�p
	float nearZ_ = 0.1f;
	float farZ_ = 1000.0f;
	float fov_ = 45.0f;

private:
	//�r���[�ϊ��s��쐬
	void CreateMatView();
	//�ˉe���e�ϊ�//
	void ProjectiveProjection();

public:
	void Init();
	void Update();
	//�f�o�b�O�J�����p�̈ړ�����
	void DebugCameraUpdate();

	//�Z�b�^�[
	void SetEye(const NVec3& eye) { eye_ = eye; }
	void SetTarget(const NVec3& target) { target_ = target; }
	void SetUpVec(const NVec3& up) { up_ = up; }
	void SetRot(const NVec3& rot) { rot_ = rot; }
	void SetMatView(const NMatrix4& mat) { matView_ = mat; }
	void SetNearFar(const float nearZ, const float farZ);
	void SetFov(const float fov) { fov_ = fov; }

	//�Q�b�^�[
	const NMatrix4& GetMatProjection()const { return matProjection_; }
	const NMatrix4& GetMatView()const { return matView_; }
	//eye���W�擾
	const NVec3& GetPos()const { return eye_; }
	//�����_���W�擾
	const NVec3& GetTarget()const { return target_; }
	//��x�N�g���擾
	const NVec3& GetUpVec()const { return up_; }

	const float GetFov()const { return fov_; }
};