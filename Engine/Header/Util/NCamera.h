#pragma once
#include <memory>

#include "NVec2.h"
#include "NVec3.h"
#include "NMatrix4.h"

// カメラ //

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
	NVec3 target_ = { 0, 0, 0 };		//注視点座標
	NVec3 up_ = { 0, 1.0f, 0 };			//上方向ベクトル
	NVec3 rot_ = NVec3::zero;			//カメラの回転角
	float nearZ_ = 0.1f;				//描画範囲の近い側
	float farZ_ = 1000.0f;				//描画範囲の遠い側
	float fov_ = 45.0f;					//視野

private:
	//ビュー変換行列作成
	void CreateMatView();
	//射影投影変換//
	void ProjectiveProjection();

public:
	//初期化
	void Init();
	//更新
	void Update();
	//デバッグカメラ用の移動処理
	void DebugCameraUpdate();

	//セッター
	//視点座標設定
	void SetEye(const NVec3& eye) { eye_ = eye; }
	//注視点座標設定
	void SetTarget(const NVec3& target) { target_ = target; }
	//上方向ベクトル設定
	void SetUpVec(const NVec3& up) { up_ = up; }
	//カメラの回転角設定
	void SetRot(const NVec3& rot) { rot_ = rot; }
	//ビュー行列設定
	void SetMatView(const NMatrix4& mat) { matView_ = mat; }
	//描画範囲設定
	void SetNearFar(const float nearZ, const float farZ);
	//視野設定
	void SetFov(const float fov) { fov_ = fov; }

	//ゲッター
	//射影変換行列取得
	const NMatrix4& GetMatProjection()const { return matProjection_; }
	//ビュー行列取得
	const NMatrix4& GetMatView()const { return matView_; }
	//eye座標取得
	const NVec3& GetEye()const { return eye_; }
	//注視点座標取得
	const NVec3& GetTarget()const { return target_; }
	//上ベクトル取得
	const NVec3& GetUpVec()const { return up_; }
	//視野取得
	const float GetFov()const { return fov_; }
};