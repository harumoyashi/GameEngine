#pragma once
#include "Item.h"
#include <vector>

// アイテム管理 //

class ItemManager
{
private:
	std::vector<std::unique_ptr<Item>> items_;	//アイテム群
	std::unique_ptr<NObj3d> mutekiTex_;			//MUTEKIテキスト

	float mutekiTexPlusPos_;					//MUTEKIテキストに足す座標(動きつける用)
	NEasing::EaseTimer floatingTimer_ = 0.6f;	//ふよふよさせたりするためのタイマー(動きつける用)
	NEasing::EaseTimer scalingTimer_ = 0.2f;	//縮小タイマー(消える用)
	bool isMutekiGet_;							//無敵アイテム取ったか否か

public:
	//シングルトンインスタンス取得
	static ItemManager* GetInstance();
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

	//生成
	void Generate(const NVec3& pos, BulletType bulletType,bool isPop = true);

	//無敵アイテムの座標取得
	NVec3 GetMutekiItemPos();

	//無敵アイテム取ったかフラグ設定
	void SetIsMutekiGet(bool isMutekiGet) { isMutekiGet_ = isMutekiGet; }

private:
	//コンストラクタ
	ItemManager() = default;
	//デストラクタ
	~ItemManager() = default;
	//コピー禁止
	ItemManager& operator=(const ItemManager&) = delete;
};