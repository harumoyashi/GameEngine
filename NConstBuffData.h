#pragma once
#include "NVector2.h"
#include "NMatrix4.h"
#include "NColor.h"

//定数バッファ用データ構造体//

//3D変換行列
struct  ConstBuffDataTransform
{
	//NMatrix4 mat;	//3D変換行列
	NMatrix4 viewproj;	// ビュープロジェクション行列
	NMatrix4 world;		// ワールド行列
	NVector3 cameraPos;	// カメラ座標(ワールド座標)
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
	NVector3 ambient;	// アンビエント係数
	float pad1;
	NVector3 diffuse;	// ディフューズ係数
	float pad2;
	NVector3 specular;	// スペキュラー係数
	float pad3;
	float alpha;	// アルファ
};

//平行光源
struct ConstBuffDataLight
{
	NVector3 dir;
	float pad1;
	NVector3 color;
	uint32_t active;
};

//ポイントライト
struct ConstBuffDataPointLight
{
	NVector3 pos;
	float pad1;
	NVector3 color;
	float pad2;
	NVector3 atten;
	uint32_t active;
};

//スポットライト
struct ConstBuffDataSpotLight
{
	NVector3 dir;
	float pad1;
	NVector3 pos;
	float pad2;
	NVector3 color;
	float pad3;
	NVector3 atten;
	float pad4;
	NVector2 factorAngleCos;
	uint32_t active;
	float pad5;
};

//丸影
struct ConstBuffDataCircleShadow
{
	NVector3 dir;
	float pad1;
	NVector3 pos;
	float disCasterLight;
	NVector3 atten;
	float pad2;
	NVector2 factorAngleCos;
	uint32_t active;
	float pad3;
};

//タイル
struct ConstBuffDataTile
{
	float divide;		//分割幅
	float activityArea;	//行動範囲
};