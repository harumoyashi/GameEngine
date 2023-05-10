#pragma once
#include "NVector2.h"
#include "NMatrix4.h"
#include "NColor.h"

//定数バッファ用データ構造体//

//3D変換行列
struct  ConstBufferDataTransform
{
	//NMatrix4 mat;	//3D変換行列
	NMatrix4 viewproj;	// ビュープロジェクション行列
	NMatrix4 world;		// ワールド行列
	NVector3 cameraPos;	// カメラ座標(ワールド座標)
};

//2D変換行列
struct ConstantBufferDataTransform2D
{
	NMatrix4 mat;	//3D変換行列
};

//ビルボード変換行列
struct ConstantBufferDataBillboard
{
	NMatrix4 mat;	//3D変換行列
	NMatrix4 matBillboard;	//ビルボード行列
};

//色
struct ConstantBufferDataColor
{
	NColor color;	//色
};

//ADS
struct ConstantBufferDataMaterial
{
	NVector3 ambient;	// アンビエント係数
	float pad1;
	NVector3 diffuse;	// ディフューズ係数
	float pad2;
	NVector3 specular;	// スペキュラー係数
	float alpha;	// アルファ
};

//ライト
struct ConstantBufferDataLight
{
	NVector3 dir;
	float pad1;
	NVector3 color;
};

//ポイントライト
struct ConstantBufferDataPointLight
{
	NVector3 pos;
	float pad1;
	NVector3 color;
	float pad2;
	NVector3 atten;
	unsigned int active;
};

//スポットライト
struct ConstantBufferDataSpotLight
{
	NVector3 vec;
	float pad1;
	NVector3 pos;
	float pad2;
	NVector3 color;
	float pad3;
	NVector3 atten;
	float pad4;
	NVector2 factorAngleCos;
	unsigned int isActive;
	float pad5;
};

//丸影
struct ConstantBufferDataCircleShadow
{
	NVector3 vec;
	float pad1;
	NVector3 pos;
	float disCasterLight;
	NVector3 atten;
	float pad2;
	NVector2 factorAngleCos;
	float darkness;
	unsigned int isActive;
};