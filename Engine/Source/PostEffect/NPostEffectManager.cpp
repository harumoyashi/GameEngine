#include "NPostEffectManager.h"
#include "NSceneManager.h"

NPostEffectManager* NPostEffectManager::GetInstance()
{
	static NPostEffectManager instance;
	return &instance;
}

void NPostEffectManager::Init()
{
	gaussian_ = std::make_unique<GaussianBlur>();
	radial_ = std::make_unique<RadialBlur>();
	bloom_ = std::make_unique<Bloom>();

	gaussian_->Init();
	radial_->Init();
	bloom_->Init();
}

void NPostEffectManager::Update()
{
	//それぞれアクティブ状態なら更新処理を行う
	/*if (gaussian_->GetIsActive())
	{
		gaussian_->Update();
	}
	if (radial_->GetIsActive())
	{
		radial_->Update();
	}
	if (bloom_->GetIsActive())
	{
		bloom_->Update();
	}*/

	gaussian_->Update();
	radial_->Update();
	bloom_->Update();
}

void NPostEffectManager::PreDraw()
{
	bloom_->PreDrawScene();
	NSceneManager::GetInstance()->Draw();
	bloom_->PostDrawScene();

	radial_->PreDrawScene();
	bloom_->Draw();
	radial_->PostDrawScene();
}

void NPostEffectManager::PostDraw()
{

}