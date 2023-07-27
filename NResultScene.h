#pragma once
#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NAudio.h"
#include "NCollision.h"

class NResultScene final
{
private:
	//オブジェクト
	NMaterial material_;				//マテリアル
	static const uint32_t kMaxObj = 3;	//オブジェクト数
	std::vector<std::unique_ptr<NObj3d>> obj_;			//オブジェクト(定数バッファ)

	bool isCol_ = false;

	//背景スプライト
	static const uint32_t kMaxBackSprite = 3;	//背景スプライト数
	std::unique_ptr<NSprite> backSprite_[kMaxBackSprite];	//背景スプライト

	//前景スプライト
	static const uint32_t kMaxForeSprite = 3;	//前景スプライト数
	std::unique_ptr<NSprite> foreSprite_[kMaxForeSprite];	//前景スプライト
	
	//ライトたち
	std::unique_ptr<NLightGroup> lightGroup_;

public:
	void Init();
	void Update();
	void Draw();
	void Reset();
	void Finalize();
};