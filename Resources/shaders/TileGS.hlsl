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
        centerPos = mul(world, float4(centerPos, 1)).xyz;               //���[���h���W�ɒ���
        float maxDist = 2.f; //�����͈�
        
        float3 objToPolyVec,plusVec;
        for (uint j = 0; j < 1; j++)
        {
            objToPolyVec = objPos[j] - centerPos;        //�I�u�W�F�N�g�ƃ|���S���̒��S�_�Ƃ̃x�N�g��
            float objToPolyDist = length(objToPolyVec);         //�I�u�W�F�N�g�ƃ|���S���̒��S�_�Ƃ̋���
            objToPolyDist = Clamp(objToPolyDist, 0.f, maxDist); //�傫���Ȃ肷���Ȃ��悤��

            objToPolyDist = maxDist - objToPolyDist;            //�I�u�W�F�N�g�ɋ߂����傫���l��
            
            objToPolyVec = normalize(objToPolyVec);
            
            plusVec = objToPolyVec * objToPolyDist;             //�ŏI�I�Ƀv���C���[����߂��قǉ�������x�N�g���𑫂�
            plusVec.y = -objToPolyDist * 0.2f;
        }
        
        //���[���h���W
        float4 wpos = mul(world, float4(input[i].pos, 1));
        //plusVec�����[���h���W������烏�[���h���W�ɒ��������̂ɑ���
        float3 plusPos = wpos.xyz + plusVec;
        
        //-------------------- ��] --------------------//
        //Z����]�s��
        float sinZ = sin(plusVec.z);
        float cosZ = cos(plusVec.z);

        float4x4 matZ = float4x4(
        cosZ, sinZ, 0, 0,
        -sinZ, cosZ, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
        
        //X����]�s��
        float sinX = sin(plusVec.x);
        float cosX = cos(plusVec.x);

        float4x4 matX = float4x4(
        1, 0, 0, 0,
        0, cosX, sinX, 0,
        0, -sinX, cosX, 0,
        0, 0, 0, 1);
        
        //Y����]�s��
        float sinY = sin(plusVec.y);
        float cosY = cos(plusVec.y);

        float4x4 matY = float4x4(
        cosY, 0, sinY, 0,
        0, 1, 0, 0,
        -sinY, 0, cosY, 0,
        0, 0, 0, 1);
        
        //��]�s��
        float4x4 rotMat = world;
        rotMat = mul(matZ, rotMat);
        rotMat = mul(matX, rotMat);
        rotMat = mul(matY, rotMat);
        
        float4 rotPos = mul(matZ, float4(plusPos, 1));
        rotPos = mul(matX, rotPos);
        rotPos = mul(matY, rotPos);
        
        //�������[���h���W�ɒ����Ă邩��V�X�e�����W�Ɋ|����̂̓r���[�s�񂾂�
        element.svpos = mul(viewproj, rotPos);
        element.worldpos = rotPos;
        element.normal = input[i].normal;
        element.uv = input[i].uv;
        element.scale = scale;
        
        output.Append(element);
    }
}