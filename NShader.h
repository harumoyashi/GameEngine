#pragma once
#include <d3d12.h>

#include <string>
#include <map>
#include <wrl.h>

class NShader
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3DBlob> vsBlob_;	// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob_;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob_;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob_; // エラーオブジェクト

	std::map<std::string, NShader> shaderMap_;

public:
	static NShader* GetInstance();

	//パスに合わせた頂点シェーダーの読み込み
	void LoadVS(std::string vsPath);
	//パスに合わせたピクセルシェーダーの読み込み
	void LoadPS(std::string psPath);
	//パスに合わせたジオメトリシェーダーの読み込み
	void LoadGS(std::string gsPath);

	//シェーダーを生成してマップに登録
	void CreateShader(std::string id, std::string path, const bool isLoadGS);
	//指定したidのシェーダーを取得
	NShader* GetShader(std::string id);
};