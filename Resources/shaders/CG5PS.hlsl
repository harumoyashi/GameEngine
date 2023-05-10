#include "CG5.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    // �e�N�X�`���}�b�s���O
    float4 texcolor = tex.Sample(smp, input.uv);

    //�_����
    if (pointLights[0].active == true)
    {
        //���C�g�̃x�N�g��
        float3 lightv = pointLights[0].lightpos - input.worldpos.xyz;
        //�x�N�g���̒���
        float d = length(lightv);
        //���K�����A�P�ʃx�N�g���ɂ���
        //lightv = normalize(lightv);
        //���������W��
        float atten = 1.0f / (pointLights[0].lightatten.x + pointLights[0].lightatten.y * d +
            pointLights[0].lightatten.z * d * d);
           
	    // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
        //float3 dotlightnormal = dot(lightv, input.normal);
	    // ���ˌ��x�N�g��
        //float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
        // �g�U���ˌ�
        //float3 diffuse = dotlightnormal * m_diffuse;
	    // ���ʔ��ˌ�
        //float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	    // �S�ĉ��Z����
        //shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor * m_color;
        //shadecolor.a = m_alpha;
        
	    //����
        float4 _LightColor = (pointLights[0].lightcolor, 1);
        float4 ambient = texcolor * 0.3f * _LightColor;

	    //�g�U���ˌ�
	    //intensity = ���邳
	    //90��������Ɠ��ς̒l���}�C�i�X�ɂȂ�̂ŁAsaturate��0~1�ɂ���
        //float4 LightPos = (lightpos, 0);
        float intensity = saturate(dot(normalize(input.normal), pointLights[0].lightpos));
				
	        //���邳�����ȏ�Ȃ疾�邭�A���ꖢ���Ȃ�Â�
        float _Threshold = 0.1f;
        float t = _Threshold; //臒l
        float u = _Threshold + 0.05f; //臒l
        intensity = smoothstep(t, u, intensity); //�Ƃ��[�[�[�[��

        float4 diffuse = texcolor * intensity * _LightColor;

	    //���ʔ��ˌ�
	    //�J�����̍��W����A���[���h�ϊ��������_���W���������ƂŁA���_����J�����܂ł̃x�N�g�����Z�o����
        //���_���王�_�ւ̕����x�N�g��
        float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
        float3 lightDir = normalize(pointLights[0].lightpos);
        input.normal = normalize(input.normal);
        float3 reflecDir = -lightDir + 2 * input.normal * dot(input.normal, lightDir);
        float4 specular = pow(saturate(dot(reflecDir, eyedir)), 20) * _LightColor;

	    //�����Ɩ@���̓��ς����������������点��悤��
        float4 rimlight = 1.0f - saturate(dot(eyedir, input.normal));
        
	    //�g�D�[�����ۂ�����̂��悫
	    //���邳�����ȏ�Ȃ疾�邭�A���ꖢ���Ȃ�Â�
        float t2 = _Threshold; //臒l
        float u2 = _Threshold + 0.05f;
        rimlight = smoothstep(t, u, rimlight);
        
	    //pow�Œl���i��Ƃ�����ۂ�
        float _Pow = 4.0f;
        rimlight = pow(rimlight, _Pow);

        float4 _RimColor = (1, 0, 0, 1);
        //float4 ads = atten * (1.0f - rimlight) * (ambient + diffuse + specular) + rimlight * _RimColor;
        //float4 ads = (1.0f - rimlight) * (ambient + diffuse + specular) + rimlight * _RimColor;
        //float4 ads = ambient + diffuse + specular;
        float4 ads = ambient;

        return ads;
    }
    else
    {
        return texcolor;
    }
}