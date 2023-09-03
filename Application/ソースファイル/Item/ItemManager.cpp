#include "ItemManager.h"

ItemManager* ItemManager::GetInstance()
{
	static ItemManager instance;
	return &instance;
}

void ItemManager::Init()
{
	items_.clear();
}

void ItemManager::Update()
{
	for (auto& item : items_)
	{
		item->Update();
	}
}

void ItemManager::Draw()
{
	for (auto& item : items_)
	{
		item->Draw();
	}
}

void ItemManager::Generate(const NVec3& pos, BulletType bulletType)
{
	items_.emplace_back();
	items_.back() = std::make_unique<Item>();
	items_.back()->Generate(pos);
	items_.back()->SetBulletType(bulletType);
}
