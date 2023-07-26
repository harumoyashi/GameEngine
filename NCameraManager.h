#pragma once
#include "NCamera.h"
#include "NEasing.h"

//ここにカメラの種類登録
enum class CameraType
{
	Normal,	//通常(プレイ中)
	Debug,	//デバッグ用
	Title,	//タイトル用
	Faild,	//失敗リザルト用
	Clear,	//クリアリザルト用
};

class NCameraManager final
{
private:
	uint32_t nowCameraType_;	//現在のカメラの種類
	uint32_t prevCameraType_;	//デバッグカメラに切り替える前のカメラの種類

	NCamera nowCamera_;			//今のカメラ情報格納用

	//通常時のカメラ関連
	NCamera normalCamera_;						//通常のカメラ
	NEasing::EaseTimer normalCameraMoveEase_;	//通常カメラに持ってくためのイージング
	//通常時のカメラ関連
	NCamera debugCamera_;						//デバッグ用カメラ
	//タイトル時のカメラ関連
	NCamera titleCamera_;						//タイトルのカメラ
	NEasing::EaseTimer titleCameraMoveEase_;	//タイトルカメラに持ってくためのイージング
	NEasing::EaseTimer cameraRotEase_;			//カメラを回転させる用のタイマー
	//リザルト時のカメラ関連
	NCamera faildCamera_;						//リザルトのカメラ
	NEasing::EaseTimer faildCameraMoveEase_;	//リザルトカメラに持ってくためのイージング

	// 姿勢制御関連
	NVector3 frontVec_;
	NVector3 upVec_;
	NVector3 rightVec_;

	bool isChange_;				//カメラの種類切り替えフラグ
	bool isActive_;

	//共通のカメラに必要な情報
	NVector3 currentPos_;		//現在のカメラ座標
	NVector3 nextPos_;			//持っていきたいカメラ座標
	NVector3 currentTarget_;	//現在のカメラの注視点座標
	NVector3 nextTarget_;		//持っていきたいカメラの注視点座標
	NVector3 currentUpVec_;		//現在のカメラの注視点座標
	NVector3 nextUpVec_;		//持っていきたいカメラの注視点座標
	float currentFov_;			//現在のカメラ視野
	float nextFov_;				//持っていきたいカメラ視野
	float length_;				//見るものとの距離

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
	// リザルト時
	void FaildCameraInit();
	void FaildCameraUpdate();

	//Vec3のイージング用
	NVector3 InQuad(const NVector3& start,const NVector3& end,float timerate);
	NVector3 OutQuad(const NVector3& start,const NVector3& end,float timerate);

public:
	//シングルトンインスタンス取得
	static NCameraManager* GetInstance();

	void Init();
	void Update();

	void ChangeCameara(const CameraType cameraType);

	bool GetisActive() { return isActive_; }

	void SetDebugCamera(NCamera& camera);
};