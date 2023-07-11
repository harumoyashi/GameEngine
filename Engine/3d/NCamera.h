#pragma once
#include <DirectXMath.h>
#include <memory>

#include "NVector2.h"
#include "NVector3.h"
#include "NMatrix4.h"

class NCamera
{
public:
	static std::unique_ptr<NCamera> sNCamera;
	static NCamera* sCurrentCamera;

private:
	//変換用行列
	NMatrix4 matProjection_{};
	NMatrix4 matView_{};

	//カメラ
	NVector3 eye_ = { 0, 10.0f, -30.0f };	//視点座標
	NVector3 target_ = { 0, 0, 0 };			//注視点座標
	NVector3 up_ = { 0, 1.0f, 0 };			//上方向ベクトル
	NVector3 rot_ = { 0,0,0 };				//カメラの回転角
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
	inline void SetEye(const NVector3& eye) { eye_ = eye; }
	inline void SetTarget(const NVector3& target) { target_ = target; }
	inline void SetUpVec(const NVector3& up) { up_ = up; }
	inline void SetRot(const NVector3& rot) { rot_ = rot; }
	inline void SetMatView(const NMatrix4& mat) { matView_ = mat; }
	void SetNearFar(const float nearZ, const float farZ);
	void SetFov(const float fov) { fov_ = fov; }

	//ゲッター
	inline const NMatrix4& GetMatProjection()const { return matProjection_; }
	inline const NMatrix4& GetMatView()const { return matView_; }
	//eye座標取得
	inline const NVector3& GetPos()const { return eye_; }
	//注視点座標取得
	inline const NVector3& GetTarget()const { return target_; }
	//上ベクトル取得
	inline const NVector3& GetUpVec()const { return up_; }

	inline const float GetFov()const { return fov_; }
};