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
	nextUpVec_ = NVector3(0, 0, 1);	//下見下ろす形にする

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

	//前のカメラから諸々イージングしてから下の処理やりたい
	//カメラ回転用のタイマー動かし続ける
	cameraRotEase_.Update();
	if (cameraRotEase_.GetStarted() == false)
	{
		cameraRotEase_.Start();
	}
	else if (cameraRotEase_.GetEnd())
	{
		cameraRotEase_.Reset();
	}

	NVector2 vec2;	//カメラに足すベクトル
	//円運動させる
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

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = Player::GetInstance()->GetPos();

	currentPos_ = NCamera::sCurrentCamera->GetPos();
	nextPos_ = Player::GetInstance()->GetPos() - NVector3(0, -length_ * 0.5f, length_);

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

	//カメラ動くことないからイージング終わったら放置でいいかも
	faildCamera_.SetTarget(nextTarget_);
	faildCamera_.SetEye(nextPos_);

	faildCamera_.Update();
	NCamera::sCurrentCamera = &faildCamera_;
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
		&NCameraManager::TitleCameraUpdate,
		&NCameraManager::FaildCameraUpdate,
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
