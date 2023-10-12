#include "Tile.hlsli"

//�l�p�`�̒��_��
static const uint vnum = 6;

//�Z���^�[����̃I�t�Z�b�g
static const float3 offset_array[vnum] =
{
    float3(-0.5f, 0.f, -0.5f), //����
    float3(-0.5f, 0.f, +0.5f), //����
    float3(+0.5f, 0.f, +0.5f), //�E��
    
    float3(+0.5f, 0.f, -0.5f), //�E��
    float3(-0.5f, 0.f, -0.5f), //����
    float3(+0.5f, 0.f, +0.5f), //�E��
};

//���オ0,0�@�E����1,1
static const float2 uv_array[vnum] =
{
    float2(0.0f, 1.0f), //����
    float2(0.0f, 0.0f), //����
    float2(1.0f, 0.0f), //�E��
    
    float2(1.0f, 1.0f), //�E��
    float2(0.0f, 1.0f), //����
    float2(1.0f, 0.0f), //�E��
};

static const float3 normal_array[vnum] =
{
    float3(0.0f, -1.0f, 0.0f), //����
    float3(0.0f, -1.0f, 0.0f), //����
    float3(0.0f, -1.0f, 0.0f), //�E��
    
    float3(0.0f, -1.0f, 0.0f), //�E��
    float3(0.0f, -1.0f, 0.0f), //����
    float3(0.0f, -1.0f, 0.0f), //�E��
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
        float3 offset = offset_array[i] * input[0].scale;
        
        //�I�t�Z�b�g�����炷(���[���h���W)
        element.svpos = input[0].svpos + (offset, 0);
        
        //�r���[�A�ˉe�ϊ�
        element.svpos = mul(viewproj, element.svpos);
        element.worldpos = input[0].worldpos;
        element.normal = normal_array[i];
        element.uv = uv_array[i];
        element.scale = input[0].scale;
        element.divide = input[0].divide;
        element.activityArea = input[0].activityArea;
        
        output.Append(element);
    }
}