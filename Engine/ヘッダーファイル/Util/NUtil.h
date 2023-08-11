#pragma once
#include <string>

static const int maxBoneIndices = 4;

enum class BlendMode
{
	None,	//そのまま
	Alpha,	//αブレンド
	Add,	//加算
	Sub,	//減算
	Inv,	//反転
};

// モデルのフォーマット
enum class ModelFormat
{
	Obj,
	Fbx,
};

struct NUtil
{
	//wstring型をstring型に変換
	static std::string ToUTF8(const std::wstring& value);
	//拡張子抽出
	static std::string GetExtension(const std::string& path);
	//拡張子置き換え処理
	static std::wstring ReplaceExtension(const std::wstring& origin, const char* ext);
};