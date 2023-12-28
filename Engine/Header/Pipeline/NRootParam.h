#pragma once
#include <vector>
#include "NDX12.h"

// ルートパラメータ //

class NRootParam final
{
public:
	std::vector<D3D12_ROOT_PARAMETER> entity_ = {};
private:
	std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRange_{};

public:
	//デストラクタ
	~NRootParam();
	//ルートパラメーターの設定
	void SetRootParam(const uint32_t texNum, const uint32_t constantNum);
};

