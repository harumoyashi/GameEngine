#include "NCamera.h"
#include "NWindows.h"
#include "NMathUtil.h"
#include "NInput.h"
#include "NVector4.h"

#include "NImGuiManager.h"
#include "imgui.h"

std::unique_ptr<NCamera> NCamera::sNCamera = std::make_unique<NCamera>();
NCamera* NCamera::sCurrentCamera = sNCamera.get();

void NCamera::Init()
{
	eye_ = { 0, 10.0f, -30.0f };	//視点座標
	target_ = { 0, 0, 0 };			//注視点座標
	up_ = { 0, 1.0f, 0 };			//上方向ベクトル
	rot_ = { 0,0,0 };				//カメラの回転角
	nearZ_ = 0.1f;
	farZ_ = 1000.0f;

	CreateMatView();
	ProjectiveProjection();
}

void NCamera::Update()
{
	CreateMatView();
	ProjectiveProjection();

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::Text("eye:%f,%f,%f", eye_.x, eye_.y, eye_.z);
	ImGui::Text("target:%f,%f,%f", target_.x, target_.y, target_.z);
	ImGui::Text("up:%f,%f,%f", up_.x, up_.y, up_.z);
	ImGui::End();
#endif // DEBUG
}

void NCamera::DebugCameraUpdate()
{
	matView_.RotateX(rot_.x);
	matView_.RotateY(rot_.y);

	const NVector3 frontVec =
	{
		sinf(sCurrentCamera->rot_.y),
		-sinf(sCurrentCamera->rot_.x),
		cosf(sCurrentCamera->rot_.y),
	};

	// 回転
	if (NInput::PushMouse(NInput::MouseLeft))
	{
		if (NInput::GetMouseMove().x != 0 || NInput::GetMouseMove().y != 0)
		{
			float moveSpeed = 0.00001f * eye_.Length();
			moveSpeed = MathUtil::Clamp(moveSpeed, 0.001f, 1.0f);
			sCurrentCamera->rot_.x += NInput::GetMouseMove().y * moveSpeed;
			sCurrentCamera->rot_.y += NInput::GetMouseMove().x * moveSpeed;
		}
	}

	// 平行移動
	if (NInput::PushMouse(NInput::MouseMiddle))
	{
		if (NInput::GetMouseMove().x != 0 || NInput::GetMouseMove().y != 0)
		{
			const float moveSpeed = 0.001f * eye_.Length();
			const NVector3 rightVec = frontVec.Cross({ 0,1,0 });
			sCurrentCamera->eye_ += rightVec * NInput::GetMouseMove().x * moveSpeed;
			sCurrentCamera->eye_ += up_ * NInput::GetMouseMove().y * moveSpeed;
		}
	}

	// 奥行
	if (NInput::GetMouseMove().z != 0)
	{
		const float moveSpeed = 0.025f;
		sCurrentCamera->eye_ += frontVec * NInput::GetMouseMove().z * moveSpeed;
	}
	matView_ = MathUtil::MatViewLockTo(eye_, frontVec, up_);

	ProjectiveProjection();

	//ここで決めないとビュー変換で変わってがくがくしちゃう
	up_.x = matView_.m[1][0];
	up_.y = matView_.m[1][1];
	up_.z = matView_.m[1][2];

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::Text("eye:%f,%f,%f", eye_.x, eye_.y, eye_.z);
	ImGui::Text("target:%f,%f,%f", target_.x, target_.y, target_.z);
	ImGui::Text("up:%f,%f,%f", up_.x, up_.y, up_.z);
	ImGui::End();
#endif // DEBUG
}

void NCamera::CreateMatView()
{
	matView_ = MathUtil::MatViewLockAt(eye_, target_, up_);
}

void NCamera::ProjectiveProjection()
{
	matProjection_ = MathUtil::PerspectiveProjection(
		MathUtil::Degree2Radian(fov_),
		(float)NWindows::kWin_width / NWindows::kWin_height,
		nearZ_, farZ_);
}

void NCamera::SetNearFar(const float nearZ, const float farZ)
{
	nearZ_ = nearZ;
	farZ_ = farZ;
}
