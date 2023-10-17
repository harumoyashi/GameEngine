#include "RadialBlur.h"

void RadialBlur::Init()
{
	if (pipelineName_ != "Radial")
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
		pipelineName_ = "Radial";
	}
}