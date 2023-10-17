#include "Bloom.h"

void Bloom::Init()
{
	if (pipelineName_ != "Bloom")
	{
		//定数バッファ
		cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform2D>>();
		cbTrans_->Init();
		cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
		cbColor_->Init();

		isActive_ = true;
		CreateTexture();
		CreateRTV();
		CreateDepthBuff();
		CreateDSV();
		pipelineName_ = "Bloom";
	}
}
