#pragma once
#include <memory>
#include "IBullet.h"

// 弾管理 //

class BulletManager final
{
public:
	std::vector<std::unique_ptr<IBullet>> bullets_;	//弾ども
	const uint32_t maxBul = 126;	//最大弾数

public:
	//シングルトンインスタンス取得
	static BulletManager* GetInstance();

	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

	//最初の要素削除
	void EraceBegin() { bullets_.erase(bullets_.begin()); }

	//満杯かどうか返す
	bool GetIsEmpty() { return bullets_.size() >= maxBul; }

private:
	//コンストラクタ
	BulletManager() = default;
	//コピー禁止
	BulletManager(const BulletManager&) = delete;
	//デストラクタ
	~BulletManager() = default;
	//コピー禁止
	BulletManager& operator=(const BulletManager&) = delete;
};