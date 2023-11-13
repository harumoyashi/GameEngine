#pragma once
#include "Item.h"
#include <vector>

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
	static ItemManager* GetInstance();

	void Init();
	void Update();
	void Draw();

	//生成
	void Generate(const NVec3& pos, BulletType bulletType,bool isPop = true);
	//無敵アイテム取ったかフラグ設定
	void SetIsMutekiGet(bool isMutekiGet) { isMutekiGet_ = isMutekiGet; }

private:
	ItemManager() = default;
	~ItemManager() = default;
	ItemManager& operator=(const ItemManager&) = delete;
};