#include "Tile.hlsli"

float Clamp(float value, float min, float max)
{
    // �l���ő�l�������Ă�����ő�l��Ԃ�
    if (value >= max)
        return max;

	// �l���ŏ��l��������Ă�����ŏ��l��Ԃ�
    if (value <= min)
        return min;

	// �ǂ���ɂ����Ă͂܂�Ȃ�������l�����̂܂ܕԂ�
    return value;
};

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
         //�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
        float4 wnormal = normalize(mul(world, float4(input[i].normal, 0)));
        float4 wpos = (0,0,0,0);
        
        //���[���h�s�񂩂�X�P�[���𒊏o
        float x = sqrt(pow(world[0][0], 2) + pow(world[0][1], 2) + pow(world[0][2], 2));
        float y = sqrt(pow(world[1][0], 2) + pow(world[1][1], 2) + pow(world[1][2], 2));
        float z = sqrt(pow(world[2][0], 2) + pow(world[2][1], 2) + pow(world[2][2], 2));
    
        float3 scale = { x, y, z };
        
        float4 offset = (input[i].normal, 0);
    
        //�r���[�A�ˉe�ϊ�
        //�I�u�W�F�N�g�ɋ߂��|���S���قǍ�����������
        float3 centerPos =
        (input[0].pos.xyz + input[1].pos.xyz + input[2].pos.xyz) / 3.f; //�|���S���̒��S�_
        float maxDist = 2.f; //�����͈�
        
        float3 plusVec;
        for (uint j = 0; j < 1; j++)
        {
            float3 objToPolyVec = objPos[j] - centerPos; //�I�u�W�F�N�g�ƃ|���S���̒��S�_�Ƃ̃x�N�g��
            float objToPolyDist = length(objToPolyVec); //�I�u�W�F�N�g�ƃ|���S���̒��S�_�Ƃ̋���
            objToPolyDist = Clamp(objToPolyDist, 0.f, maxDist); //�傫���Ȃ肷���Ȃ��悤��

            objToPolyDist = maxDist - objToPolyDist; //�I�u�W�F�N�g�ɋ߂����傫���l��
            
            objToPolyVec = normalize(objToPolyVec);
            
            plusVec = objToPolyVec * objToPolyDist; //�ŏI�I�Ƀv���C���[����߂��قǉ�������x�N�g���𑫂�
        }
        
        float3 plusPos = input[i].pos + plusVec;
        
        wpos = mul(world, float4(input[i].pos, 1));
        
        element.svpos = mul(mul(viewproj, world), float4(plusPos, 1));
        element.worldpos = wpos;
        //element.worldpos = mul(world, float4(input[i].pos, 1));
        element.normal = input[i].normal;
        element.uv = input[i].uv;
        element.scale = scale;
        
        output.Append(element);
    }
}