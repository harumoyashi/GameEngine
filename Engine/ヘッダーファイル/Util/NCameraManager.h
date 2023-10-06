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

	// 姿勢制御関連
	NVec3 frontVec_;
	NVec3 upVec_;
	NVec3 rightVec_;

	bool isChange_;				//カメラの種類切り替えフラグ
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

private:
	//カメラ固有の初期化、更新処理
	// 通常時
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

	//Vec3のイージング用
	NVec3 InQuad(const NVec3& start, const NVec3& end, float timerate);
	NVec3 OutQuad(const NVec3& start, const NVec3& end, float timerate);

public:
	//シングルトンインスタンス取得
	static NCameraManager* GetInstance();

	void Init();
	void Update();

	//指定したカメラに変更
	void ChangeCameara(const CameraType cameraType);

	bool GetIsActive() { return isActive_; }
	//通常カメラへの遷移が完了したかどうか取得
	bool GetIsNormalCameraChanged() { return normalCameraMoveEase_.GetEnd(); }

	void SetDebugCamera(NCamera& camera);

private:
	NCameraManager() = default;
	NCameraManager(const NCameraManager&) = delete;
	~NCameraManager() = default;
	NCameraManager& operator=(const NCameraManager&) = delete;
};