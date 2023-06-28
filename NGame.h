#pragma once
#include "NWindows.h"
#include "NDX12.h"
#include "NGPipeline.h"
#include "NAudio.h"
#include "NTexture.h"
#include "NSceneManager.h"
#include "NAudioManager.h"
#include "NPostEffect.h"
#include "NFramework.h"

class NGame :public NFramework
{
private:
	NWindows* win_;
	NDX12* dx12_;
	NAudio* audio_;
	PipeLineManager* pipeline_;
	NTextureManager* texManager_;
	NSceneManager* sceneMane_;
	std::unique_ptr<NPostEffect> postEffect_;
	bool isPostEffect_;	//ポストエフェクト適用するか

public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};