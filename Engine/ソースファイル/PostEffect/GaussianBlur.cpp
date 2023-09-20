#include "GaussianBlur.h"

void GaussianBlur::Init()
{
	if (pipelineName_ != "Gaussian")
	{
		//�萔�o�b�t�@
		cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform2D>>();
		cbTrans_->Init();
		cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
		cbColor_->Init();

		isActive_ = true;
		CreateTexture();
		CreateRTV();
		CreateDepthBuff();
		CreateDSV();
		pipelineName_ = "Gaussian";
	}
}
