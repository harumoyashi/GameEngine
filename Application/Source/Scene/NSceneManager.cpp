#include "NSceneManager.h"

#include "NTestScene.h"
#include "NTitleScene.h"
#include "NGameScene.h"
#include "NSprite.h"
#include "NObj3d.h"
#include "NParticle3D.h"
#include "NSceneChange.h"
#include "UI.h"
#include "NAudioManager.h"

#pragma region staticメンバ変数初期化

//シーン変更フラグの初期化
std::unique_ptr<IScene> NSceneManager::currentScene_;
std::unique_ptr<IScene> NSceneManager::nextScene_;

NSceneManager::NSceneManager()
{
	currentScene_ = std::move(std::make_unique<NTitleScene>());
}

#pragma region
NSceneManager* NSceneManager::GetInstance()
{
	static NSceneManager instance;
	return &instance;
}

void NSceneManager::Init()
{
	UIManager::GetInstance()->Init();
	NAudio::GetInstance()->Init();
	currentScene_->Init();
	NSceneChange::GetInstance()->Init();
}

void NSceneManager::Update()
{
	currentScene_->Update();
	NSceneChange::GetInstance()->Update();
	UIManager::GetInstance()->Update();
}

void NSceneManager::Draw()
{
	NSprite::SetBlendMode(BlendMode::Alpha);
	NSprite::CommonBeginDraw();
	currentScene_->DrawBackSprite();
	NObj3d::CommonBeginDraw();
	currentScene_->DrawBack3D();
	NObj3d::CommonBeginDraw();
	currentScene_->Draw3D();
	IEmitter3D::SetBlendMode(BlendMode::None);
	IEmitter3D::CommonBeginDraw();
	currentScene_->DrawParticle();
	NSprite::SetBlendMode(BlendMode::Alpha);
	NSprite::CommonBeginDraw();
	currentScene_->DrawForeSprite();
	NSceneChange::GetInstance()->Draw();
}
