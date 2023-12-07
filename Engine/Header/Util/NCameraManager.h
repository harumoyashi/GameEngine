#pragma once
#include "NCamera.h"
#include "NEasing.h"

//ここにカメラの種類登録
enum class CameraType
{
	Normal,			//通常(プレイ中)
	Debug,			//デバッグ用
	Title,			//タイトル用
	BeforeStart,	//始まる前にゴール見せる用
	Faild,			//失敗リザルト用
	Clear,			//クリアリザルト用
	Muteki,			//無敵演出用
	BossEntry,		//ボス登場演出用
};

enum class EntryState
{
	CameraChange,	//カメラ遷移
	ZoomOut,		//咆哮前にカメラ引き
	Shake			//咆哮中のカメラシェイク
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
	//始まる前にゴール見せる時のカメラ関連
	NCamera beforeStartCamera_;						//始まる前にゴール見せるカメラ
	NEasing::EaseTimer beforeStartCameraMoveEase_;	//始まる前にゴール見せるカメラに持ってくためのイージング
	//失敗リザルト時のカメラ関連
	NCamera faildCamera_;							//失敗リザルトのカメラ
	NEasing::EaseTimer faildCameraMoveEase_;		//失敗リザルトカメラに持ってくためのイージング
	//クリアリザルト時のカメラ関連
	NCamera clearCamera_;							//クリアリザルトのカメラ
	NEasing::EaseTimer clearCameraMoveEase_;		//クリアリザルトカメラに持ってくためのイージング
	//無敵演出時のカメラ関連
	NCamera mutekiCamera_;							//無敵演出時のカメラ
	NEasing::EaseTimer mutekiCameraMoveEase_;		//無敵演出時カメラに持ってくためのイージング
	//ボス登場演出時のカメラ関連
	NCamera entryCamera_;							//ボス登場演出時のカメラ
	NEasing::EaseTimer entryCameraMoveEase_;		//ボス登場演出時カメラに持ってくためのイージング
	NEasing::EaseTimer entryCameraZoomOutEase_;		//ボス登場演出時カメラ一瞬引くためのイージング
	NEasing::EaseTimer entryCameraShakeEase_;		//ボス登場演出時カメラ揺らすためのイージング
	EntryState entryState_ = EntryState::CameraChange;

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
	//　通常時
	void NormalCameraInit();
	void NormalCameraUpdate();
	// デバッグカメラ時
	void DebugCameraUpdate();
	// タイトル時
	void TitleCameraInit();
	void TitleCameraUpdate();
	// 始まる前にゴール見せる時
	void BeforeStartCameraInit();
	void BeforeStartCameraUpdate();
	// 失敗リザルト時
	void FaildCameraInit();
	void FaildCameraUpdate();
	// クリアリザルト時
	void ClearCameraInit();
	void ClearCameraUpdate();
	// 無敵演出時
	void MutekiCameraInit();
	void MutekiCameraUpdate();
	// ボス登場演出時
	void EntryCameraInit();
	void EntryCameraUpdate();

	//　カメラシェイク用更新処理
	void ShakeUpdate();

	

public:
	//シングルトンインスタンス取得
	static NCameraManager* GetInstance();

	void Init();
	void Update();

	//指定したカメラに変更
	void ChangeCameara(const CameraType cameraType);

	//有効フラグ取得
	bool GetIsActive() { return isActive_; }
	//通常カメラへの遷移が完了したかどうか取得
	bool GetIsNormalCameraChanged() { return normalCameraMoveEase_.GetEnd(); }
	//無敵演出カメラへの遷移が完了したかどうか取得
	bool GetIsMutekiCameraChanged() { return mutekiCameraMoveEase_.GetEnd(); }
	//ボスの登場演出カメラへの遷移が完了したかどうか取得
	bool GetIsEntryCameraChanged() { return entryCameraMoveEase_.GetEnd(); }
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
	NCameraManager() = default;
	NCameraManager(const NCameraManager&) = delete;
	~NCameraManager() = default;
	NCameraManager& operator=(const NCameraManager&) = delete;
};