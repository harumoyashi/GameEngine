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
	//初期化
	void Init() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//終了処理
	void Finalize() override;
};