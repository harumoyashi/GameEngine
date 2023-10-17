#include "Tile.hlsli"

//��x�ɂ����钸�_��
static const uint vnum = 3;

[maxvertexcount(vnum)]
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
        
        float4 offset = (input[i].normal, 0);
    
        //�r���[�A�ˉe�ϊ�
        element.worldpos = input[i].svpos + offset;
        element.normal = input[i].normal;
        element.uv = input[i].uv;
        element.scale = scale;
        
        output.Append(element);
    }
}