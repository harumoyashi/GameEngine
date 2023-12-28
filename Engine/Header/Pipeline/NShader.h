#pragma once
#include <d3d12.h>

#include <string>
#include <unordered_map>
#include <wrl.h>

// シェーダー //
// unordered_mapにまとめて登録もやってる //

class NShader final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3DBlob> vsBlob_;		// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob_;		// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob_;		// ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> csBlob_;		// コンピュートシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob_;	// エラーオブジェクト

	static std::unordered_map<std::string, NShader> shaderMap_;

public:
	//パスに合わせた頂点シェーダーの読み込み
	void LoadVS(std::string vsPath);
	//パスに合わせたピクセルシェーダーの読み込み
	void LoadPS(std::string psPath);
	//パスに合わせたジオメトリシェーダーの読み込み
	void LoadGS(std::string gsPath);
	//パスに合わせたコンピュートシェーダーの読み込み
	void LoadCS(std::string csPath);

	//シェーダーを生成してマップに登録
	//id:シェーダーマップに登録する名前
	//path:シェーダーマップに登録するシェーダーのパス
	//isLoadGS:ジオメトリシェーダー読み込むか
	//isLoadCS:コンピュートシェーダー読み込むか
	static void CreateShader(std::string id, std::string path, bool isLoadGS = false, bool isLoadCS = false);
	//指定したidのシェーダーを取得
	static NShader* GetShader(std::string id);

	//頂点シェーダオブジェクト取得
	ID3DBlob* GetVSBlob() { return vsBlob_.Get(); }
	//ピクセルシェーダオブジェクト取得
	ID3DBlob* GetPSBlob() { return psBlob_.Get(); }
	//ジオメトリシェーダオブジェクト取得
	ID3DBlob* GetGSBlob() { return gsBlob_.Get(); }
	//コンピュートシェーダオブジェクト取得
	ID3DBlob* GetCSBlob() { return csBlob_.Get(); }
};