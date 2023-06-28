#include "GaussianBlur.h"

void GaussianBlur::Init()
{
	CreateTexture();
	CreateRTV();
	CreateDepthBuff();
	CreateDSV();
	pipelineName_ = "Gaussian";
}
