#pragma once
#include "NWindows.h"
#include "NDX12.h"
#include "NGPipeline.h"
#include "NAudio.h"
#include "NTexture.h"
#include "NSceneManager.h"
#include "NAudioManager.h"

class NGame
{
private:
	NWindows* win;
	NDX12* dx12;
	NAudio* audio;
	PipeLineManager* pipeline;
	NTextureManager* texManager;
	NSceneManager* sceneMane;

	//ゲーム終了フラグ
	bool isGameEnd = false;

public:
	void Init();
	void Update();
	void Draw();
	void Finalize();

	//ゲーム終了フラグ取得
	inline bool GetIsGameEnd() { return isGameEnd; }
};