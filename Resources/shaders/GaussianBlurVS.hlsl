//#include "GaussianBlur.hlsli"

//VSOutput main(float4 pos : SV_POSITION, float2 uv : TEXCOORD)
//{
//    VSOutput output; //�s�N�Z���V�F�[�_�[�ɓn���l
//    output.svpos = pos; //���W�ɍs�����Z
//    output.uv = uv;
//    return output;
//}

float4 main(float4 pos : POSITION) : SV_POSITION
{
    return pos;
}