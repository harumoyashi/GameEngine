#pragma once
#include "NDX12.h"
#include "NTextureConverter.h"
#include <array>
#include <unordered_map>

#include <wrl.h>

typedef std::string TextureHandle;

struct NTexture final
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> texBuff_;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle_ = D3D12_CPU_DESCRIPTOR_HANDLE(); //SRVのハンドル(CPU側)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_ = D3D12_GPU_DESCRIPTOR_HANDLE(); //SRVのハンドル(GPU側)
	std::string fileName_; //ファイル名

	NTexture();
	~NTexture();
};

class NTextureManager final
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	DirectX::TexMetadata metadata_{};
	DirectX::ScratchImage scratchImg_{};
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc_{};
	D3D12_HEAP_PROPERTIES texHeapProp_{};	//ヒープ
	D3D12_RESOURCE_DESC texResDesc_{};		//リソース
	static const size_t kMaxSRVCount = 2056;	//SRVの最大個数
	DirectX::ScratchImage mipChain_{};				//ミップマップ

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};	//設定構造体

	std::unordered_map<TextureHandle, NTexture> textureMap_;	//テクスチャ群
	ComPtr<ID3D12Resource> tb_;	//転送用のテクスチャバッファ

	NTextureConverter converter;

public:
	//TextureManagerを取得する
	static NTextureManager* GetInstance();

	//初期化
	void Init();

	//エラーテクスチャを生成する
	NTexture CreateErrorTexture();

	//テクスチャの読み込み
	//indexを入れなかったら0番目に格納
	//読み込むたびに書き変わるから、一回きりの時だけにして
	NTexture LoadTexture(const std::string& pictureName, const std::string& handle = "");

private:
	//WICテクスチャのロード
	bool Load(const std::string& pictureName);
	//ミップマップ生成
	void CreateMipmap();

	//ヒープ設定
	void SetTBHeap();
	//リソース設定
	void SetTBResource();
	//テクスチャバッファの生成
	ID3D12Resource* CreateTexBuff();
	//ミップマップデータの転送
	void TexBuffMaping(ID3D12Resource* texBuff);
	//シェーダーリソースビュー設定
	void SetSRV();
	//シェーダーリソースビュー生成
	NTexture CreateSRV(NTexture& tex);
};

