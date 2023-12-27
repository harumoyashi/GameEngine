#pragma once

// フレームワーク //

class NFramework
{
private:
	//ゲーム終了フラグ
	bool isGameEnd_ = false;

public:
	virtual ~NFramework() = default;
	//初期化
	virtual void Init();
	//更新
	virtual void Update();
	//描画
	virtual void Draw() = 0;
	//終了処理
	virtual void Finalize();

	//実行
	void Run();

	//ゲーム終了フラグ取得
	virtual bool GetIsGameEnd()const { return isGameEnd_; }
	//ゲーム終了フラグ設定
	virtual void SetIsGameEnd(bool flag) { isGameEnd_ = flag; }
};