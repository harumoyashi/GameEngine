#include "NCameraManager.h"
#include "Player.h"
#include "Field.h"

void NCameraManager::NormalCameraInit()
{
	length_ = 10.0f;
	normalCameraMoveEase_.Reset();

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = Player::GetInstance()->GetPos();

	currentPos_ = NCamera::sCurrentCamera->GetEye();
	nextPos_ = nextTarget_ + NVec3(0, length_, -length_ * 0.5f);

	currentUpVec_ = NCamera::sCurrentCamera->GetUpVec();
	nextUpVec_ = NVec3(0, 0, 1);	//下見下ろす形にする

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
	//イージング用のタイマー動かす
	normalCameraMoveEase_.Update();
	if (normalCameraMoveEase_.GetStarted() == false)
	{
		normalCameraMoveEase_.Start();
	}

	//前のカメラから現在のカメラまでの補間
	if (normalCameraMoveEase_.GetRun())
	{
		NVec3 target, pos, upVec;
		target = OutQuad(currentTarget_, nextTarget_, normalCameraMoveEase_.GetTimeRate());
		pos = OutQuad(currentPos_, nextPos_, normalCameraMoveEase_.GetTimeRate());
		upVec = OutQuad(currentUpVec_, nextUpVec_, normalCameraMoveEase_.GetTimeRate());

		normalCamera_.SetTarget(target);
		normalCamera_.SetEye(pos);
		normalCamera_.SetUpVec(upVec);
	}
	else if (normalCameraMoveEase_.GetEnd())
	{
		normalCamera_.SetTarget(Player::GetInstance()->GetPos());
		normalCamera_.SetEye(normalCamera_.GetTarget() + NVec3(0, length_, -length_ * 0.5f));
		normalCamera_.SetUpVec(nextUpVec_);
	}

	normalCamera_.SetEye(normalCamera_.GetEye() + shakeVec_);

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

	currentPos_ = NCamera::sCurrentCamera->GetEye();
	nextPos_ = nextTarget_ + NVec3(-length_, 0, length_);

	currentUpVec_ = NCamera::sCurrentCamera->GetUpVec();
	nextUpVec_ = NVec3(0, 1, 0);

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

	NVec2 vec2;	//カメラに足すベクトル
	//円運動させる
	vec2 = MathUtil::CircleMotion(
		{ titleCamera_.GetTarget().x,titleCamera_.GetTarget().z },
		length_,
		cameraRotEase_.GetTimeRate() * PI2);

	titleCamera_.SetTarget(Player::GetInstance()->GetPos());
	titleCamera_.SetEye(NVec3(vec2.x, length_, vec2.y));
	titleCamera_.SetUpVec(nextUpVec_);

	titleCamera_.SetEye(titleCamera_.GetEye() + shakeVec_);

	titleCamera_.Update();
	NCamera::sCurrentCamera = &titleCamera_;
}

void NCameraManager::BeforeStartCameraInit()
{
	length_ = 10.0f;
	beforeStartCameraMoveEase_.Reset();

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = { 0,0,Field::GetInstance()->GetGoalPos() };

	currentPos_ = NCamera::sCurrentCamera->GetEye();
	nextPos_ = nextTarget_ + NVec3(0, length_, -length_ * 0.8f);

	currentUpVec_ = NCamera::sCurrentCamera->GetUpVec();
	//あえてZ方向を見る形にすることで、見下ろす形に遷移してくといい感じのカメラワークになる
	//多分回転とかでやったほうがいいけどまあいいやいい感じだもん
	nextUpVec_ = NVec3(0, 1, 0);

	currentFov_ = NCamera::sCurrentCamera->GetFov();
	nextFov_ = 45.0f;
}

void NCameraManager::BeforeStartCameraUpdate()
{
	if (isChange_ == false)
	{
		BeforeStartCameraInit();
		isChange_ = true;
	}

	////イージング用のタイマー動かす
	//beforeStartCameraMoveEase_.Update();
	//if (beforeStartCameraMoveEase_.GetStarted() == false)
	//{
	//	beforeStartCameraMoveEase_.Start();
	//}

	////前のカメラから現在のカメラまでの補間
	//if (beforeStartCameraMoveEase_.GetRun())
	//{
	//	NVec3 target, pos, upVec;
	//	target = OutQuad(currentTarget_, nextTarget_, beforeStartCameraMoveEase_.GetTimeRate());
	//	pos = OutQuad(currentPos_, nextPos_, beforeStartCameraMoveEase_.GetTimeRate());
	//	upVec = OutQuad(currentUpVec_, nextUpVec_, beforeStartCameraMoveEase_.GetTimeRate());

	//	beforeStartCamera_.SetTarget(target);
	//	beforeStartCamera_.SetEye(pos);
	//	beforeStartCamera_.SetUpVec(upVec);
	//}	//カメラ動くことないから補間終わったら放置

	beforeStartCamera_.SetTarget(nextTarget_);
	beforeStartCamera_.SetEye(nextPos_);
	beforeStartCamera_.SetUpVec(nextUpVec_);

	beforeStartCamera_.SetEye(beforeStartCamera_.GetEye() + shakeVec_);

	beforeStartCamera_.Update();
	NCamera::sCurrentCamera = &beforeStartCamera_;
}

void NCameraManager::FaildCameraInit()
{
	length_ = 2.0f;
	faildCameraMoveEase_.Reset();

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = Player::GetInstance()->GetHeadPos();

	currentPos_ = NCamera::sCurrentCamera->GetEye();
	nextPos_ = Player::GetInstance()->GetPos() + NVec3(0, length_ * 0.5f, -length_);

	currentUpVec_ = NCamera::sCurrentCamera->GetUpVec();
	nextUpVec_ = NVec3(0, 1, 0);

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

	//イージング用のタイマー動かす
	faildCameraMoveEase_.Update();
	if (faildCameraMoveEase_.GetStarted() == false)
	{
		faildCameraMoveEase_.Start();
	}

	//前のカメラから現在のカメラまでの補間
	if (faildCameraMoveEase_.GetRun())
	{
		NVec3 target, pos, upVec;
		target = OutQuad(currentTarget_, nextTarget_, faildCameraMoveEase_.GetTimeRate());
		pos = OutQuad(currentPos_, nextPos_, faildCameraMoveEase_.GetTimeRate());
		upVec = OutQuad(currentUpVec_, nextUpVec_, faildCameraMoveEase_.GetTimeRate());

		faildCamera_.SetTarget(target);
		faildCamera_.SetEye(pos);
		faildCamera_.SetUpVec(upVec);
	}	//カメラ動くことないから補間終わったら放置

	faildCamera_.SetEye(faildCamera_.GetEye() + shakeVec_);

	faildCamera_.Update();
	NCamera::sCurrentCamera = &faildCamera_;
}

void NCameraManager::ClearCameraInit()
{
	length_ = 1.0f;
	clearCameraMoveEase_.Reset();

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = Player::GetInstance()->GetHeadPos();

	currentPos_ = NCamera::sCurrentCamera->GetEye();
	nextPos_ = Player::GetInstance()->GetHeadPos() + NVec3(-length_, length_ * 0.5f, length_);

	currentUpVec_ = NCamera::sCurrentCamera->GetUpVec();
	nextUpVec_ = NVec3(0, 1, 0);

	currentFov_ = NCamera::sCurrentCamera->GetFov();
	nextFov_ = 45.0f;
}

void NCameraManager::ClearCameraUpdate()
{
	if (isChange_ == false)
	{
		ClearCameraInit();
		isChange_ = true;
	}

	//イージング用のタイマー動かす
	clearCameraMoveEase_.Update();
	if (clearCameraMoveEase_.GetStarted() == false)
	{
		clearCameraMoveEase_.Start();
	}

	//前のカメラから現在のカメラまでの補間
	if (clearCameraMoveEase_.GetRun())
	{
		//ずれが起きないように更新し続ける
		currentTarget_ = NCamera::sCurrentCamera->GetTarget();
		nextTarget_ = Player::GetInstance()->GetHeadPos();
		currentPos_ = NCamera::sCurrentCamera->GetEye();
		nextPos_ = Player::GetInstance()->GetHeadPos() + NVec3(-length_, length_ * 0.5f, length_);
		currentUpVec_ = NCamera::sCurrentCamera->GetUpVec();

		NVec3 target, pos, upVec;
		target = InQuad(currentTarget_, nextTarget_, clearCameraMoveEase_.GetTimeRate());
		pos = InQuad(currentPos_, nextPos_, clearCameraMoveEase_.GetTimeRate());
		upVec = InQuad(currentUpVec_, nextUpVec_, clearCameraMoveEase_.GetTimeRate());

		clearCamera_.SetTarget(target);
		clearCamera_.SetEye(pos);
		clearCamera_.SetUpVec(upVec);
	}

	//補間終わった後もプレイヤーが動くので追いかけ続ける
	if (clearCameraMoveEase_.GetEnd())
	{
		NVec3 pos;
		pos = Player::GetInstance()->GetHeadPos() + NVec3(-length_, length_ * 0.5f, length_);
		clearCamera_.SetEye(pos);
		clearCamera_.SetTarget(Player::GetInstance()->GetHeadPos());
	}

	clearCamera_.SetEye(clearCamera_.GetEye() + shakeVec_);

	clearCamera_.Update();
	NCamera::sCurrentCamera = &clearCamera_;
}

void NCameraManager::ShakeStart(float shakePower, float shakeTime)
{
	shakeTimer_ = shakeTime;
	shakeTimer_.Start();
	shakePower_ = shakePower;
	shakePrevPos = NCamera::sCurrentCamera->GetEye();
}

void NCameraManager::ShakeUpdate()
{
	if (shakeTimer_.GetStarted())
	{
		float shakeVecRand = MathUtil::Randomf(-1.f, 1.f);	//ランダムで揺らす方向決める

		shakeVec_ =
		{
			shakeVecRand,
			shakeVecRand,
			shakeVecRand
		};
		//大きさかけて、タイマーが進むごとに揺れ小さくなってく
		shakeVec_ *= shakePower_ * (1.f - shakeTimer_.GetTimeRate());
		shakeTimer_.Update();
	}
	else if(shakeTimer_.GetEnd())
	{
		//終わったら揺れる前の位置に戻してあげて、揺れの値も0に
		shakeVec_ = NVec3::zero;
		NCamera::sCurrentCamera->SetEye(shakePrevPos);
	}
}

NVec3 NCameraManager::InQuad(const NVec3& start, const NVec3& end, float timerate)
{
	NVec3 result;
	result.x = NEasing::InQuad(start.x, end.x, timerate);
	result.y = NEasing::InQuad(start.y, end.y, timerate);
	result.z = NEasing::InQuad(start.z, end.z, timerate);
	return result;
}

NVec3 NCameraManager::OutQuad(const NVec3& start, const NVec3& end, float timerate)
{
	NVec3 result;
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
	normalCameraMoveEase_ = 0.8f;
	beforeStartCameraMoveEase_ = 0.3f;
	faildCameraMoveEase_ = 0.3f;
	clearCameraMoveEase_ = 0.5f;
}

void NCameraManager::Update()
{
	ShakeUpdate();

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
		&NCameraManager::BeforeStartCameraUpdate,
		&NCameraManager::FaildCameraUpdate,
		&NCameraManager::ClearCameraUpdate,
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
