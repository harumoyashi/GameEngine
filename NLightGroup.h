#pragma once
#include "NDirectionalLight.h"
#include "NPointLight.h"
#include "NSpotLight.h"
#include "NCircleShadow.h"

class NLightGroup
{
public:
	static NDirectionalLight sDirLights;
	static NPointLight sPointLights;
	static NSpotLight sSpotLights;
	static NCircleShadow sCircleShadows;

public://静的メンバ関数
	//それぞれのライトを使うか決める
	//何も指定しなければ平行光源と丸影が有効になる
	void Init(const bool isDirLights = true, const bool isPointLights = false,
		const bool isSpotLights = false, const bool isCircleShadows = false);
	void Update();
	void Draw();
};