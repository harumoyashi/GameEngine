#include "Sprite.hlsli"

VSOutput main(float3 pos:POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;	//�s�N�Z���V�F�[�_�[�ɓn���l
    float4 p = (pos, 0);
	output.svpos = mul(mat, p);	//���W�ɍs�����Z
	output.uv = uv;
	return output;
}