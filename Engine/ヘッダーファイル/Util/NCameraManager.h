#pragma once
#include "NCamera.h"
#include "NEasing.h"

//�����ɃJ�����̎�ޓo�^
enum class CameraType
{
	Normal,	//�ʏ�(�v���C��)
	Debug,	//�f�o�b�O�p
	Title,	//�^�C�g���p
	Faild,	//���s���U���g�p
	Clear,	//�N���A���U���g�p
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
	NEasing::EaseTimer cameraRotEase_;			//�J��������]������p�̃^�C�}�[
	//���s���U���g���̃J�����֘A
	NCamera faildCamera_;						//���s���U���g�̃J����
	NEasing::EaseTimer faildCameraMoveEase_;	//���s���U���g�J�����Ɏ����Ă����߂̃C�[�W���O
	//�N���A���U���g���̃J�����֘A
	NCamera clearCamera_;						//�N���A���U���g�̃J����
	NEasing::EaseTimer clearCameraMoveEase_;	//�N���A���U���g�J�����Ɏ����Ă����߂̃C�[�W���O

	// �p������֘A
	NVec3 frontVec_;
	NVec3 upVec_;
	NVec3 rightVec_;

	bool isChange_;				//�J�����̎�ސ؂�ւ��t���O
	bool isActive_;

	//���ʂ̃J�����ɕK�v�ȏ��
	NVec3 currentPos_;		//���݂̃J�������W
	NVec3 nextPos_;			//�����Ă��������J�������W
	NVec3 currentTarget_;	//���݂̃J�����̒����_���W
	NVec3 nextTarget_;		//�����Ă��������J�����̒����_���W
	NVec3 currentUpVec_;		//���݂̃J�����̒����_���W
	NVec3 nextUpVec_;		//�����Ă��������J�����̒����_���W
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
	// ���s���U���g��
	void FaildCameraInit();
	void FaildCameraUpdate();
	// �N���A���U���g��
	void ClearCameraInit();
	void ClearCameraUpdate();

	//Vec3�̃C�[�W���O�p
	NVec3 InQuad(const NVec3& start,const NVec3& end,float timerate);
	NVec3 OutQuad(const NVec3& start,const NVec3& end,float timerate);

public:
	//�V���O���g���C���X�^���X�擾
	static NCameraManager* GetInstance();

	void Init();
	void Update();

	void ChangeCameara(const CameraType cameraType);

	bool GetisActive() { return isActive_; }

	void SetDebugCamera(NCamera& camera);

private:
	NCameraManager() = default;
	NCameraManager(const NCameraManager&) = delete;
	~NCameraManager() = default;
	NCameraManager& operator=(const NCameraManager&) = delete;
};