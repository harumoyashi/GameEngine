#pragma once
#include "Item.h"
#include <vector>

class ItemManager
{
private:
	std::vector< std::unique_ptr<Item>> items_;	//ƒAƒCƒeƒ€ŒQ

public:
	static ItemManager* GetInstance();

	void Init();
	void Update();
	void Draw();

	void Generate(const NVec3& pos, const float moveAngle = 0.0f);

private:
	ItemManager() = default;
	~ItemManager() = default;
	ItemManager& operator=(const ItemManager&) = delete;
};