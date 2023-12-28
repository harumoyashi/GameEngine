#pragma once

// イージング関数まとめ //

namespace NEasing
{
	// イージング群 //
	//等速
	float lerp(float start, float end, float timeRate);
	//イージング関数
	float InQuad(float timeRate);
	//イージング関数
	float OutQuad(float timeRate);
	//イージング関数
	float InQuad(float start, float end, float timeRate);
	//イージング関数
	float OutQuad(float start, float end, float timeRate);
	//イージング関数
	float InOutQuad(float timeRate);
	//イージング関数
	float InOutQuad(float start, float end, float timeRate);
	//イージング関数
	float OutBounce(float timeRate);
	//イージング関数
	float OutBounce(float start, float end, float timeRate);
	//イージング関数
	float InBounce(float start, float end, float timeRate);
	//イージング関数
	float InOutBounce(float start, float end, float timeRate);
	//イージング関数
	float InElastic(float timeRate);
	//イージング関数
	float OutElastic(float timeRate);
	//イージング関数
	float InElastic(float start, float end, float timeRate);
	//イージング関数
	float OutElastic(float start, float end, float timeRate);
	//イージング関数
	float InBack(float timeRate);
	//イージング関数
	float InBack(float start, float end, float timeRate);
	//イージング関数
	float OutBack(float timeRate);
	//イージング関数
	float OutBack(float start, float end, float timeRate);
	//イージング関数
	float InOutBack(float timeRate);
	//イージング関数
	float InOutBack(float start, float end, float timeRate);

	//イージング用のタイマー(秒数)
	class EaseTimer
	{
	private:
		//進行中フラグ
		bool run_ = false;
		//進行終了フラグ
		bool end_ = false;
		//後退中フラグ
		bool reverse_ = false;
		//後退終了フラグ
		bool reverseend_ = false;

	public:
		//コンストラクタ
		EaseTimer(float maxTime = 1.0f) {
			maxTime_ = maxTime;
		}

		float nowTime_ = 0.0f;		// 現在の経過時間
		float maxTime_ = 1.0f;		// 何秒かけて移動するか

		//タイマーが進んでるかフラグ取得
		bool GetRun()const { return run_; };
		//一度でも進行されたらtrueになるフラグ取得
		bool GetStarted()const { return run_ || end_; };
		//タイマーが進み切ったかフラグ取得
		bool GetEnd()const { return end_; };
		//進み切ったかフラグを設定
		void SetEnd(bool end) { end_ = end; };

		//タイマーが戻ってるかフラグ取得
		bool GetReverse()const { return reverse_; };
		//タイマーが戻り切ったかフラグ取得
		bool GetReverseEnd()const { return reverseend_; };
		//戻り切ったかフラグを設定
		void SetReverseEnd(bool reverseend) { reverseend_ = reverseend; };
		//一度でも後退されたらtrueになるフラグ取得
		bool GetReverseStarted()const { return reverse_ || reverseend_; };

		//タイマーは起動しないが初期化する
		void Reset();
		//呼び出したらタイマーが進行
		void Start();
		//呼び出したらタイマーが後退
		void ReverseStart();
		//経過時間を更新
		void Update(const float elapseTimer = 1.0f);
		//タイマー更新+無限ループ
		void Roop(const float elapseTimer = 1.0f);
		//タイマー更新+無限ループ
		void RoopReverse(const float elapseTimer = 1.0f);

		//現在が最大秒数のうちどの割合かを(0.0f～1.0f)の値で返す
		float GetTimeRate();
	};
};