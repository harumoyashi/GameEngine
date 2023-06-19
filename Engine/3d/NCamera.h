#pragma once
#include <DirectXMath.h>
#include <memory>

#include "NVector2.h"
#include "NVector3.h"
#include "NMatrix4.h"

using namespace DirectX;

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

	//デバッグカメラか否か
	bool isDebugCamera_ = false;
	float distance = 20.0f;

public:
	void Reset();
	void Update();
	//デバッグカメラ用の移動処理
	void DebugCameraUpdate();

	//ビュー変換行列作成
	void CreateMatView();
	//射影投影変換//
	void ProjectiveProjection();

	//デバッグカメラモード切り替え
	void ChangeIsDebugCamera();

	//セッター
	inline void SetEye(const NVector3& eye) { eye_ = eye; }
	inline void SetTarget(const NVector3& target) { target_ = target; }
	inline void SetUp(const NVector3& up) { up_ = up; }
	void SetNearFar(const float nearZ, const float farZ);

	//ゲッター
	inline const NMatrix4& GetMatProjection()const { return matProjection_; }
	inline const NMatrix4& GetMatView()const { return matView_; }
	inline const bool GetIsCamera()const { return isDebugCamera_; }
	//eye座標取得
	inline const NVector3& GetPos()const { return eye_; }
};