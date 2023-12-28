#pragma once
#include "NDirectionalLight.h"
#include "NPointLight.h"
#include "NSpotLight.h"
#include "NCircleShadow.h"
#include "NConstBuff.h"

// 光源まとめて管理 //

class NLightGroup final
{
public:
	static const uint32_t kDirLightNum = 3;
	static const uint32_t kPointLightNum = 128;
	static const uint32_t kSpotLightNum = 3;
	static const uint32_t kCircleShadowNum = 1;

	static std::unique_ptr<NDirectionalLight> sDirLights[kDirLightNum];
	static std::unique_ptr<NPointLight> sPointLights[kPointLightNum];
	static std::unique_ptr<NSpotLight> sSpotLights[kSpotLightNum];
	static std::unique_ptr<NCircleShadow> sCircleShadows[kCircleShadowNum];

private:
	//NConstBuffData.hで定義していないのは
	//上記の定数がないと定義できないため、ここでやってる
	struct ConstBuffDataLightGroup
	{
		NVec3 ambientcolor;	//環境光の色
		float pad1;	//パディング
		ConstBuffDataLight dirLights[kDirLightNum];					//平行光源用
		ConstBuffDataPointLight pointLights[kPointLightNum];		//点光源用
		ConstBuffDataSpotLight spotLights[kSpotLightNum];			//スポットライト用
		ConstBuffDataCircleShadow circleShadows[kCircleShadowNum];	//丸影用
	};

	//定数バッファ
	std::unique_ptr<NConstBuff<ConstBuffDataLightGroup>> cbLight_;

	NVec3 ambientcolor = NVec3::one;				//環境光の色

	//ダーティフラグ
	bool isDirty = false;

public://静的メンバ関数
	//それぞれのライトを使うか決める
	//何も指定しなければ平行光源と丸影が有効になる
	void Init(bool isDirLights = true, bool isPointLights = false,
		bool isSpotLights = false, bool isCircleShadows = false);
	//更新
	void Update();
	//rootParamIndex:ルートパラメータの何番目か
	void Draw(const uint32_t rootParamIndex);

	//定数バッファ情報転送
	void TransferConstBuffer();

	// セッター //
	//アンビエント色設定
	void SetAmbientColor(const NVec3& color);
	//平行光源の色をセット
	void SetDirLightColor(const NVec3& lightcolor);
	//ポイントライトの色をセット
	void SetPointLightColor(const NVec3& lightcolor);
	//スポットライトの色をセット
	void SetSpotLightColor(const NVec3& lightcolor);
};