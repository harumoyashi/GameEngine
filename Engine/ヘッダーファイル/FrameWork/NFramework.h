#pragma once
class NFramework
{
private:
	//ゲーム終了フラグ
	bool isGameEnd_ = false;

public:
	virtual ~NFramework() = default;
	virtual void Init();
	virtual void Update();
	virtual void Draw() = 0;
	virtual void Finalize();

	//実行
	void Run();

	//ゲーム終了フラグ取得
	virtual bool GetIsGameEnd()const { return isGameEnd_; }
	//ゲーム終了フラグ設定
	virtual void SetIsGameEnd(bool flag) { isGameEnd_ = flag; }
};