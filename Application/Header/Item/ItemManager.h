#pragma once
#include "Item.h"
#include <vector>

class ItemManager
{
private:
	std::vector< std::unique_ptr<Item>> items_;	//アイテム群

public:
	static ItemManager* GetInstance();

	void Init();
	void Update();
	void Draw();

	//生成
	void Generate(const NVec3& pos, BulletType bulletType);

private:
	ItemManager() = default;
	~ItemManager() = default;
	ItemManager& operator=(const ItemManager&) = delete;
};