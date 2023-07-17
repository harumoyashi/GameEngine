#include "NSceneManager.h"

#include "NTitleScene.h"
#include "NGameScene.h"
#include "NSprite.h"
#include "NObj3d.h"
#include "NParticle3D.h"
#include "NSceneChange.h"

#pragma region staticメンバ変数初期化

//シーン変更フラグの初期化
std::unique_ptr<IScene> NSceneManager::currentScene_;
std::unique_ptr<IScene> NSceneManager::nextScene_;

NSceneManager::NSceneManager()
{
	currentScene_ = std::move(std::make_unique<NGameScene>());
}

#pragma region
NSceneManager* NSceneManager::GetInstance()
{
	static NSceneManager instance;
	return &instance;
}

void NSceneManager::Init()
{
	currentScene_->Init();
	NSceneChange::GetInstance()->Init();
}

void NSceneManager::Update()
{
	currentScene_->Update();
	NSceneChange::GetInstance()->Update();
}

void NSceneManager::Draw()
{
	NObj3d::CommonBeginDraw();
	currentScene_->DrawBack3D();
	NSprite::CommonBeginDraw();
	currentScene_->DrawBackSprite();
	NObj3d::CommonBeginDraw();
	currentScene_->Draw3D();
	IEmitter3D::CommonBeginDraw();
	currentScene_->DrawParticle();
	NSprite::CommonBeginDraw();
	currentScene_->DrawForeSprite();
	NSceneChange::GetInstance()->Draw();
}
