#include "GaussianBlur.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2.0f * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	 //�K�E�V�A���u���[//
    float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.001; //Bloom�̓u���[��傰����
    float4 col = float4(0, 0, 0, 1);

    for (float py = -_Sigma * 2.0f; py <= _Sigma * 2.0f; py += _StepWidth)
    {
        for (float px = -_Sigma * 2.0f; px <= _Sigma * 2.0f; px += _StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, _Sigma);
            col += float4(tex.Sample(smp, pickUV)) * weight; //Gaussian�Ŏ擾�����u�d�݁v��F�ɂ�����
            totalWeight += weight; //�������u�d�݁v�̍��v�l���T���Ƃ�
        }
    }
    col.rgb = col.rgb / totalWeight; //�������u�d�݁v���A���ʂ��犄��
    return col;
}

//float4 main() : SV_TARGET
//{
//    return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}