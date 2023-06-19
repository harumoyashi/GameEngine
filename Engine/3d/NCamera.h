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

	//�f�o�b�O�J�������ۂ�
	bool isDebugCamera_ = false;
	float distance = 20.0f;

public:
	void Reset();
	void Update();
	//�f�o�b�O�J�����p�̈ړ�����
	void DebugCameraUpdate();

	//�r���[�ϊ��s��쐬
	void CreateMatView();
	//�ˉe���e�ϊ�//
	void ProjectiveProjection();

	//�f�o�b�O�J�������[�h�؂�ւ�
	void ChangeIsDebugCamera();

	//�Z�b�^�[
	inline void SetEye(const NVector3& eye) { eye_ = eye; }
	inline void SetTarget(const NVector3& target) { target_ = target; }
	inline void SetUp(const NVector3& up) { up_ = up; }
	void SetNearFar(const float nearZ, const float farZ);

	//�Q�b�^�[
	inline const NMatrix4& GetMatProjection()const { return matProjection_; }
	inline const NMatrix4& GetMatView()const { return matView_; }
	inline const bool GetIsCamera()const { return isDebugCamera_; }
	//eye���W�擾
	inline const NVector3& GetPos()const { return eye_; }
};