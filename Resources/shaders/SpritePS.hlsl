#include "Sprite.hlsli"

Texture2D<float4>tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float3 main(VSOutput input) : SV_TARGET
{
    float4 col = (tex.Sample(smp, input.uv)) * color; //�萔�o�b�t�@�ɑ������F�ς���Ɣ��f�����
    return float3(col.x, col.y, col.z);

}