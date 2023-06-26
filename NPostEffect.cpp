#include "NPostEffect.h"
#include "NWindows.h"
#include "NVector4.h"
#include "NMathUtil.h"

NPostEffect::NPostEffect()
{
}

void NPostEffect::Init()
{
	HRESULT result;

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		(UINT)NWindows::kWin_width,
		(UINT)NWindows::kWin_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	D3D12_HEAP_PROPERTIES texHeapProp;
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;	//�]����CPU��(L0)���璼�ڍs��
	texHeapProp.CreationNodeMask = 0;							//�P��A�_�v�^�[������0
	texHeapProp.VisibleNodeMask = 0;

	//�e�N�X�`���o�b�t�@�̐���
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr, IID_PPV_ARGS(&texBuff_)
	);
	assert(SUCCEEDED(result));

	//------------------------------�e�N�X�`������--------------------------------//
	//��ӂ̃s�N�Z����
	const uint32_t rowPitch = sizeof(NVector4) * NWindows::kWin_width;
	const uint32_t depthPitch = rowPitch * NWindows::kWin_height;
	//�z��̗v�f��
	const uint32_t imageDataCount = NWindows::kWin_width * NWindows::kWin_height;
	//�摜�C���[�W�f�[�^�z��
	std::vector<NVector4> imageData;

	//�S�s�N�Z���̐F��������
	for (size_t i = 0; i < imageDataCount; i++)
	{
		imageData.emplace_back(NVector4(1, 0, 1, 1));
	}

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuff_->WriteToSubresource(
		0,
		nullptr,	//�S�̈�փR�s�[
		imageData.data(),	//���f�[�^�A�h���X
		rowPitch,	//1���C���T�C�Y
		depthPitch	//�S�T�C�Y
	);

	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = NDX12::GetInstance()->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV�쐬
	NDX12::GetInstance()->GetDevice()->CreateShaderResourceView(
		texBuff_.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);

	// ---------------------------���_�֘A----------------------------- //
	//���_���i�[�p
	std::vector<NVertexUV> vert(4);

	//�A���J�[�|�C���g�ݒ�
	float left = 0.0f * texresDesc.Width;
	float right = 1.0f * texresDesc.Width;
	float top = 0.0f * texresDesc.Height;
	float bottom = 1.0f * texresDesc.Height;

	//�ݒ肵���A���J�[�|�C���g�ɍ��킹�Ē��_�ݒ肵�Ȃ���
	vert[0].pos = { left ,bottom,0.0f };	// ����
	vert[1].pos = { left ,top   ,0.0f };	// ����
	vert[2].pos = { right,bottom,0.0f };	// �E��
	vert[3].pos = { right,top   ,0.0f };	// �E��

	//�e�N�X�`���T�C�Y�����Ƃɐ؂��镔����uv���v�Z
	float tex_left = texLeftTop_.x / texresDesc.Width;
	float tex_right = (texLeftTop_.x + texSize_.x) / texresDesc.Width;
	float tex_top = texLeftTop_.y / texresDesc.Height;
	float tex_bottom = (texLeftTop_.y + texSize_.y) / texresDesc.Height;

	//�v�Z����uv�ɍ��킹�Đݒ�
	vert[0].uv = { tex_left ,tex_bottom };	// ����
	vert[1].uv = { tex_left ,tex_top };	// ����
	vert[2].uv = { tex_right,tex_bottom };	// �E��
	vert[3].uv = { tex_right,tex_top };	// �E��

	//���_�o�b�t�@�ւ̃f�[�^�]��
	vertexBuff_.Init(vert);

	//���s���e����
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::kWin_width),
		static_cast<float>(NWindows::kWin_height)
	);
	Update();
}

void NPostEffect::Draw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSetSprite().pipelineState_.Get());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSetSprite().rootSig_.entity_.Get());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`�X�g���b�v

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { descHeapSRV.Get() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());

	//�|�X�g�G�t�F�N�g�p�ɍ����SRV�����[�g�p�����[�^1�Ԃɐݒ�
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBuff_.view_);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbTrans_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}
