#include "NPostEffect.h"
#include "NWindows.h"
#include "NVector4.h"
#include "NMathUtil.h"
#include "NInput.h"

const float NPostEffect::kClearColor[4] = { 0.25f,0.5f,0.1f,0.0f };

NPostEffect::NPostEffect()
{
}

void NPostEffect::Init()
{
	CreateTexture();
	CreateRTV();
	CreateDepthBuff();
	CreateDSV();
}

void NPostEffect::TexChange()
{
	//SRV�ݒ�
	if (NInput::GetInstance()->IsKeyDown(DIK_0))
	{
		//�e�N�X�`���ԍ���01�Ő؂�ւ�
		static uint32_t texNum = 0;
		texNum = (texNum + 1) % 2;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
		srvDesc.Texture2D.MipLevels = 1;

		//�f�X�N���v�^�q�[�v��SRV�쐬
		NDX12::GetInstance()->GetDevice()->CreateShaderResourceView(
			texBuff_[texNum].Get(),
			&srvDesc,
			descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
		);
	}
}

void NPostEffect::Draw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSetPostEffect().pipelineState_.Get());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSetPostEffect().rootSig_.entity_.Get());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`�X�g���b�v

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { descHeapSRV_.Get() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());

	//�|�X�g�G�t�F�N�g�p�ɍ����SRV�����[�g�p�����[�^1�Ԃɐݒ�
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV_->GetGPUDescriptorHandleForHeapStart());

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBuff_.view_);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbTrans_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	NDX12::GetInstance()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}

void NPostEffect::CreateTexture()
{
	HRESULT result;

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
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
	CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kClearColor);

	for (uint32_t i = 0; i < 2; i++)
	{
		result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(&texBuff_[i])
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
		for (size_t j = 0; j < imageDataCount; j++)
		{
			imageData.emplace_back(NVector4(1, 0, 1, 1));
		}

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff_[i]->WriteToSubresource(
			0,
			nullptr,	//�S�̈�փR�s�[
			imageData.data(),	//���f�[�^�A�h���X
			rowPitch,	//1���C���T�C�Y
			depthPitch	//�S�T�C�Y
		);
	}

	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = NDX12::GetInstance()->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV_));
	assert(SUCCEEDED(result));

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV�쐬
	NDX12::GetInstance()->GetDevice()->CreateShaderResourceView(
		texBuff_[0].Get(),
		&srvDesc,
		descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
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
	//vert[0].uv = { tex_left ,tex_bottom };	// ����
	//vert[1].uv = { tex_left ,tex_top };	// ����
	//vert[2].uv = { tex_right,tex_bottom };	// �E��
	//vert[3].uv = { tex_right,tex_top };	// �E��

	vert[0].uv = { 0 ,1 };	// ����
	vert[1].uv = { 0 ,0 };	// ����
	vert[2].uv = { 1,1 };	// �E��
	vert[3].uv = { 1,0 };	// �E��

	//���_�o�b�t�@�ւ̃f�[�^�]��
	vertexBuff_.Init(vert);

	//���s���e����
	matProjection_ = cbTrans_->constMap_->mat = MathUtil::ParallelProjection(
		static_cast<float>(NWindows::kWin_width),
		static_cast<float>(NWindows::kWin_height)
	);
	Update();
}

void NPostEffect::CreateRTV()
{
	HRESULT result;

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;
	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = NDX12::GetInstance()->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
	assert(SUCCEEDED(result));

	//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//�f�X�N���v�^�q�[�v��RTV�쐬
	for (uint32_t i = 0; i < 2; i++)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle(
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), i,
				NDX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));

		NDX12::GetInstance()->GetDevice()->CreateRenderTargetView(
			texBuff_[i].Get(),
			nullptr,
			cpuHandle
		);
	}
}

void NPostEffect::CreateDepthBuff()
{
	HRESULT result;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResDesc{};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = NWindows::kWin_width;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.Height = NWindows::kWin_height;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//�f�v�X�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//�[�x�o�b�t�@����
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_)
	);
	assert(SUCCEEDED(result));
}

void NPostEffect::CreateDSV()
{
	HRESULT result;

	//DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;	//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//�f�v�X�X�e���V���r���[
	//DSV�p�f�X�N���v�^�q�[�v�쐬
	result = NDX12::GetInstance()->GetDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�q�[�v��DSV�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	NDX12::GetInstance()->GetDevice()->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc, descHeapDSV_->GetCPUDescriptorHandleForHeapStart()
	);
}

void NPostEffect::PreDrawScene()
{
	// ------------------���\�[�X�o���A���������ݐ�p��ԂɕύX----------------------- //
	D3D12_RESOURCE_BARRIER barrierDesc{};
	for (uint32_t i = 0; i < 2; i++)
	{
		barrierDesc.Transition.pResource = texBuff_[i].Get();
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;	//�ǂݍ��ݐ�p��Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;				//�������ݐ�p��Ԃ�
		NDX12::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrierDesc);
	}

	//�����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle[2];
	for (uint32_t i = 0; i < 2; i++)
	{
		rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV_->GetCPUDescriptorHandleForHeapStart(),
			NDX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}
	//�f�v�X�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�e�N�X�`���������_�[�^�[�Q�b�g�Ɏw��
	NDX12::GetInstance()->GetCommandList()->OMSetRenderTargets(2, rtvHandle, false, &dsvHandle);

	// -----------------------���̑��������-------------------------- //
	//�r���[�|�[�g�̐ݒ�
	CD3DX12_VIEWPORT viewport[2];
	for (uint32_t i = 0; i < 2; i++)
	{
		viewport[i] = CD3DX12_VIEWPORT(0.0f, 0.0f,
			NWindows::kWin_width, NWindows::kWin_height);
	}

	NDX12::GetInstance()->GetCommandList()->RSSetViewports(2, viewport);

	//�V�U�����O��`�̐ݒ�
	CD3DX12_RECT rect[2];
	for (uint32_t i = 0; i < 2; i++)
	{
		rect[i] = CD3DX12_RECT(0, 0,
			NWindows::kWin_width, NWindows::kWin_height);
	}

	NDX12::GetInstance()->GetCommandList()->RSSetScissorRects(2, rect);

	//�S��ʃN���A
	for (uint32_t i = 0; i < 2; i++)
	{
		NDX12::GetInstance()->GetCommandList()->ClearRenderTargetView(rtvHandle[i], kClearColor, 0, nullptr);
	}
	//�[�x�o�b�t�@�̃N���A
	NDX12::GetInstance()->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void NPostEffect::PostDrawScene()
{
	//���\�[�X�o���A��ύX(�������ݐ�p��Ԃ���ǂݎ���p��Ԃ�)
	D3D12_RESOURCE_BARRIER barrierDesc{};
	for (uint32_t i = 0; i < 2; i++)
	{
		barrierDesc.Transition.pResource = texBuff_[i].Get();
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;		//�������ݐ�p��Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;	//�ǂݍ��ݐ�p��Ԃ�
		NDX12::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrierDesc);
	}
}
