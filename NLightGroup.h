#pragma once
#include "NDirectionalLight.h"
#include "NPointLight.h"
#include "NSpotLight.h"
#include "NCircleShadow.h"
#include "NConstBuff.h"

class NLightGroup
{
public:
	static const uint32_t kDirLightNum = 3;
	static const uint32_t kPointLightNum = 3;
	static const uint32_t kSpotLightNum = 3;
	static const uint32_t kCircleShadowNum = 1;

	static std::unique_ptr<NDirectionalLight> sDirLights[kDirLightNum];
	static std::unique_ptr<NPointLight> sPointLights[kPointLightNum];
	static std::unique_ptr<NSpotLight> sSpotLights[kSpotLightNum];
	static std::unique_ptr<NCircleShadow> sCircleShadows[kCircleShadowNum];

private:
	//NConstBuffData.h�Œ�`���Ă��Ȃ��̂�
	//��L�̒萔���Ȃ��ƒ�`�ł��Ȃ����߁A�����ł���Ă�
	struct ConstBuffDataLightGroup
	{
		NVector3 ambientcolor;	//�����̐F
		float pad1;	//�p�f�B���O
		ConstBuffDataLight dirLights[kDirLightNum];					//���s�����p
		ConstBuffDataPointLight pointLights[kPointLightNum];		//�_�����p
		ConstBuffDataSpotLight spotLights[kSpotLightNum];			//�X�|�b�g���C�g�p
		ConstBuffDataCircleShadow circleShadows[kCircleShadowNum];	//�ۉe�p
	};

	//�萔�o�b�t�@
	std::unique_ptr<NConstBuff<ConstBuffDataLightGroup>> cbLight_;

	NVector3 ambientcolor = { 1,1,1 };				//�����̐F
	
	//�_�[�e�B�t���O
	bool isDirty = false;

public://�ÓI�����o�֐�
	//���ꂼ��̃��C�g���g�������߂�
	//�����w�肵�Ȃ���Ε��s�����Ɗۉe���L���ɂȂ�
	void Init(const bool isDirLights = true, const bool isPointLights = false,
		const bool isSpotLights = false, const bool isCircleShadows = false);
	void Update();
	void Draw();

	void TransferConstBuffer();

	void SetAmbientColor(const NVector3& color);

	//���C�g�̐F���Z�b�g
	void SetDirLightColor(const NVector3& lightcolor);
	void SetPointLightColor(const NVector3& lightcolor);
	void SetSpotLightColor(const NVector3& lightcolor);
};