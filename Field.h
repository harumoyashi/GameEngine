#pragma once
#include "NObj3d.h"
#include "NEasing.h"

//床。タイリングと行動範囲以外色変えする専用のシェーダーがいる。
class Field
{
public:
	enum class ObjType
	{
		Line,	//横線
		Start,	//スタート地点のやつ

		MaxSize
	};

private:
	std::unique_ptr<NObj3d> fieldObj_;			//床
	std::vector<std::unique_ptr<NObj3d>> obj_;	//オブジェクト群

	const float kStartPosZ = 5.0f;	//スタート地点(縦だけ)
	float linePosZ_ = 0.0f;			//横線の位置
	float startOffset_ = 5.0f;		//スタート文字のオフセット

	float slidePos_ = 0.0f;			//横に掃けてく時のスライド位置
	NEasing::EaseTimer slideTimer_ = 0.1f;	//スライド時のイージング用

	bool isStart_ = false;			//スタートしたかフラグ

public:
	static Field* GetInstance();

	void Init();
	void Update();
	void Draw();

	// ゲッター //
	//スタート地点座標取得
	inline float GetStartPos() const { return kStartPosZ; }
	//フィールドの大きさ取得
	inline NVector3 GetScale() const { return fieldObj_->scale_; }
	//スタートしたかフラグ取得
	inline bool GetIsStart() const { return isStart_; }

	// セッター //

};