#pragma once
#include "IEnemy.h"

// 敵管理 //

class EnemyManager final
{
private:
	bool isEnemyDead = false;

public:
	std::vector<std::unique_ptr<IEnemy>> enemys_;

	

	NEasing::EaseTimer beatInTimer_ = 0.25f;		//リズムに乗る用タイマー(でかくなる)
	NEasing::EaseTimer beatOutTimer_ = 0.25f;		//リズムに乗る用タイマー(縮む)

	//シングルトンインスタンス取得
	static EnemyManager* GetInstance();
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

private:
	//コンストラクタ
	EnemyManager() = default;
	//コピー禁止
	EnemyManager(const EnemyManager&) = delete;
	//デストラクタ
	~EnemyManager() = default;
	//コピー禁止
	EnemyManager& operator=(const EnemyManager&) = delete;
};