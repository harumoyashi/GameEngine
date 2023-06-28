#include "OBJ.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input) : SV_TARGET
{
    PSOutput output;
    
	// �e�N�X�`���}�b�s���O
    float4 texcolor = tex.Sample(smp, input.uv);
    
	// ����x
    const float shininess = 4.0f;
	// ���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	
	// �����ˌ�
    float3 ambient = m_ambient;
    
	// �V�F�[�f�B���O�ɂ��F
    float4 shadecolor = float4(ambient, m_color.a);
	
    //���s����
    if (dirLight.active)
    {
            // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
        float3 dotlightnormal = dot(dirLight.lightv, input.normal);
	        // ���ˌ��x�N�g��
        float3 reflect = normalize(-dirLight.lightv + 2 * dotlightnormal * input.normal);
            // �g�U���ˌ�
        float3 diffuse = dotlightnormal * m_diffuse;
	        // ���ʔ��ˌ�
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	        // �S�ĉ��Z����
        shadecolor.rgb += (diffuse + specular) * dirLight.lightcolor;
        //shadecolor.a = m_color.a;
    }
    
    //�_����
    if (pointLight.active)
    {
            // ���C�g�̃x�N�g��
        float3 lightv = pointLight.lightpos - input.worldpos.xyz;
            //�x�N�g���̒���
        float d = length(lightv);
            //���K�����A�P�ʃx�N�g���ɂ���
        lightv = normalize(lightv);
            //���������W��
        float atten = 1.0f / (pointLight.lightatten.x + pointLight.lightatten.y * d +
            pointLight.lightatten.z * d * d);
            // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
        float3 dotlightnormal = dot(lightv, input.normal);
	        // ���ˌ��x�N�g��
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
            // �g�U���ˌ�
        float3 diffuse = dotlightnormal * m_diffuse;
	        // ���ʔ��ˌ�
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	        // �S�ĉ��Z����
        shadecolor.rgb += atten * (diffuse + specular) * pointLight.lightcolor;
        //shadecolor.a = m_color.a;
    }
    
    //�X�|�b�g���C�g
    if (spotLight.active)
    {
            // ���C�g�̃x�N�g��
        float3 lightv = spotLight.lightpos - input.worldpos.xyz;
            //�x�N�g���̒���
        float d = length(lightv);
            //���K�����A�P�ʃx�N�g���ɂ���
        lightv = normalize(lightv);
            //���������W��
        float atten = saturate(1.0f / (spotLight.lightatten.x + spotLight.lightatten.y * d +
            spotLight.lightatten.z * d * d));
            //�p�x����
        float cos = dot(lightv, spotLight.lightv);
            //�����J�n�p�x���猸���I���p�x�ɂ����Č���
            //�����J�n�p�x�̓�����1�{�@�����I���p�x�̊O����0�{�̋P�x
        float angleatten = smoothstep(spotLight.lightfactoranglecos.y, spotLight.lightfactoranglecos.x, cos);
            //�p�x��������Z
        atten *= angleatten;
            // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
        float3 dotlightnormal = dot(lightv, input.normal);
	        // ���ˌ��x�N�g��
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
            // �g�U���ˌ�
        float3 diffuse = dotlightnormal * m_diffuse;
	        // ���ʔ��ˌ�
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	        // �S�ĉ��Z����
        shadecolor.rgb += atten * (diffuse + specular) * spotLight.lightcolor;
        //shadecolor.a = m_color.a;
    }
    
    //�ۉe
    if (circleShadow.active)
    {
            //�I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
        float3 casterv = circleShadow.casterPos - input.worldpos.xyz;
            //���e�����ł̋���
        float d = dot(casterv, circleShadow.dir);
            //���������W��
        float atten = saturate(1.0f / (circleShadow.atten.x + circleShadow.atten.y * d +
            circleShadow.atten.z * d * d));
            //�������}�C�i�X�Ȃ�0�ɂ���
        atten *= step(0, d);
            //���z���C�g�̍��W
        float3 lightpos = circleShadow.casterPos + circleShadow.dir * circleShadow.distanceCasterLight;
            //�I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g��(�P�ʃx�N�g��)
        float3 lightv = normalize(lightpos - input.worldpos.xyz);
            //�p�x����
        float cos = dot(lightv, circleShadow.dir);
            //�����J�n�p�x���猸���I���p�x�ɂ����Č���
            //�����J�n�p�x�̓�����1�{�@�����I���p�x�̊O����0�{�̋P�x
        float angleatten = smoothstep(circleShadow.factoranglecos.y, circleShadow.factoranglecos.x, cos);
            //�p�x��������Z
        atten *= angleatten;
	        // �S�Č��Z����
        shadecolor.rgb -= atten;
    }

    // �V�F�[�f�B���O�F�ŕ`��
    float4 color = shadecolor * texcolor * m_color;
    output.target0 = color;
    output.target1 = color;
    return output;
    //return shadecolor * texcolor * m_color;
}