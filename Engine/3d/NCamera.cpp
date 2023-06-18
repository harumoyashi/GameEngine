#include "NCamera.h"
#include "NWindows.h"
#include "NMathUtil.h"
#include "NInput.h"
#include "NVector4.h"

std::unique_ptr<NCamera> NCamera::sNCamera = std::make_unique<NCamera>();
NCamera* NCamera::sCurrentCamera = sNCamera.get();

void NCamera::Reset()
{
	eye_ = { 0, 10.0f, -30.0f };	//視点座標
	target_ = { 0, 0, 0 };			//注視点座標
	up_ = { 0, 1.0f, 0 };			//上方向ベクトル
	rot_ = { 0,0,0 };				//カメラの回転角
	nearZ_ = 0.1f;
	farZ_ = 1000.0f;

	//デバッグカメラか否か
	isDebugCamera_ = false;
	distance = 20.0f;

	CreateMatView();
	ProjectiveProjection();
}

void NCamera::Update()
{
	CreateMatView();
	ProjectiveProjection();
}

void NCamera::DebugCameraMove()
{
	up_.x = matView_.m[1][0];
	up_.y = matView_.m[1][1];
	up_.z = matView_.m[1][2];

	const NVector3 frontVec =
	{
		sinf(sCurrentCamera->rot_.y),
		-sinf(sCurrentCamera->rot_.x),
		cosf(sCurrentCamera->rot_.y),
	};

	// 回転
	if (NInput::TriggerMouse(NInput::MouseLeft))
	{
		if (NInput::GetMouseMove().x != 0 || NInput::GetMouseMove().y != 0)
		{
			const float moveSpeed = 0.005f;
			sCurrentCamera->rot_.x += NInput::GetMouseMove().y * moveSpeed;
			sCurrentCamera->rot_.y += NInput::GetMouseMove().x * moveSpeed;
		}
	}

	// 平行移動
	if (NInput::TriggerMouse(NInput::MouseMiddle))
	{
		if (NInput::GetMouseMove().x != 0 || NInput::GetMouseMove().y != 0)
		{
			const float moveSpeed = 1.1f;
			const NVector3 rightVec = frontVec.Cross({ 0,1,0 });
			sCurrentCamera->eye_ += rightVec * NInput::GetMouseMove().x * moveSpeed;
		}
	}

	// 奥行
	if (NInput::GetMouseMove().z != 0)
	{
		const float moveSpeed = 0.025f;
		sCurrentCamera->eye_ += frontVec * NInput::GetMouseMove().z * moveSpeed;
	}
}

void NCamera::CreateMatView()
{
	matView_ = MathUtil::MatView(eye_, target_, up_, rot_);
}

void NCamera::ProjectiveProjection()
{
	matProjection_ = MathUtil::PerspectiveProjection(
		MathUtil::Degree2Radian(45.0f),
		(float)NWindows::kWin_width / NWindows::kWin_height,
		nearZ_, farZ_);
}

void NCamera::SetNearFar(const float nearZ, const float farZ)
{
	nearZ_ = nearZ;
	farZ_ = farZ;
}
