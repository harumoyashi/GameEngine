#include "RadialBlur.h"

void RadialBlur::Init()
{
	CreateTexture();
	CreateRTV();
	CreateDepthBuff();
	CreateDSV();
	pipelineName_ = "Radial";
}