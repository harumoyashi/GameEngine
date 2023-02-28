#pragma once
class NFramework
{
private:
	//ゲーム終了フラグ
	bool isGameEnd = false;

public:
	virtual ~NFramework() = default;
	virtual void Init();
	virtual void Update();
	virtual void Draw() = 0;
	virtual void Finalize();

	//実行
	void Run();

	//ゲーム終了フラグ取得
	virtual inline bool GetIsGameEnd() { return isGameEnd; }
	//ゲーム終了フラグ設定
	virtual inline bool SetIsGameEnd(bool flag) { isGameEnd = flag; }
};