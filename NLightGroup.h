#pragma once
#include "NDirectionalLight.h"
#include "NPointLight.h"
#include "NSpotLight.h"
#include "NCircleShadow.h"
#include "NVector3.h"
#include "NVector2.h"

class NLightGroup
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const int DirLightNum = 3;
	static const int PointLightNum = 3;
	static const int SpotLightNum = 3;
	static const int CircleShadowNum = 1;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		NVector3 ambientcolor;	//�����̐F
		float pad1;	//�p�f�B���O
		NDirectionalLight::ConstBufferData dirLights[DirLightNum];	//���s�����p
		NPointLight::ConstBufferData pointLights[PointLightNum];		//�_�����p
		NSpotLight::ConstBufferData spotLights[SpotLightNum];		//�X�|�b�g���C�g�p
		NCircleShadow::ConstBufferData circleShadows[CircleShadowNum];		//�ۉe�p
	};

private://�ÓI�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	NVector3 ambientcolor = { 1,1,1 };				//�����̐F
	NDirectionalLight dirLights[DirLightNum];		//���s�����̔z��
	NPointLight pointLights[PointLightNum];			//�_�����̔z��
	NSpotLight spotLights[SpotLightNum];				//�_�����̔z��
	NCircleShadow circleShadows[CircleShadowNum];	//�ۉe�̔z��

	//�_�[�e�B�t���O
	bool isDirty = false;

public://�ÓI�����o�֐�
	void Init();
	void Update();
	void Draw(UINT rootParamerterIndex);

	static std::unique_ptr<NLightGroup> Create();

	//�萔�o�b�t�@�]��
	void TransferConstBuffer();
	//�����̃��C�g�F���Z�b�g
	void SetAmbientColor(const NVector3& color);
	//���s�����̗L���t���O���Z�b�g
	void SetDirLightActive(int index, bool active);
	//���s�����̃��C�g�������Z�b�g
	void SetDirLightDir(int index, const NVector3& lightdir);
	//���s�����̃��C�g�F���Z�b�g
	void SetDirLightColor(int index, const NVector3& lightColor);
	//�W���̃��C�g�ݒ�
	void DefaultLightSetting();

	//�_�����̍��W���Z�b�g
	void SetPointLightPos(int index, const NVector3& lightpos);
	//�_�����̐F���Z�b�g
	void SetPointLightColor(int index, const NVector3& lightcolor);
	//�_�����̌����W�����Z�b�g
	void SetPointLightAtten(int index, const NVector3& lightatten);
	//�L���t���O���Z�b�g
	void SetPointLightActive(int index, bool isActive);

	//�X�|�b�g���C�g�̃��C�g�������Z�b�g
	void SetSpotLightDir(int index, const NVector3& lightdir);
	//�X�|�b�g���C�g�̍��W���Z�b�g
	void SetSpotLightPos(int index, const NVector3& lightpos);
	//�X�|�b�g���C�g�̐F���Z�b�g
	void SetSpotLightColor(int index, const NVector3& lightcolor);
	//�X�|�b�g���C�g�̌����W�����Z�b�g
	void SetSpotLightAtten(int index, const NVector3& lightatten);
	//�X�|�b�g���C�g�̌����p�x���Z�b�g
	void SetSpotLightFactorAngle(int index, const NVector2& lightFactorAngle);
	//�L���t���O���Z�b�g
	void SetSpotLightActive(int index, bool isActive);

	//�ۉe�̃��C�g�������Z�b�g
	void SetCircleShadowDir(int index, const NVector3& dir);
	//�ۉe�̍��W���Z�b�g
	void SetCircleShadowCasterPos(int index, const NVector3& casterPos);
	//�ۉe�̍��W���Z�b�g
	void SetCircleShadowDistanceCasterLight(int index, const float& distanceCasterLight);
	//�ۉe�̌����W�����Z�b�g
	void SetCircleShadowAtten(int index, const NVector3& atten);
	//�ۉe�̌����p�x���Z�b�g
	void SetCircleShadowFactorAngle(int index, const NVector2& factorAngle);
	//�L���t���O���Z�b�g
	void SetCircleShadowActive(int index, bool isActive);
};