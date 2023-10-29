#include "BackObj.hlsli"

//��x�ɂ����钸�_��
static const uint vnum = 3;

[maxvertexcount(3)] //���(�O�p) * 1 + ����(�l�p) * 3 = 15(��x�ɂ����钸�_��)
void main(
	triangle VSOutput input[3], //�|���S�����`������O�p�`���Ƃɏ��������
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
    
        float3 wpos0 = mul(world, float4(input[0].pos, 1)).xyz;
        float3 wpos1 = mul(world, float4(input[1].pos, 1)).xyz;
        float3 wpos2 = mul(world, float4(input[2].pos, 1)).xyz;
    
        if (isAvoid)
        {
            //���[���h���W
            float4 wpos = mul(world, float4(input[i].pos, 1));
            
            //�������[���h���W�ɒ����Ă邩��V�X�e�����W�Ɋ|����̂̓r���[�s�񂾂�
            element.svpos = mul(viewproj, wpos);
            element.worldpos = wpos;
            element.normal = input[i].normal;
            element.uv = input[i].uv;
            element.scale = scale;
        }
        else
        {
            //���[���h���W
            float4 wpos = mul(world, float4(input[i].pos, 1));
            
            element.svpos = mul(viewproj, wpos);
            element.worldpos = wpos;
            element.normal = input[i].normal;
            element.uv = input[i].uv;
            element.scale = scale;
        }
        
        output.Append(element);
    }
}