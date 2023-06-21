#include "NPostEffect.h"

NPostEffect::NPostEffect()
{
}

void NPostEffect::Draw()
{
	CommonBeginDraw();

	//��\���t���O�����Ă���`�悹���ɏ���������
	if (isInvisible_)
	{
		return;
	}

	//�n���h�����w��
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = NTextureManager::GetInstance()->textureMap_["error"].gpuHandle_;

	//�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBuff_.view_);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbTrans_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}
