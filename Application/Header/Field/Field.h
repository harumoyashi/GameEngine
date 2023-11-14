#pragma once
#include "NObj3d.h"
#include "NCollider.h"
#include "PlaneCollider.h"
#include "NTile.h"
#include "BackObj.h"
#include "NEasing.h"
#include <array>

//床
class Field final
{
public:
	enum class LineType
	{
		Start,	//スタート
		Goal,	//ゴール

		MaxSize
	};

	//フィールドにある線(スタート、ゴール、チェックポイント)
	struct Line
	{
		std::unique_ptr<NObj3d> line;			//線
		std::unique_ptr<NObj3d> text;			//文字

		float linePosZ = 0.0f;					//横線の位置
		float offset = 5.0f;					//文字どこに置くか(線からのオフセット)

		bool isSlide = false;					//スライドするかフラグ
		float slidePos = 0.0f;					//横に掃けてく時のスライド位置
		NEasing::EaseTimer slideTimer = 0.1f;	//スライド時のイージング用
	};

private:
	std::array<std::unique_ptr<NTile>,2> fieldObj_;		//床2枚用意してスクロールさせる
	std::array<std::unique_ptr<BackObj>,2> backObj_;	//背景オブジェクト2枚用意してスクロールさせる
	std::array<PlaneCollider,2> collider_;				//床の当たり判定も2枚分用意
	std::vector<Line> lines_;							//フィールドにある線たち
	std::vector<Line> checkPoints_;						//フィールドにあるチェックポイントたち

	float tileDivide_ = 0.5f;							//タイルの分割幅
	float activityAreaX_ = 9.0f;						//行動範囲制限
	float avoidArea_ = 2.0f;							//割れる範囲
	std::array<NEasing::EaseTimer,3> extrusionTimer_;	//押し出すタイマー
	std::array<NVec3,maxObj> objPos_;					//フィールド上のオブジェクト座標
	bool isAvoid_ = true;								//避けるようになるか否か
	uint32_t checkPointNum = 5;							//チェックポイントの数

	float startPosZ_ = 3.0f;							//スタート地点(縦だけ)
	float goalPosZ_ = 40.0f;							//ゴール地点(縦だけ)

	bool isStart_ = false;								//スタートしたかフラグ
	bool isGoal_ = false;								//ゴールしたかフラグ

public:
	static Field* GetInstance();

	void Init();
	void Update();
	void Draw();

	//何かに当たった時の処理
	void OnCollision();

	void StopSE();

	void FieldScroll(float standardPosZ);

	// ゲッター //
	//スタート地点座標取得
	float GetStartPos() const { return startPosZ_; }
	//ゴール地点座標取得
	float GetGoalPos() const { return goalPosZ_; }
	//スタートしたかフラグ取得
	bool GetIsStart() const { return isStart_; }
	//ゴールしたかフラグ取得
	bool GetIsGoal() const { return isGoal_; }
	//行動範囲取得
	float GetActivityAreaX() const { return activityAreaX_; }
	//色取得
	NColor GetColor() { return fieldObj_[0]->color_; }

	// セッター //
	void SetIsGoal(bool isGoal) { isGoal_ = isGoal, isStart_ = isGoal; }

private:
	Field() = default;
	Field(const Field&) = delete;
	~Field() = default;
	Field& operator=(const Field&) = delete;
};