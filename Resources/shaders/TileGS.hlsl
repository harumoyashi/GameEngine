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
	triangle VSOutput input[3],
	inout TriangleStream<GSOutput> output
)
{
    
    GSOutput element; //�o�͗p���_�f�[�^
   
    for (uint i = 0; i < vnum; i++)
    {
         //���[���h�s�񂩂�X�P�[���𒊏o
        float x = sqrt(pow(world[0][0], 2) + pow(world[0][1], 2) + pow(world[0][2], 2));
        float y = sqrt(pow(world[1][0], 2) + pow(world[1][1], 2) + pow(world[1][2], 2));
        float z = sqrt(pow(world[2][0], 2) + pow(world[2][1], 2) + pow(world[2][2], 2));
    
        float3 scale = { x, y, z };
    
        //���S����̃I�t�Z�b�g���X�P�[�����O
        float3 offset = offset_array[i] * scale;
        
        //�I�t�Z�b�g�����炷(���[���h���W)
        float4 svpos = input[0].svpos + (offset, 0);
        
        //�r���[�A�ˉe�ϊ�
        svpos = mul(viewproj, svpos);
        element.worldpos = mul(world, svpos);
        element.normal = normal_array[i];
        element.uv = uv_array[i];
        element.scale = scale;
        
        output.Append(element);
    }
}