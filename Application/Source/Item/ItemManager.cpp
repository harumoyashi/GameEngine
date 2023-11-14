#include "ItemManager.h"

ItemManager* ItemManager::GetInstance()
{
	static ItemManager instance;
	return &instance;
}

void ItemManager::Init()
{
	items_.clear();
	mutekiTex_ = std::make_unique<NObj3d>();
	mutekiTex_->scale_ = NVec3(1.f, 0.01f, 0.3f);
	mutekiTex_->SetModel("plane");
	mutekiTex_->SetTexture("muteki");

	isMutekiGet_ = false;
	floatingTimer_.Reset();
	scalingTimer_.Reset();
}

void ItemManager::Update()
{
	for (auto& item : items_)
	{
		item->Update();

		if (item->GetBulletType() == BulletType::MaxType)
		{
			//無敵アイテムテキストふよふよ
			floatingTimer_.RoopReverse();
			if (floatingTimer_.GetRun())
			{
				mutekiTexPlusPos_ = -NEasing::OutBounce(floatingTimer_.GetTimeRate()) * 0.5f + 1.2f;
			}
			else
			{
				mutekiTexPlusPos_ = -NEasing::InQuad(floatingTimer_.GetTimeRate()) * 0.5f + 1.2f;
			}

			mutekiTex_->position_ = item->GetPos() + NVec3(0, 0, mutekiTexPlusPos_);

			//無敵アイテム取ったらテキストを縮小して消す
			if (isMutekiGet_)
			{
				scalingTimer_.Update();
				if (scalingTimer_.GetStarted() == false)
				{
					scalingTimer_.Start();
				}

				mutekiTex_->scale_.x = 1.f - NEasing::InQuad(scalingTimer_.GetTimeRate());
			}

			mutekiTex_->Update();
		}
	}
}

void ItemManager::Draw()
{
	for (auto& item : items_)
	{
		item->Draw();
	}

	mutekiTex_->SetBlendMode(BlendMode::Alpha);
	mutekiTex_->Draw();
	mutekiTex_->SetBlendMode(BlendMode::Alpha);
}

void ItemManager::Generate(const NVec3& pos, BulletType bulletType, bool isPop)
{
	items_.emplace_back();
	items_.back() = std::make_unique<Item>();
	items_.back()->Generate(pos, isPop);
	items_.back()->SetBulletType(bulletType);
}
