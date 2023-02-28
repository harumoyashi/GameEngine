#pragma once
#include "NWindows.h"
#include "NDX12.h"
#include "NGPipeline.h"
#include "NAudio.h"
#include "NTexture.h"
#include "NSceneManager.h"
#include "NAudioManager.h"
#include "NFramework.h"

class NGame :public NFramework
{
private:
	NWindows* win;
	NDX12* dx12;
	NAudio* audio;
	PipeLineManager* pipeline;
	NTextureManager* texManager;
	NSceneManager* sceneMane;

public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};