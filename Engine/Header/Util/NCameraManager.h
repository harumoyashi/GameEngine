#pragma once
#include "NCamera.h"
#include "NEasing.h"

// カメラ管理、制御 //

//ここにカメラの種類登録
enum class CameraType
{
	Normal,			//通常(プレイ中)
	Debug,			//デバッグ用
	Title,			//タイトル用
};

class NCameraManager final
{
private:
	uint32_t nowCameraType_;	//現在のカメラの種類
	uint32_t prevCameraType_;	//デバッグカメラに切り替える前のカメラの種類

	NCamera nowCamera_;			//今のカメラ情報格納用

	//通常時のカメラ関連
	NCamera normalCamera_;							//通常のカメラ
	NEasing::EaseTimer normalCameraMoveEase_;		//通常カメラに持ってくためのイージング
	//デバッグ時のカメラ関連
	NCamera debugCamera_;							//デバッグ用カメラ
	//タイトル時のカメラ関連
	NCamera titleCamera_;							//タイトルのカメラ
	NEasing::EaseTimer titleCameraMoveEase_;		//タイトルカメラに持ってくためのイージング
	NEasing::EaseTimer cameraRotEase_;				//カメラを回転させる用のタイマー
	
	// 姿勢制御関連
	NVec3 frontVec_;
	NVec3 upVec_;
	NVec3 rightVec_;

	bool isChange_;			//カメラの種類切り替えフラグ
	bool isActive_;

	//共通のカメラに必要な情報
	NVec3 currentPos_;		//現在のカメラ座標
	NVec3 nextPos_;			//持っていきたいカメラ座標
	NVec3 currentTarget_;	//現在のカメラの注視点座標
	NVec3 nextTarget_;		//持っていきたいカメラの注視点座標
	NVec3 currentUpVec_;	//現在のカメラの注視点座標
	NVec3 nextUpVec_;		//持っていきたいカメラの注視点座標
	float currentFov_;		//現在のカメラ視野
	float nextFov_;			//持っていきたいカメラ視野
	float length_;			//見るものとの距離
	//カメラシェイク用
	NEasing::EaseTimer shakeTimer_;	//シェイク時間
	NVec3 shakeVec_;				//シェイク方向
	float shakePower_;				//シェイクの大きさ
	NVec3 shakePrevPos;				//シェイク前のカメラ座標

private:
	//　カメラ固有の初期化、更新処理
	//　通常時の初期化
	void NormalCameraInit();
	//　通常時の更新
	void NormalCameraUpdate();

	// デバッグカメラ時の更新
	void DebugCameraUpdate();

	// タイトル時の初期化
	void TitleCameraInit();
	// タイトル時の更新
	void TitleCameraUpdate();

	//　カメラシェイク用更新処理
	void ShakeUpdate();

public:
	//シングルトンインスタンス取得
	static NCameraManager* GetInstance();

	//初期化
	void Init();
	//更新
	void Update();

	//指定したカメラに変更
	void ChangeCameara(const CameraType cameraType);

	//有効フラグ取得
	bool GetIsActive() { return isActive_; }
	//通常カメラへの遷移が完了したかどうか取得
	bool GetIsNormalCameraChanged() { return normalCameraMoveEase_.GetEnd(); }
	//現在のカメラタイプがデバッグカメラかフラグを取得
	bool GetIsDebugCamera() { return (CameraType)nowCameraType_ == CameraType::Debug; }
	//現在のカメラの種類取得
	CameraType GetNowCameraType() { return (CameraType)nowCameraType_; }

	//今のカメラ情報をデバッグカメラに入れる
	void SetDebugCamera(NCamera& camera);

	//カメラシェイク開始、パラメーター設定
	//shakePower:シェイクの大きさ
	//シェイク時間(秒)
	void ShakeStart(float shakePower, float shakeTime);

private:
	//コンストラクタ
	NCameraManager() = default;
	//コピー禁止
	NCameraManager(const NCameraManager&) = delete;
	//デストラクタ
	~NCameraManager() = default;
	//コピー禁止
	NCameraManager& operator=(const NCameraManager&) = delete;
};