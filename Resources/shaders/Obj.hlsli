cbuffer cbuff0 : register(b0)
{
    float3 m_ambient : packoffset(c0);  // �A���r�G���g�W��
    float3 m_diffuse : packoffset(c1);  // �f�B�t���[�Y�W��
    float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
}

cbuffer cbuff1 : register(b1)
{
    float4 m_color;
}

cbuffer cbuff2 : register(b2)
{
    matrix viewproj; // �r���[�v���W�F�N�V�����s��
    matrix world; // ���[���h�s��
    float3 cameraPos; // �J�������W(���[���h���W)
};

//���s����
struct DirLight
{
    float3 lightv; //���C�g�ւ̕����̒P�ʃx�N�g��
    uint active;
    float3 lightcolor; //���C�g�̐F(RGB)
};

cbuffer cbuff3 : register(b3)
{
    DirLight dirLight;
};

//�_����
struct PointLight
{
    float3 lightpos; //���C�g���W
    float3 lightcolor; //���C�g�̐F(RGB)
    float3 lightatten; //���C�g�̋��������W��
    uint active;
};

cbuffer cbuff4 : register(b4)
{
    PointLight pointLight;
};

//�X�|�b�g���C�g
struct SpotLight
{
    float3 lightv; //���C�g�̌��������̋t�x�N�g��
    float3 lightpos; //���C�g���W
    float3 lightcolor; //���C�g�̐F(RGB)
    float3 lightatten; //���C�g�̋��������W��
    float2 lightfactoranglecos; //���C�g�̋��������p�x�̃R�T�C��
    uint active;
};

cbuffer cbuff5 : register(b5)
{
    SpotLight spotLight;
};

//�ۉe
struct CircleShadow
{
    float3 dir; //���e�����̋t�x�N�g��
    float3 casterPos; //���C�g���W
    float distanceCasterLight; //�L���X�^�[�ƃ��C�g�̋���
    float3 atten; //���������W��
    float2 factoranglecos; //�����p�x�̃R�T�C��
    uint active;
};

cbuffer cbuff6 : register(b6)
{
    CircleShadow circleShadow;
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float4 worldpos : POSITION; // ���[���h���W
    float3 normal : NORMAL; // �@���x�N�g��
    float2 uv : TEXCOORD; // uv�l
};
