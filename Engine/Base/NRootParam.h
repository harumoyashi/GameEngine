#pragma once
#include <vector>
#include "NDX12.h"

class NRootParam
{
public:
	std::vector<D3D12_ROOT_PARAMETER> entity_ = {};
	D3D12_DESCRIPTOR_RANGE descriptorRange_{};

public:
	~NRootParam();
	//デスクリプタレンジの設定
	void SetDescRange();
	void SetRootParam();
};

