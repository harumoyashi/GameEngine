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
	static NCamera* sNowCamera;

private:
	//カメラ
	NMatrix4 matProjection_{};
	NMatrix4 matView_{};

	//カメラ
	NVector3 eye_ = { 0, 10.0f, -30.0f };	//視点座標
	NVector3 target_ = { 0, 0, 0 };			//注視点座標
	NVector3 up_ = { 0, 1.0f, 0 };			//上方向ベクトル
	float angle_ = 0.0f;					//カメラの回転角

public:
	//ビュー変換行列作成
	void CreateMatView();
	//射影投影変換//
	void ProjectiveProjection();

	//セッター
	inline void SetEye(const NVector3& eye) { eye_ = eye; }
	inline void SetTarget(const NVector3& target) { target_ = target; }
	inline void SetUp(const NVector3& up) { up_ = up; }

	//ゲッター
	inline NMatrix4 GetMatProjection() { return matProjection_; }
	inline NMatrix4 GetMatView() { return matView_; }
	//eye座標取得
	inline NVector3 GetPos() { return eye_; }
};