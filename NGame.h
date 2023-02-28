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

	//�Q�[���I���t���O
	bool isGameEnd = false;

public:
	void Init();
	void Update();
	void Draw();
	void Finalize();

	//�Q�[���I���t���O�擾
	inline bool GetIsGameEnd() { return isGameEnd; }
};