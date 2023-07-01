#include "NTexture.h"
#include "NVector4.h"
#include "NUtil.h"

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
	LoadTexture("Resources/start.png", "start");
}

NTexture NTextureManager::CreateErrorTexture()
{
	if (textureMap_["error"].texBuff_ == nullptr)
	{
		NTexture texture;	//���ʕ��ꎞ�ۑ��p

		//��ӂ̃s�N�Z����
		const size_t textureLen = 256;
		//�z��̗v�f��
		const size_t imageDataCount = textureLen * textureLen;
		//�摜�C���[�W�f�[�^�z��
		std::vector<NVector4> imageData;

		//�S�s�N�Z���̐F��������
		for (size_t i = 0; i < imageDataCount; i++)
		{
			size_t x = i % textureLen;
			size_t y = i / textureLen;

			//����ƉE�����s���N��
			if ((x < textureLen / 2 && y < textureLen / 2)
				|| (x >= textureLen / 2 && y >= textureLen / 2)) {
				imageData.emplace_back(NVector4(1, 0, 1, 1));
			}
			else {	//����ȊO�͍���
				imageData.emplace_back(NVector4(0, 0, 0, 1));
			}
		}

		//�e�N�X�`���o�b�t�@�ݒ�
		SetTBHeap();
		texResDesc_.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		texResDesc_.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texResDesc_.Width = textureLen;	//��
		texResDesc_.Height = textureLen;	//����
		texResDesc_.DepthOrArraySize = 1;
		texResDesc_.MipLevels = 1;
		texResDesc_.SampleDesc.Count = 1;

		HRESULT result;
		result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
			&texHeapProp_,
			D3D12_HEAP_FLAG_NONE,
			&texResDesc_,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&texture.texBuff_)
		);

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texture.texBuff_->WriteToSubresource(
			0,
			nullptr,	//�S�̈�փR�s�[
			imageData.data(),	//���f�[�^�A�h���X
			sizeof(NVector4) * textureLen,	//1���C���T�C�Y
			sizeof(NVector4) * imageDataCount	//�S�T�C�Y
		);

		//�V�F�[�_�[���\�[�X�r���[�ݒ�
		SetSRV();
		texture = CreateSRV(texture);

		//���O�o�^
		texture.fileName_ = "error";

		//�Ō�ɐ��ʕ����i�[
		textureMap_["error"] = texture;

		return texture;
	}

	return textureMap_["error"];
}

NTexture NTextureManager::LoadTexture(const std::string& pictureName, const std::string& handle)
{
	if (textureMap_[handle].texBuff_ == nullptr)
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
		texture.texBuff_ = CreateTexBuff();
		TexBuffMaping(texture.texBuff_.Get());
		//�V�F�[�_�[���\�[�X�r���[�ݒ�
		SetSRV();
		texture = CreateSRV(texture);

		//���O�o�^
		texture.fileName_ = handle;

		//�Ō�ɐ��ʕ����i�[
		textureMap_[handle] = texture;

		return texture;
	}
	return textureMap_[handle];
}

bool NTextureManager::Load(const std::string& pictureName)
{
	HRESULT result;
	//���ʂ̕�����^�ɕϊ�����
	std::wstring wPictureName(pictureName.begin(), pictureName.end());

	//�g���q�𒊏o���Ă���ɉ��������œǂݍ���
	std::string ext = NUtil::GetExtension(pictureName);
	//.psd�������ꍇ��.tga�ɕύX(�����t�H���_����.psd�Ɠ������O��.tga�t�@�C�����Ȃ��Ƃ���)
	if (ext == "psd")
	{
		wPictureName = NUtil::ReplaceExtension(wPictureName, "tga");
		ext = "tga";
	}

	if (ext == "tga")
	{
		result = LoadFromTGAFile(
			wPictureName.c_str(),	//�����ŕ����^��
			&metadata_, scratchImg_);
	}
	else /*if (ext == "png" || ext == "jpg")*/
	{
		result = LoadFromWICFile(
			wPictureName.c_str(),	//�����ŕ����^��
			WIC_FLAGS_NONE,
			&metadata_, scratchImg_);
	}

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
		scratchImg_.GetImages(), scratchImg_.GetImageCount(), scratchImg_.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain_);
	if (SUCCEEDED(result))
	{
		scratchImg_ = std::move(mipChain_);
		metadata_ = scratchImg_.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata_.format = MakeSRGB(metadata_.format);
}

void NTextureManager::SetTBHeap()
{
	texHeapProp_.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp_.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp_.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;	//�]����CPU��(L0)���璼�ڍs��
	texHeapProp_.CreationNodeMask = 0;							//�P��A�_�v�^�[������0
	texHeapProp_.VisibleNodeMask = 0;
}

void NTextureManager::SetTBResource()
{
	texResDesc_.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texResDesc_.Format = metadata_.format;

	texResDesc_.Width = metadata_.width;	//��
	texResDesc_.Height = (uint32_t)metadata_.height;	//����
	texResDesc_.DepthOrArraySize = (uint32_t)metadata_.arraySize;
	texResDesc_.MipLevels = (uint32_t)metadata_.mipLevels;
	texResDesc_.SampleDesc.Count = 1;
}

ID3D12Resource* NTextureManager::CreateTexBuff()
{
	HRESULT result;

	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&texHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tb_)
	);

	return tb_.Get();
}

void NTextureManager::TexBuffMaping(ID3D12Resource* texBuff)
{
	HRESULT result;

	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata_.mipLevels; i++)
	{
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg_.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(uint32_t)i,
			nullptr,				//�S�̈�փR�s�[
			img->pixels,			//���f�[�^�A�h���X
			(uint32_t)img->rowPitch,	//1���C���T�C�Y
			(uint32_t)img->slicePitch	//�S�T�C�Y
		);
		assert(SUCCEEDED(result));
	}
}

void NTextureManager::SetSRV()
{
	srvDesc_.Format = texResDesc_.Format;		//RGBA float
	srvDesc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc_.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc_.Texture2D.MipLevels = texResDesc_.MipLevels;
}

NTexture NTextureManager::CreateSRV(NTexture& tex)
{
	//SRV�q�[�v�̐擪�n���h�����擾
	tex.cpuHandle_ = NDX12::GetInstance()->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart();
	tex.gpuHandle_ = NDX12::GetInstance()->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
	//�w�肳�ꂽ�e�N�X�`���ԍ��ɉ����ăn���h����i�߂�
	uint32_t incrementSize = NDX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	tex.cpuHandle_.ptr += incrementSize * (uint32_t)textureMap_.size();
	tex.gpuHandle_.ptr += incrementSize * (uint32_t)textureMap_.size();

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	NDX12::GetInstance()->GetDevice()->CreateShaderResourceView(tex.texBuff_.Get(), &srvDesc_, tex.cpuHandle_);

	return tex;
}

NTexture::NTexture()
{
}

NTexture::~NTexture()
{
}
