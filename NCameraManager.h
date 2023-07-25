#pragma once
#include "NCamera.h"
#include "NEasing.h"

//�����ɃJ�����̎�ޓo�^
enum class CameraType
{
	Normal,
	Debug,
	Title,
	Result,
};

class NCameraManager final
{
private:
	uint32_t nowCameraType_;	//���݂̃J�����̎��
	uint32_t prevCameraType_;	//�f�o�b�O�J�����ɐ؂�ւ���O�̃J�����̎��

	NCamera nowCamera_;			//���̃J�������i�[�p

	//�ʏ펞�̃J�����֘A
	NCamera normalCamera_;						//�ʏ�̃J����
	NEasing::EaseTimer normalCameraMoveEase_;	//�ʏ�J�����Ɏ����Ă����߂̃C�[�W���O
	//�ʏ펞�̃J�����֘A
	NCamera debugCamera_;						//�f�o�b�O�p�J����
	//�^�C�g�����̃J�����֘A
	NCamera titleCamera_;						//�^�C�g���̃J����
	NEasing::EaseTimer titleCameraMoveEase_;	//�^�C�g���J�����Ɏ����Ă����߂̃C�[�W���O
	NEasing::EaseTimer cameraRotEase_ = 4.0f;	//�J��������]������p�̃^�C�}�[
	//���U���g���̃J�����֘A
	NCamera resultCamera_;						//���U���g�̃J����
	NEasing::EaseTimer resultCameraMoveEase_;	//���U���g�J�����Ɏ����Ă����߂̃C�[�W���O

	// �p������֘A
	NVector3 frontVec_;
	NVector3 upVec_;
	NVector3 rightVec_;

	bool isChange_;				//�J�����̎�ސ؂�ւ��t���O
	bool isActive_;

	//���ʂ̃J�����ɕK�v�ȏ��
	NVector3 currentPos_;		//���݂̃J�������W
	NVector3 nextPos_;			//�����Ă��������J�������W
	NVector3 currentTarget_;	//���݂̃J�����̒����_���W
	NVector3 nextTarget_;		//�����Ă��������J�����̒����_���W
	NVector3 currentUpVec_;		//���݂̃J�����̒����_���W
	NVector3 nextUpVec_;		//�����Ă��������J�����̒����_���W
	float currentFov_;			//���݂̃J��������
	float nextFov_;				//�����Ă��������J��������
	float length_;				//������̂Ƃ̋���

private:
	//�J�����ŗL�̏������A�X�V����
	// �ʏ펞
	void NormalCameraInit();
	void NormalCameraUpdate();
	// �f�o�b�O�J������
	void DebugCameraUpdate();
	// �^�C�g����
	void TitleCameraInit();
	void TitleCameraUpdate();
	// ���U���g��
	void ResultCameraInit();
	void ResultCameraUpdate();

public:
	//�V���O���g���C���X�^���X�擾
	static NCameraManager* GetInstance();

	void Init();
	void Update();

	void ChangeCameara(const CameraType cameraType);

	bool GetisActive() { return isActive_; }

	void SetDebugCamera(NCamera& camera);
};