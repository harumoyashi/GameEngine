#pragma once
#include <vector>
#include "NDX12.h"

class NRootParam
{
public:
	std::vector<D3D12_ROOT_PARAMETER> entity_ = {};
private:
	std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRange_{};

public:
	~NRootParam();
	//�f�X�N���v�^�����W�̐ݒ�
	void SetDescRange(const uint32_t texNum);
	void SetRootParam(const uint32_t texNum, const uint32_t constantNum);
};

