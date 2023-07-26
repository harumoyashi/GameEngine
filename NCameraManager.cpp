#include "NCameraManager.h"
#include "Player.h"

void NCameraManager::NormalCameraInit()
{
	length_ = 10.0f;

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = Player::GetInstance()->GetPos();

	currentPos_ = NCamera::sCurrentCamera->GetPos();
	nextPos_ = nextTarget_ - NVector3(0, length_, length_ * 0.5f);

	currentUpVec_ = NCamera::sCurrentCamera->GetUpVec();
	nextUpVec_ = NVector3(0, 0, 1);	//�������낷�`�ɂ���

	currentFov_ = NCamera::sCurrentCamera->GetFov();
	nextFov_ = 45.0f;
}

void NCameraManager::NormalCameraUpdate()
{
	if (isChange_ == false)
	{
		NormalCameraInit();
		isChange_ = true;
	}

	//�O�̃J�������珔�X�C�[�W���O���Ă��牺�̏�����肽��

	normalCamera_.SetTarget(Player::GetInstance()->GetPos());
	normalCamera_.SetEye(normalCamera_.GetTarget() + NVector3(0, length_, -length_ * 0.5f));
	normalCamera_.SetUpVec(nextUpVec_);

	normalCamera_.Update();
	NCamera::sCurrentCamera = &normalCamera_;
}

void NCameraManager::DebugCameraUpdate()
{
	if (isChange_ == false)
	{
		debugCamera_ = *NCamera::sCurrentCamera;
		isChange_ = true;
	}

	debugCamera_.DebugCameraUpdate();
	NCamera::sCurrentCamera = &debugCamera_;
}

void NCameraManager::TitleCameraInit()
{
	length_ = 1.0f;
	cameraRotEase_.Reset();

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = Player::GetInstance()->GetPos();

	currentPos_ = NCamera::sCurrentCamera->GetPos();
	nextPos_ = nextTarget_ - NVector3(length_, 0, -length_);

	currentUpVec_ = NCamera::sCurrentCamera->GetUpVec();
	nextUpVec_ = NVector3(0, 1, 0);

	currentFov_ = NCamera::sCurrentCamera->GetFov();
	nextFov_ = 45.0f;
}

void NCameraManager::TitleCameraUpdate()
{
	if (isChange_ == false)
	{
		TitleCameraInit();
		isChange_ = true;
	}

	//�O�̃J�������珔�X�C�[�W���O���Ă��牺�̏�����肽��
	//�J������]�p�̃^�C�}�[������������
	cameraRotEase_.Update();
	if (cameraRotEase_.GetStarted() == false)
	{
		cameraRotEase_.Start();
	}
	else if (cameraRotEase_.GetEnd())
	{
		cameraRotEase_.Reset();
	}

	NVector2 vec2;	//�J�����ɑ����x�N�g��
	//�~�^��������
	vec2 = MathUtil::CircleMotion(
		{ titleCamera_.GetTarget().x,titleCamera_.GetTarget().z },
		length_,
		cameraRotEase_.GetTimeRate() * PI2);

	titleCamera_.SetTarget(Player::GetInstance()->GetPos());
	titleCamera_.SetEye(NVector3(vec2.x, length_, vec2.y));
	titleCamera_.SetUpVec(nextUpVec_);

	titleCamera_.Update();
	NCamera::sCurrentCamera = &titleCamera_;
}

void NCameraManager::FaildCameraInit()
{
	length_ = 2.0f;
	faildCameraMoveEase_.Reset();

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = Player::GetInstance()->GetPos();

	currentPos_ = NCamera::sCurrentCamera->GetPos();
	nextPos_ = Player::GetInstance()->GetPos() - NVector3(0, -length_ * 0.5f, length_);

	currentUpVec_ = NCamera::sCurrentCamera->GetUpVec();
	nextUpVec_ = NVector3(0, 1, 0);

	currentFov_ = NCamera::sCurrentCamera->GetFov();
	nextFov_ = 45.0f;
}

void NCameraManager::FaildCameraUpdate()
{
	if (isChange_ == false)
	{
		FaildCameraInit();
		isChange_ = true;
	}

	//�C�[�W���O�p�̃^�C�}�[������
	faildCameraMoveEase_.Update();
	if (faildCameraMoveEase_.GetStarted() == false)
	{
		faildCameraMoveEase_.Start();
	}

	//�O�̃J�������猻�݂̃J�����܂ł̃C�[�W���O
	if (faildCameraMoveEase_.GetRun())
	{
		NVector3 target,pos,upVec;
		target = OutQuad(currentTarget_, nextTarget_, faildCameraMoveEase_.GetTimeRate());
		pos = OutQuad(currentPos_, nextPos_, faildCameraMoveEase_.GetTimeRate());
		upVec = OutQuad(currentUpVec_, nextUpVec_, faildCameraMoveEase_.GetTimeRate());

		faildCamera_.SetTarget(target);
		faildCamera_.SetEye(pos);
		faildCamera_.SetUpVec(upVec);
	}

	//�J�����������ƂȂ�����C�[�W���O�I���������u�ł�������
	if (faildCameraMoveEase_.GetEnd())
	{
		faildCamera_.SetTarget(nextTarget_);
		faildCamera_.SetEye(nextPos_);
		faildCamera_.SetUpVec(nextUpVec_);
	}

	faildCamera_.Update();
	NCamera::sCurrentCamera = &faildCamera_;
}

NVector3 NCameraManager::InQuad(const NVector3& start, const NVector3& end, float timerate)
{
	NVector3 result;
	result.x = NEasing::InQuad(start.x, end.x, timerate);
	result.y = NEasing::InQuad(start.y, end.y, timerate);
	result.z = NEasing::InQuad(start.z, end.z, timerate);
	return result;
}

NVector3 NCameraManager::OutQuad(const NVector3& start, const NVector3& end, float timerate)
{
	NVector3 result;
	result.x = NEasing::OutQuad(start.x, end.x, timerate);
	result.y = NEasing::OutQuad(start.y, end.y, timerate);
	result.z = NEasing::OutQuad(start.z, end.z, timerate);
	return result;
}

NCameraManager* NCameraManager::GetInstance()
{
	static NCameraManager instance;
	return &instance;
}

void NCameraManager::Init()
{
	NCamera::sCurrentCamera->Init();

	length_ = 10.0f;

	nowCameraType_ = (uint32_t)CameraType::Normal;
	isChange_ = false;
	isActive_ = true;

	cameraRotEase_ = 4.0f;
	faildCameraMoveEase_ = 0.3f;
}

void NCameraManager::Update()
{
	//�ʏ�J�����̎��ɉE�N���b�N������f�o�b�O�J�������[�h�Ɛ؂�ւ��
	if (NInput::TriggerMouse(NInput::MouseRight))
	{
		if (nowCameraType_ != (uint32_t)CameraType::Debug)
		{
			prevCameraType_ = nowCameraType_;

			ChangeCameara(CameraType::Debug);
		}
		else if (nowCameraType_ == (uint32_t)CameraType::Debug)
		{
			ChangeCameara((CameraType)prevCameraType_);
		}
	}

	// �֐��|�C���^
	void (NCameraManager:: * pFunc[])() =
	{
		// �o�^
		&NCameraManager::NormalCameraUpdate,
		&NCameraManager::DebugCameraUpdate,
		&NCameraManager::TitleCameraUpdate,
		&NCameraManager::FaildCameraUpdate,
	};

	// ���s
	(this->*pFunc[nowCameraType_])();
}

void NCameraManager::ChangeCameara(const CameraType cameraType)
{
	nowCameraType_ = (uint32_t)cameraType;
	isChange_ = false;
	isActive_ = true;
}

void NCameraManager::SetDebugCamera(NCamera& camera)
{
	debugCamera_ = camera;
}
