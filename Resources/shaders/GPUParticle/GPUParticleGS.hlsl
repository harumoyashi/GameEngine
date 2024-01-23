#include "GPUParticle.hlsli"

//�l�p�`�̒��_��
static const uint kVnum = 4;

//�Z���^�[����̃I�t�Z�b�g
static const float4 kOffset_array[kVnum] =
{
    float4(-0.5f, -0.5f, -0.5f, 0), //����
    float4(-0.5f, +0.5f, -0.5f, 0), //����
    float4(+0.5f, -0.5f, -0.5f, 0), //�E��
    float4(+0.5f, +0.5f, -0.5f, 0), //�E��
};

//���オ0,0�@�E����1,1
static const float2 kUV_array[kVnum] =
{
    float2(0.0f, 1.0f), //����
    float2(0.0f, 0.0f), //����
    float2(1.0f, 1.0f), //�E��
    float2(1.0f, 0.0f), //�E��
};

[maxvertexcount(kVnum)]
void main(
	point VSOutput input[1],
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element; //�o�͗p���_�f�[�^
   
    for (uint i = 0; i < kVnum; i++)
    {
        //���S����̃I�t�Z�b�g���X�P�[�����O
        float4 offset = kOffset_array[i] * input[0].scale;
        
        //�I�t�Z�b�g�����炷(���[���h���W)
        element.svpos = input[0].pos + offset;
        
        //�r���[�A�ˉe�ϊ�
        element.svpos = mul(viewproj, element.svpos);
        float4 wpos = mul(world, element.svpos);
        element.worldpos = wpos;
        element.color = input[0].color;
        element.uv = kUV_array[i];
        
        output.Append(element);
    }
}