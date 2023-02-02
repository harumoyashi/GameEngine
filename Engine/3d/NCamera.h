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
	static std::unique_ptr<NCamera> _nCamera;
	static NCamera* nowCamera;

private:
	//�J����
	NMatrix4 matProjection{};
	NMatrix4 matView{};

	//�J����
	NVector3 eye = { 0, 10.0f, -30.0f };	//���_���W
	NVector3 target = { 0, 0, 0 };	//�����_���W
	NVector3 up = { 0, 1.0f, 0 };		//������x�N�g��
	float angle = 0.0f;				//�J�����̉�]�p

public:
	//�r���[�ϊ��s��쐬
	void CreateMatView();
	//�ˉe���e�ϊ�//
	void ProjectiveProjection();

	//�Z�b�^�[
	void SetEye(NVector3 eye) { this->eye = eye; }
	void SetTarget(NVector3 target) { this->target = target; }
	void SetUp(NVector3 up) { this->up = up; }

	//�Q�b�^�[
	NMatrix4 GetMatProjection() { return matProjection; }
	NMatrix4 GetMatView() { return matView; }
	//eye���W�擾
	NVector3 GetPos() { return eye; }
};