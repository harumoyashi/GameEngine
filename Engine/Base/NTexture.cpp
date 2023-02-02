#include "NTexture.h"

NTextureManager* NTextureManager::GetInstance()
{
	static NTextureManager instance;
	return &instance;
}

void NTextureManager::Init()
{
	CreateErrorTexture();
	//�����ňꊇ�ǂݍ���
	LoadTexture("Resources/white64x64.png", "white");
	LoadTexture("Resources/itiro_kimegao.png", "itiro");
	LoadTexture("Resources/hamutaro.jpg", "hamu");
	LoadTexture("Resources/mario.jpg", "mario");
}

NTexture NTextureManager::CreateErrorTexture()
{
	if (textureMap["error"].texBuff == nullptr)
	{
		NTexture texture;	//���ʕ��ꎞ�ۑ��p

		//��ӂ̃s�N�Z����
		const size_t textureLen = 256;
		//�z��̗v�f��
		const size_t imageDataCount = textureLen * textureLen;
		//�摜�C���[�W�f�[�^�z��
		XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];	//���K����ŉ������

		//�S�s�N�Z���̐F��������
		for (size_t i = 0; i < imageDataCount; i++)
		{
			size_t x = i % textureLen;
			size_t y = i / textureLen;

			//����ƉE�����s���N��
			if ((x < textureLen / 2 && y < textureLen / 2)
				|| (x >= textureLen / 2 && y >= textureLen / 2)) {
				imageData[i] = { 1, 0, 1, 1 };
			}
			else {	//����ȊO�͍���
				imageData[i] = { 0, 0, 0, 1 };
			}
		}

		//�e�N�X�`���o�b�t�@�ݒ�
		SetTBHeap();
		texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		texResDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texResDesc.Width = textureLen;	//��
		texResDesc.Height = textureLen;	//����
		texResDesc.DepthOrArraySize = 1;
		texResDesc.MipLevels = 1;
		texResDesc.SampleDesc.Count = 1;

		HRESULT result;
		result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texResDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&texture.texBuff)
		);

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texture.texBuff->WriteToSubresource(
			0,
			nullptr,	//�S�̈�փR�s�[
			imageData,	//���f�[�^�A�h���X
			sizeof(XMFLOAT4) * textureLen,	//1���C���T�C�Y
			sizeof(XMFLOAT4) * imageDataCount	//�S�T�C�Y
		);

		//���f�[�^���
		delete[] imageData;


		//�V�F�[�_�[���\�[�X�r���[�ݒ�
		SetSRV();
		texture = CreateSRV(texture);

		//���O�o�^
		texture.fileName = "error";

		//�Ō�ɐ��ʕ����i�[
		textureMap["error"] = texture;

		return texture;
	}

	return textureMap["error"];
}

NTexture NTextureManager::LoadTexture(const std::string pictureName, const std::string handle)
{
	if (textureMap[handle].texBuff == nullptr)
	{
		NTexture texture;	//���ʕ��ꎞ�ۑ��p

		if (!Load(pictureName))
		{
			return CreateErrorTexture();
		}

		CreateMipmap();
		//�e�N�X�`���o�b�t�@�ݒ�
		SetTBHeap();
		SetTBResource();
		texture.texBuff = CreateTexBuff();
		TexBuffMaping(texture.texBuff.Get());
		//�V�F�[�_�[���\�[�X�r���[�ݒ�
		SetSRV();
		texture = CreateSRV(texture);

		//���O�o�^
		texture.fileName = handle;

		//�Ō�ɐ��ʕ����i�[
		textureMap[handle] = texture;

		return texture;
	}
	return textureMap[handle];
}

bool NTextureManager::Load(const std::string pictureName)
{
	HRESULT result;
	//���ʂ̕�����^�ɕϊ�����
	std::wstring wPictureName(pictureName.begin(), pictureName.end());

	result = LoadFromWICFile(
		wPictureName.c_str(),	//�����ŕ����^��
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (result != S_OK)
	{
		return false;
	}
	return true;
}

void NTextureManager::CreateMipmap()
{
	HRESULT result;

	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);
}

void NTextureManager::SetTBHeap()
{
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;	//�]����CPU��(L0)���璼�ڍs��
	texHeapProp.CreationNodeMask = 0;							//�P��A�_�v�^�[������0
	texHeapProp.VisibleNodeMask = 0;
}

void NTextureManager::SetTBResource()
{
	texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texResDesc.Format = metadata.format;

	texResDesc.Width = metadata.width;	//��
	texResDesc.Height = (UINT)metadata.height;	//����
	texResDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texResDesc.MipLevels = (UINT16)metadata.mipLevels;
	texResDesc.SampleDesc.Count = 1;
}

ID3D12Resource* NTextureManager::CreateTexBuff()
{
	HRESULT result;

	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tb)
	);

	return tb.Get();
}

void NTextureManager::TexBuffMaping(ID3D12Resource* texBuff)
{
	HRESULT result;

	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,				//�S�̈�փR�s�[
			img->pixels,			//���f�[�^�A�h���X
			(UINT)img->rowPitch,	//1���C���T�C�Y
			(UINT)img->slicePitch	//�S�T�C�Y
		);
		assert(SUCCEEDED(result));
	}
}

void NTextureManager::SetSRV()
{
	srvDesc.Format = texResDesc.Format;		//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = texResDesc.MipLevels;
}

NTexture NTextureManager::CreateSRV(NTexture tex)
{
	//SRV�q�[�v�̐擪�n���h�����擾
	tex.cpuHandle = NDX12::GetInstance()->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart();
	tex.gpuHandle = NDX12::GetInstance()->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
	//�w�肳�ꂽ�e�N�X�`���ԍ��ɉ����ăn���h����i�߂�
	UINT incrementSize = NDX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	tex.cpuHandle.ptr += incrementSize * (UINT)textureMap.size();
	tex.gpuHandle.ptr += incrementSize * (UINT)textureMap.size();

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	NDX12::GetInstance()->GetDevice()->CreateShaderResourceView(tex.texBuff.Get(), &srvDesc, tex.cpuHandle);

	return tex;
}

NTexture::NTexture()
{
}

NTexture::~NTexture()
{
}
