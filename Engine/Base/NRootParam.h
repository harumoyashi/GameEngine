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
	//�f�X�N���v�^�����W�̐ݒ�
	void SetDescRange();
	void SetRootParam();
};

