#include "NCameraManager.h"
#include "Player.h"

void NCameraManager::NormalCameraInit()
{
	length_ = 80.0f;

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = Player::GetInstance()->GetPos();

	currentPos_ = NCamera::sCurrentCamera->GetPos();
	nextPos_ = nextTarget_ - NVector3(0, length_, 0);

	currentUpVec_ = NCamera::sCurrentCamera->GetUpVec();
	nextUpVec_ = NVector3(0, 0, 1);

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

	//前のカメラから諸々イージングしてから下の処理やりたい

	normalCamera_.SetTarget(Player::GetInstance()->GetPos());
	normalCamera_.SetEye(normalCamera_.GetTarget() + NVector3(0, length_, 0));
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

void NCameraManager::ResultCameraInit()
{
	length_ = 20.0f;

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = Player::GetInstance()->GetPos();

	currentPos_ = NCamera::sCurrentCamera->GetPos();
	nextPos_ = Player::GetInstance()->GetPos() - NVector3(0, 0, -length_);

	currentFov_ = NCamera::sCurrentCamera->GetFov();
	nextFov_ = 45.0f;
}

void NCameraManager::ResultCameraUpdate()
{
	if (isChange_ == false)
	{
		ResultCameraInit();
		isChange_ = true;
	}

	//カメラ動くことないからイージング終わったら放置でいいかも
	resultCamera_.SetTarget(nextTarget_);
	resultCamera_.SetEye(nextPos_);

	resultCamera_.Update();
	NCamera::sCurrentCamera = &resultCamera_;
}

NCameraManager* NCameraManager::GetInstance()
{
	static NCameraManager instance;
	return &instance;
}

void NCameraManager::Init()
{
	NCamera::sCurrentCamera->Init();

	length_ = 80.0f;

	nowCameraType_ = (uint32_t)CameraType::Normal;
	isChange_ = false;
	isActive_ = true;
}

void NCameraManager::Update()
{
	//通常カメラの時に右クリックしたらデバッグカメラモードと切り替わる
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

	// 関数ポインタ
	void (NCameraManager:: * pFunc[])() =
	{
		// 登録
		&NCameraManager::NormalCameraUpdate,
		&NCameraManager::DebugCameraUpdate,
		&NCameraManager::ResultCameraUpdate,
	};

	// 実行
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
