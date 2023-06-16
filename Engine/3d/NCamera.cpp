#include "NCamera.h"
#include "NWindows.h"
#include "NMathUtil.h"

std::unique_ptr<NCamera> NCamera::sNCamera = std::make_unique<NCamera>();
NCamera* NCamera::sCurrentCamera = sNCamera.get();

void NCamera::CreateMatView()
{
	matView_ = MathUtil::MatView(eye_, target_, up_);
}

void NCamera::ProjectiveProjection()
{
	matProjection_ = MathUtil::PerspectiveProjection(
		MathUtil::Degree2Radian(45.0f),
		(float)NWindows::kWin_width / NWindows::kWin_height,
		0.1f, 1000.0f);
}
