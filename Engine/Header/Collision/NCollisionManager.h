#pragma once
#include <forward_list>

// 当たり判定管理 //
// 当たり判定登録して総当たりさせる //

class NBaseCollider;

class NCollisionManager final
{
private:
	std::forward_list<NBaseCollider*>colliders_;

	NBaseCollider* colA_;
	NBaseCollider* colB_;

	bool isCollisionCheck = true;	//当たり判定するかフラグ

public:
	//シングルトンインスタンス取得
	static NCollisionManager* GetInstance();

	//初期化(コライダー群全削除)
	void Init();

	//コライダーをリストに追加
	void AddCollider(NBaseCollider* collider)
	{
		colliders_.emplace_front(collider);
	}
	//コライダーをリストから削除
	void RemoveCollider(NBaseCollider* collider)
	{
		colliders_.remove(collider);
	}

	//全ての衝突チェック
	void CheckAllCollision();

private:
	//コンストラクタ
	NCollisionManager() = default;
	//コピー禁止
	NCollisionManager(const NCollisionManager&) = delete;
	//デストラクタ
	~NCollisionManager() = default;
	//コピー禁止
	NCollisionManager& operator=(const NCollisionManager&) = delete;

	//円同士の当たり判定
	void CircleCol();
	//矩形同士の当たり判定
	void SquareCol();
	//矩形と円の当たり判定
	void Square2CircleCol();
	//球体同士の当たり判定
	void SphereCol();
	//球体と平面の当たり判定
	void Sphere2PlaneCol();
};