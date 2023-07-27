#pragma once
#include "NObj3d.h"
#include "NTile.h"
#include "NEasing.h"

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
	std::unique_ptr<NTile> fieldObj_;			//床
	std::vector<Line> lines_;					//フィールドにある線たち
	std::vector<Line> checkPoints_;				//フィールドにあるチェックポイントたち

	float tileDivide_ = 0.5f;					//タイルの分割幅
	float activityAreaX_ = 10.0f;				//行動範囲制限
	uint32_t checkPosintNum = 3;				//チェックポイントいくつか
	
	float startPosZ_ = 5.0f;					//スタート地点(縦だけ)
	float goalPosZ_ = 20.0f;					//ゴール地点(縦だけ)

	bool isStart_ = false;						//スタートしたかフラグ
	bool isGoal_ = false;						//ゴールしたかフラグ

public:
	static Field* GetInstance();

	void Init();
	void Update();
	void Draw();

	// ゲッター //
	//スタート地点座標取得
	float GetStartPos() const { return startPosZ_; }
	//フィールドの大きさ取得
	NVector3 GetScale() const { return fieldObj_->scale_; }
	//スタートしたかフラグ取得
	bool GetIsStart() const { return isStart_; }
	//ゴールしたかフラグ取得
	bool GetIsGoal() const { return isGoal_; }
	//行動範囲取得
	float GetActivityAreaX() const { return activityAreaX_; }

	// セッター //


private:
	Field() = default;
	Field(const Field&) = delete;
	~Field() = default;
	Field& operator=(const Field&) = delete;
};