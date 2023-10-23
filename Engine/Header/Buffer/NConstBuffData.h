#pragma once
#include "NVec2.h"
#include "NMatrix4.h"
#include "NColor.h"
#include <array>

//定数バッファ用データ構造体//

//3D変換行列
struct  ConstBuffDataTransform
{
	//NMatrix4 mat;	//3D変換行列
	NMatrix4 viewproj;	// ビュープロジェクション行列
	NMatrix4 world;		// ワールド行列
	NVec3 cameraPos;	// カメラ座標(ワールド座標)
};

//2D変換行列
struct ConstBuffDataTransform2D
{
	NMatrix4 mat;	//3D変換行列
};

//ビルボード変換行列
struct ConstBuffDataBillboard
{
	NMatrix4 mat;	//3D変換行列
	NMatrix4 matBillboard;	//ビルボード行列
};

//色
struct ConstBuffDataColor
{
	NColor color;	//色
};

//ADS
struct ConstBuffDataMaterial
{
	NVec3 ambient;	// アンビエント係数
	float pad1;
	NVec3 diffuse;	// ディフューズ係数
	float pad2;
	NVec3 specular;	// スペキュラー係数
	float pad3;
	float alpha;	// アルファ
};

//平行光源
struct ConstBuffDataLight
{
	NVec3 dir;
	float pad1;
	NVec3 color;
	uint32_t active;
};

//ポイントライト
struct ConstBuffDataPointLight
{
	NVec3 pos;
	float pad1;
	NVec3 color;
	float pad2;
	NVec3 atten;
	uint32_t active;
};

//スポットライト
struct ConstBuffDataSpotLight
{
	NVec3 dir;
	float pad1;
	NVec3 pos;
	float pad2;
	NVec3 color;
	float pad3;
	NVec3 atten;
	float pad4;
	NVec2 factorAngleCos;
	uint32_t active;
	float pad5;
};

//丸影
struct ConstBuffDataCircleShadow
{
	NVec3 dir;
	float pad1;
	NVec3 pos;
	float disCasterLight;
	NVec3 atten;
	float pad2;
	NVec2 factorAngleCos;
	uint32_t active;
	float pad3;
};

//タイル
static const uint32_t maxObj = 256;
struct ConstBuffDataTile
{
	float divide;						//分割幅
	float activityArea;					//行動範囲
	bool isAvoid;						//避けるようになるか否か
	float pad2;
	std::array<NVec3,maxObj> objPos;	//フィールド上にいるオブジェクトの座標
};

//スキン情報
static const uint32_t maxBones = 32;
struct ConstBuffDataSkin
{
	std::array<NMatrix4, maxBones> bones;
};