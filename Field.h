#pragma once
#include "NObj3d.h"
#include "NSprite.h"
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
	std::unique_ptr<NObj3d> fieldObj_;
	std::vector<std::unique_ptr<NObj3d>> obj_;	//オブジェクト群

	const float kStartPos = 5.0f;	//スタート地点(縦だけ)
	float linePos_ = 0.0f;			//横線の位置
	float startOffset_ = 5.0f;		//スタート文字のオフセット

	float slidePos_ = 0.0f;			//横に掃けてく時のスライド位置
	NEasing::EaseTimer slideTimer_ = 0.1f;	//スライド時のイージング用

	bool isStart = false;			//スタートしたかフラグ

public:
	static Field* GetInstance();

	void Init();
	void Update();
	void DrawObj();
	void DrawSprite();

	// ゲッター //
	//スタート地点座標取得
	inline float GetStartPos() const { return kStartPos; }

	// セッター //

};