#pragma once
#include "NWindows.h"
#include "NDX12.h"
#include "NGPipeline.h"
#include "NAudio.h"
#include "NTexture.h"
#include "NSceneManager.h"
#include "NAudioManager.h"
#include "NModelManager.h"
#include "NFramework.h"

// ゲームの流れまとめたフレームワーク //

class NGame final
	:public NFramework
{
private:
	NWindows* win_;
	NDX12* dx12_;
	NAudio* audio_;
	PipeLineManager* pipeline_;
	NTextureManager* texManager_;
	NSceneManager* sceneMane_;

public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};