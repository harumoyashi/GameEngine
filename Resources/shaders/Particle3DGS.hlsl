#include "Particle3D.hlsli"

//�l�p�`�̒��_��
static const uint vnum = 24;

//�Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[vnum] =
{
    //--------------- ��O ---------------//
    float4(-0.5f, -0.5f, +0.5f, 0), //����
    float4(-0.5f, +0.5f, +0.5f, 0), //����
    float4(+0.5f, -0.5f, +0.5f, 0), //�E��
    float4(+0.5f, +0.5f, +0.5f, 0), //�E��
    
    //---------------  ��  ---------------//
    float4(+0.5f, -0.5f, -0.5f, 0), //����
    float4(+0.5f, +0.5f, -0.5f, 0), //����
    float4(-0.5f, -0.5f, -0.5f, 0), //�E��
    float4(-0.5f, +0.5f, -0.5f, 0), //�E��
    
    //---------------  �E  ---------------//
    float4(+0.5f, -0.5f, -0.5f, 0), //����
    float4(+0.5f, +0.5f, -0.5f, 0), //����
    float4(+0.5f, -0.5f, +0.5f, 0), //�E��
    float4(+0.5f, +0.5f, +0.5f, 0), //�E��
    
    //---------------  ��  ---------------//
    float4(-0.5f, -0.5f, +0.5f, 0), //����
    float4(-0.5f, +0.5f, +0.5f, 0), //����
    float4(-0.5f, -0.5f, -0.5f, 0), //�E��
    float4(-0.5f, +0.5f, -0.5f, 0), //�E��
    
    //---------------  ��  ---------------//
    float4(-0.5f, -0.5f, -0.5f, 0), //����
    float4(-0.5f, -0.5f, +0.5f, 0), //����
    float4(+0.5f, -0.5f, -0.5f, 0), //�E��
    float4(+0.5f, -0.5f, +0.5f, 0), //�E��
    
    //---------------  ��  ---------------//
    float4(-0.5f, +0.5f, +0.5f, 0), //����
    float4(-0.5f, +0.5f, -0.5f, 0), //����
    float4(+0.5f, +0.5f, +0.5f, 0), //�E��
    float4(+0.5f, +0.5f, -0.5f, 0), //�E��
};

//���オ0,0�@�E����1,1
static const float2 uv_array[vnum] =
{
    //--------------- ��O ---------------//
    float2(0.0f, 1.0f), //����
    float2(0.0f, 0.0f), //����
    float2(1.0f, 1.0f), //�E��
    float2(1.0f, 0.0f), //�E��
    //---------------  ��  ---------------//
    float2(0.0f, 1.0f), //����
    float2(0.0f, 0.0f), //����
    float2(1.0f, 1.0f), //�E��
    float2(1.0f, 0.0f), //�E��
    //---------------  �E  ---------------//
    float2(0.0f, 1.0f), //����
    float2(0.0f, 0.0f), //����
    float2(1.0f, 1.0f), //�E��
    float2(1.0f, 0.0f), //�E��
    //---------------  ��  ---------------//
    float2(0.0f, 1.0f), //����
    float2(0.0f, 0.0f), //����
    float2(1.0f, 1.0f), //�E��
    float2(1.0f, 0.0f), //�E��
    //---------------  ��  ---------------//
    float2(0.0f, 1.0f), //����
    float2(0.0f, 0.0f), //����
    float2(1.0f, 1.0f), //�E��
    float2(1.0f, 0.0f), //�E��
    //---------------  ��  ---------------//
    float2(0.0f, 1.0f), //����
    float2(0.0f, 0.0f), //����
    float2(1.0f, 1.0f), //�E��
    float2(1.0f, 0.0f), //�E��
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element; //�o�͗p���_�f�[�^
   
    for (uint i = 0; i < vnum; i++)
    {
        //���S����̃I�t�Z�b�g���X�P�[�����O
        float4 offset = offset_array[i] * input[0].scale;
        //�I�t�Z�b�g�����炷(���[���h���W)
        element.svpos = input[0].pos + offset;
        
        //�r���[�A�ˉe�ϊ�
        element.svpos = mul(mat,element.svpos);
        element.uv = uv_array[i];
        element.color = input[0].color;
        //element.color = (0.0f, 0.0f, 0.0f, 1.0f);
        output.Append(element);      
    }
}