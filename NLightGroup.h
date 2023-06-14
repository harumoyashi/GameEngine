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

public://�ÓI�����o�֐�
	//���ꂼ��̃��C�g���g�������߂�
	//�����w�肵�Ȃ���Ε��s�����Ɗۉe���L���ɂȂ�
	void Init(const bool isDirLights = true, const bool isPointLights = false,
		const bool isSpotLights = false, const bool isCircleShadows = false);
	void Update();
	void Draw();
};