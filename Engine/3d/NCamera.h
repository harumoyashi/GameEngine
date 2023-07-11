#pragma once
#include <DirectXMath.h>
#include <memory>

#include "NVector2.h"
#include "NVector3.h"
#include "NMatrix4.h"

class NCamera
{
public:
	static std::unique_ptr<NCamera> sNCamera;
	static NCamera* sCurrentCamera;

private:
	//�ϊ��p�s��
	NMatrix4 matProjection_{};
	NMatrix4 matView_{};

	//�J����
	NVector3 eye_ = { 0, 10.0f, -30.0f };	//���_���W
	NVector3 target_ = { 0, 0, 0 };			//�����_���W
	NVector3 up_ = { 0, 1.0f, 0 };			//������x�N�g��
	NVector3 rot_ = { 0,0,0 };				//�J�����̉�]�p
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
	inline void SetEye(const NVector3& eye) { eye_ = eye; }
	inline void SetTarget(const NVector3& target) { target_ = target; }
	inline void SetUpVec(const NVector3& up) { up_ = up; }
	inline void SetRot(const NVector3& rot) { rot_ = rot; }
	inline void SetMatView(const NMatrix4& mat) { matView_ = mat; }
	void SetNearFar(const float nearZ, const float farZ);
	void SetFov(const float fov) { fov_ = fov; }

	//�Q�b�^�[
	inline const NMatrix4& GetMatProjection()const { return matProjection_; }
	inline const NMatrix4& GetMatView()const { return matView_; }
	//eye���W�擾
	inline const NVector3& GetPos()const { return eye_; }
	//�����_���W�擾
	inline const NVector3& GetTarget()const { return target_; }
	//��x�N�g���擾
	inline const NVector3& GetUpVec()const { return up_; }

	inline const float GetFov()const { return fov_; }
};