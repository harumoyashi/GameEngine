#include "NCamera.h"
#include "NWindows.h"
#include "NMathUtil.h"

std::unique_ptr<NCamera> NCamera::_nCamera = std::make_unique<NCamera>();
NCamera* NCamera::nowCamera = _nCamera.get();

void NCamera::CreateMatView()
{
	matView = MathUtil::MatView(eye, target, up);
}

void NCamera::ProjectiveProjection()
{
	matProjection = MathUtil::PerspectiveProjection(
		MathUtil::Degree2Radian(45.0f),
		(float)NWindows::win_width / NWindows::win_height,
		0.1f, 1000.0f);
}
