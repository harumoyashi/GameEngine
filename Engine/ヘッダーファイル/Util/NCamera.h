#pragma once
#include <DirectXMath.h>
#include <memory>

#include "NVec2.h"
#include "NVec3.h"
#include "NMatrix4.h"

class NCamera final
{
public:
	static std::unique_ptr<NCamera> sNCamera;
	static NCamera* sCurrentCamera;

private:
	//変換用行列
	NMatrix4 matProjection_{};
	NMatrix4 matView_{};

	//カメラ
	NVec3 eye_ = { 0, 10.0f, -30.0f };	//視点座標
	NVec3 target_ = { 0, 0, 0 };			//注視点座標
	NVec3 up_ = { 0, 1.0f, 0 };			//上方向ベクトル
	NVec3 rot_ = NVec3::zero;				//カメラの回転角
	float nearZ_ = 0.1f;
	float farZ_ = 1000.0f;
	float fov_ = 45.0f;

private:
	//ビュー変換行列作成
	void CreateMatView();
	//射影投影変換//
	void ProjectiveProjection();

public:
	void Init();
	void Update();
	//デバッグカメラ用の移動処理
	void DebugCameraUpdate();

	//セッター
	void SetEye(const NVec3& eye) { eye_ = eye; }
	void SetTarget(const NVec3& target) { target_ = target; }
	void SetUpVec(const NVec3& up) { up_ = up; }
	void SetRot(const NVec3& rot) { rot_ = rot; }
	void SetMatView(const NMatrix4& mat) { matView_ = mat; }
	void SetNearFar(const float nearZ, const float farZ);
	void SetFov(const float fov) { fov_ = fov; }

	//ゲッター
	const NMatrix4& GetMatProjection()const { return matProjection_; }
	const NMatrix4& GetMatView()const { return matView_; }
	//eye座標取得
	const NVec3& GetPos()const { return eye_; }
	//注視点座標取得
	const NVec3& GetTarget()const { return target_; }
	//上ベクトル取得
	const NVec3& GetUpVec()const { return up_; }

	const float GetFov()const { return fov_; }
};