#include "NShader.h"

#include <cassert>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

void NShader::LoadVS(std::string vsPath)
{
	HRESULT result;

	std::string shaderFolder = "Resources/shaders/";
	std::string shaderType = "VS.hlsl";
	vsPath = shaderFolder + vsPath + shaderType;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		std::wstring(vsPath.begin(), vsPath.end()).c_str(),  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob_, &errorBlob_);

	// エラーなら
	if (FAILED(result)) {
		// errorBlob_からエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NShader::LoadPS(std::string psPath)
{
	HRESULT result;

	std::string shaderFolder = "Resources/shaders/";
	std::string shaderType = "PS.hlsl";
	psPath = shaderFolder + psPath + shaderType;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		std::wstring(psPath.begin(), psPath.end()).c_str(),  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob_, &errorBlob_);

	// エラーなら
	if (FAILED(result)) {
		// errorBlob_からエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NShader::LoadGS(std::string gsPath)
{
	HRESULT result;

	std::string shaderFolder = "Resources/shaders/";
	std::string shaderType = "GS.hlsl";
	gsPath = shaderFolder + gsPath + shaderType;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		std::wstring(gsPath.begin(), gsPath.end()).c_str(),  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob_, &errorBlob_);

	// エラーなら
	if (FAILED(result)) {
		// errorBlob_からエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NShader::CreateShader(std::string id)
{
	shaderMap_[id] = NShader();
}

void NShader::LoadShader(std::string id, std::string path, const bool isLoadGS)
{
	shaderMap_[id].LoadVS(path);
	shaderMap_[id].LoadPS(path);
	if (isLoadGS)
	{
		shaderMap_[id].LoadGS(path);
	}
}

NShader* NShader::GetShader(std::string id)
{
	return &shaderMap_[id];
}
