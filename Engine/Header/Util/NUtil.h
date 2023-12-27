#pragma once
#include <string>

// 便利な構造体やら関数やら //

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

	//マルチバイト文字列をワイド文字列に変換
	static std::wstring ToWideString(const std::string& mString);
	//拡張子抽出
	static std::string GetExtension(const std::string& path);
	//拡張子置き換え処理
	static std::wstring ReplaceExtension(const std::wstring& origin, const char* ext);
	//文字列からuint32_t型に変換して返す
	//逆やりたいときはstd::to_string()使えばいい
	static uint32_t StringToInt(const std::string& string);
	//文字列からfloat型に変換して返す
	//逆やりたいときはstd::to_string()使えばいい
	static float StringToFloat(const std::string& string);
};