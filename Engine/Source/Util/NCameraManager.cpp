#include "NCameraManager.h"
#include "NInput.h"
#include "NMathUtil.h"

void NCameraManager::NormalCameraInit()
{
	length_ = 10.0f;
	normalCameraMoveEase_.Reset();

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = NVec3::zero;

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
		target = MathUtil::OutQuad(currentTarget_, nextTarget_, normalCameraMoveEase_.GetTimeRate());
		pos = MathUtil::OutQuad(currentPos_, nextPos_, normalCameraMoveEase_.GetTimeRate());
		upVec = MathUtil::OutQuad(currentUpVec_, nextUpVec_, normalCameraMoveEase_.GetTimeRate());

		normalCamera_.SetTarget(target);
		normalCamera_.SetEye(pos);
		normalCamera_.SetUpVec(upVec);
	}
	else if (normalCameraMoveEase_.GetEnd())
	{
		normalCamera_.SetTarget(NVec3::zero);
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
	length_ = 1.5f;
	cameraRotEase_.Reset();

	currentTarget_ = NCamera::sCurrentCamera->GetTarget();
	nextTarget_ = NVec3::zero;

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

	titleCamera_.SetTarget(NVec3::zero);
	titleCamera_.SetEye(NVec3(vec2.x, length_, vec2.y));
	titleCamera_.SetUpVec(nextUpVec_);

	titleCamera_.SetEye(titleCamera_.GetEye() + shakeVec_);

	titleCamera_.Update();
	NCamera::sCurrentCamera = &titleCamera_;
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
	shakeTimer_.Update();

	if (shakeTimer_.GetStarted())
	{
		float shakeVecRand = MathUtil::Randomf(-1.f, 1.f);	//ランダムで揺らす方向決める

		shakeVec_ =
		{
			shakeVecRand,
			shakeVecRand,
			0
		};
		//大きさかけて、タイマーが進むごとに揺れ小さくなってく
		shakeVec_ *= shakePower_ * (1.f - shakeTimer_.GetTimeRate());
	}
	
	if (shakeTimer_.GetEnd())
	{
		//終わったら揺れる前の位置に戻してあげて、揺れの値も0に
		shakeVec_ = NVec3::zero;
		NCamera::sCurrentCamera->SetEye(shakePrevPos);

		shakeTimer_.Reset();
	}
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

	cameraRotEase_ = 8.0f;
	normalCameraMoveEase_ = 0.8f;
}

void NCameraManager::Update()
{
	ShakeUpdate();

	//通常カメラの時に右クリックしたらデバッグカメラモードと切り替わる
#ifdef _DEBUG
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
#endif // _DEBUG

	// 関数ポインタ
	void (NCameraManager:: * pFunc[])() =
	{
		// 登録
		&NCameraManager::NormalCameraUpdate,
		&NCameraManager::DebugCameraUpdate,
		&NCameraManager::TitleCameraUpdate,
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
