#include "Sprite.hlsli"

VSOutput main(float4 pos:POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;	//�s�N�Z���V�F�[�_�[�ɓn���l
    //float4 p = mul(mat, float4(pos, 1)); //���W�ɍs�����Z
    //output.svpos = float3(p.x,p.y,p.z);
    output.svpos = pos;
	output.uv = uv;
	return output;
}