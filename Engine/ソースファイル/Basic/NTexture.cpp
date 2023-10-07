#include "NTexture.h"
#include "NVec4.h"
#include "NUtil.h"

NTextureManager* NTextureManager::GetInstance()
{
	static NTextureManager instance;
	return &instance;
}

void NTextureManager::Init()
{
	std::string directryPass = "Resources/Texture/";

	CreateErrorTexture();
	//ここで一括読み込み
	LoadTexture(directryPass + "white64x64.png", "white");
	LoadTexture(directryPass + "start.png", "start");
	LoadTexture(directryPass + "goal.png", "goal");
	LoadTexture(directryPass + "timerocker_tile.png", "tile");
	LoadTexture(directryPass + "title_logo.png", "logo");
	LoadTexture(directryPass + "Abutton_UI.png", "Abutton");
	LoadTexture(directryPass + "Bbutton_UI.png", "Bbutton");
	LoadTexture(directryPass + "Menubutton_UI.png", "Menubutton");
	LoadTexture(directryPass + "shaft.png", "shaft");
	LoadTexture(directryPass + "stick.png", "stick");
	LoadTexture(directryPass + "checkpoint.png", "checkpoint");
	LoadTexture(directryPass + "clear.png", "clear");
	LoadTexture(directryPass + "faild.png", "faild");
	LoadTexture(directryPass + "number.png", "number");
	LoadTexture(directryPass + "top.png", "top");
	LoadTexture(directryPass + "item.png", "item");
	LoadTexture(directryPass + "wave_UI.png", "waveUI");
	LoadTexture(directryPass + "meter.png", "meter");
	LoadTexture(directryPass + "line_UI.png", "lineUI");
	LoadTexture(directryPass + "sideshot_UI.png", "sideUI");
	LoadTexture(directryPass + "wideshot_UI.png", "wideUI");
}

NTexture NTextureManager::CreateErrorTexture()
{
	if (textureMap_["error"].texBuff_ == nullptr)
	{
		NTexture texture;	//成果物一時保存用

		//一辺のピクセル数
		const size_t textureLen = 256;
		//配列の要素数
		const size_t imageDataCount = textureLen * textureLen;
		//画像イメージデータ配列
		std::vector<NVec4> imageData;

		//全ピクセルの色を初期化
		for (size_t i = 0; i < imageDataCount; i++)
		{
			size_t x = i % textureLen;
			size_t y = i / textureLen;

			//左上と右下をピンクに
			if ((x < textureLen / 2 && y < textureLen / 2)
				|| (x >= textureLen / 2 && y >= textureLen / 2)) {
				imageData.emplace_back(NVec4(1, 0, 1, 1));
			}
			else {	//それ以外は黒に
				imageData.emplace_back(NVec4(0, 0, 0, 1));
			}
		}

		//テクスチャバッファ設定
		SetTBHeap();
		texResDesc_.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		texResDesc_.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texResDesc_.Width = textureLen;	//幅
		texResDesc_.Height = textureLen;	//高さ
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

		//テクスチャバッファにデータ転送
		result = texture.texBuff_->WriteToSubresource(
			0,
			nullptr,	//全領域へコピー
			imageData.data(),	//元データアドレス
			sizeof(NVec4) * textureLen,	//1ラインサイズ
			sizeof(NVec4) * imageDataCount	//全サイズ
		);

		//シェーダーリソースビュー設定
		SetSRV();
		texture = CreateSRV(texture);

		//名前登録
		texture.fileName_ = "error";

		//最後に成果物を格納
		textureMap_["error"] = texture;

		return texture;
	}

	return textureMap_["error"];
}

NTexture NTextureManager::LoadTexture(const std::string& pictureName, const std::string& handle)
{
	if (textureMap_[handle].texBuff_ == nullptr)
	{
		NTexture texture;	//成果物一時保存用

		if (!Load(pictureName))
		{
			return CreateErrorTexture();
		}

		CreateMipmap();
		//テクスチャバッファ設定
		SetTBHeap();
		SetTBResource();
		texture.texBuff_ = CreateTexBuff();
		TexBuffMaping(texture.texBuff_.Get());
		//シェーダーリソースビュー設定
		SetSRV();
		texture = CreateSRV(texture);

		//名前登録
		texture.fileName_ = handle;

		//最後に成果物を格納
		textureMap_[handle] = texture;

		return texture;
	}
	return textureMap_[handle];
}

bool NTextureManager::Load(const std::string& pictureName)
{
	HRESULT result;
	//一回別の文字列型に変換して
	std::wstring wPictureName(pictureName.begin(), pictureName.end());

	//拡張子を抽出してそれに応じた方で読み込み
	std::string ext = NUtil::GetExtension(pictureName);
	//.psdだった場合は.tgaに変更(同じフォルダ内に.psdと同じ名前の.tgaファイルがないとだめ)
	if (ext == "psd")
	{
		wPictureName = NUtil::ReplaceExtension(wPictureName, "tga");
		ext = "tga";
	}

	if (ext == "tga")
	{
		result = LoadFromTGAFile(
			wPictureName.c_str(),	//ここで文字型に
			&metadata_, scratchImg_);
	}
	else
	{
		result = LoadFromWICFile(
			wPictureName.c_str(),	//ここで文字型に
			DirectX::WIC_FLAGS_NONE,
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
		DirectX::TEX_FILTER_DEFAULT, 0, mipChain_);
	if (SUCCEEDED(result))
	{
		scratchImg_ = std::move(mipChain_);
		metadata_ = scratchImg_.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata_.format = DirectX::MakeSRGB(metadata_.format);
}

void NTextureManager::SetTBHeap()
{
	texHeapProp_.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp_.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp_.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;	//転送はCPU側(L0)から直接行う
	texHeapProp_.CreationNodeMask = 0;							//単一アダプターだから0
	texHeapProp_.VisibleNodeMask = 0;
}

void NTextureManager::SetTBResource()
{
	texResDesc_.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texResDesc_.Format = metadata_.format;

	texResDesc_.Width = metadata_.width;	//幅
	texResDesc_.Height = (uint32_t)metadata_.height;	//高さ
	texResDesc_.DepthOrArraySize = (UINT16)metadata_.arraySize;
	texResDesc_.MipLevels = (UINT16)metadata_.mipLevels;
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

	//全ミップマップについて
	for (size_t i = 0; i < metadata_.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const DirectX::Image* img = scratchImg_.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(uint32_t)i,
			nullptr,				//全領域へコピー
			img->pixels,			//元データアドレス
			(uint32_t)img->rowPitch,	//1ラインサイズ
			(uint32_t)img->slicePitch	//全サイズ
		);
		assert(SUCCEEDED(result));
	}
}

void NTextureManager::SetSRV()
{
	srvDesc_.Format = texResDesc_.Format;		//RGBA float
	srvDesc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc_.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc_.Texture2D.MipLevels = texResDesc_.MipLevels;
}

NTexture NTextureManager::CreateSRV(NTexture& tex)
{
	//SRVヒープの先頭ハンドルを取得
	tex.cpuHandle_ = NDX12::GetInstance()->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart();
	tex.gpuHandle_ = NDX12::GetInstance()->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
	//指定されたテクスチャ番号に応じてハンドルを進める
	uint32_t incrementSize = NDX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	tex.cpuHandle_.ptr += incrementSize * (uint32_t)textureMap_.size();
	tex.gpuHandle_.ptr += incrementSize * (uint32_t)textureMap_.size();

	//ハンドルの指す位置にシェーダーリソースビュー作成
	NDX12::GetInstance()->GetDevice()->CreateShaderResourceView(tex.texBuff_.Get(), &srvDesc_, tex.cpuHandle_);

	return tex;
}

NTexture::NTexture()
{
}

NTexture::~NTexture()
{
}
