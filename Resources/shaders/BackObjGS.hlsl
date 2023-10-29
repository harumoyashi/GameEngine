#include "BackObj.hlsli"

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
    
     //�ӂ�ӂ�^�C�}�[��
    float floatingTimer = 0, maxFloatingTimer = 0;
    bool isTimerPlus = true; //�^�C�}�[�������������t���O
        
    if (maxFloatingTimer <= 0)
    {
        maxFloatingTimer = 120.f;
    }
        
    if (isTimerPlus)
    {
        floatingTimer++;
    }
    else
    {
        floatingTimer--;
    }
    
    if (isTimerPlus && floatingTimer >= maxFloatingTimer)
    {
        isTimerPlus = false;
    }
    else if (isTimerPlus == false && floatingTimer <= 0)
    {
        isTimerPlus = true;
    }
   
    for (uint i = 0; i < vnum; i++)
    {
         //�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
        float4 wnormal = normalize(mul(world, float4(input[i].normal, 0)));
        
        //���[���h�s�񂩂�X�P�[���𒊏o
        float x = sqrt(pow(world[0][0], 2) + pow(world[0][1], 2) + pow(world[0][2], 2));
        float y = sqrt(pow(world[1][0], 2) + pow(world[1][1], 2) + pow(world[1][2], 2));
        float z = sqrt(pow(world[2][0], 2) + pow(world[2][1], 2) + pow(world[2][2], 2));
    
        float3 scale = { x, y, z };
        
        //�r���[�A�ˉe�ϊ�
        //�I�u�W�F�N�g�ɋ߂��|���S���قǍ�����������
        float3 centerPos =
        (input[0].pos + input[1].pos + input[2].pos) / 3.f; //�|���S���̒��S�_
        centerPos = mul(world, float4(centerPos, 1)).xyz; //���[���h���W�ɒ���
        float maxDist = 2.f; //�����͈�
        
        float3 objToPolyVec, plusVec;
        float objToPolyDist;
        
        objToPolyVec = centerPos; //�I�u�W�F�N�g�ƃ|���S���̒��S�_�Ƃ̃x�N�g��
        objToPolyDist = length(objToPolyVec); //�I�u�W�F�N�g�ƃ|���S���̒��S�_�Ƃ̋���
        objToPolyDist = Clamp(objToPolyDist, 0.f, maxDist); //�傫���Ȃ肷���Ȃ��悤��

        objToPolyDist = maxDist - objToPolyDist; //�I�u�W�F�N�g�ɋ߂����傫���l��
            
        objToPolyVec = normalize(objToPolyVec);
            
        plusVec = objToPolyVec * objToPolyDist; //�ŏI�I�Ƀv���C���[����߂��قǉ�������x�N�g���𑫂�
        plusVec.y = -abs(objToPolyDist) * 0.5f;
        
        ////�����Ă�Ȃ炳��ɂӂ�ӂ悳����
        //if (objToPolyDist > 0)
        //{
        //    plusVec += (floatingTimer / maxFloatingTimer) * objToPolyVec * 0.2f;
        //}
        
        if (isAvoid)
        {
            //���[���h���W
            float4 wpos = mul(world, float4(input[i].pos, 1));
            //plusVec�����[���h���W������烏�[���h���W�ɒ��������̂ɑ���
            float3 plusPos = wpos.xyz + plusVec;
            
            //�������[���h���W�ɒ����Ă邩��V�X�e�����W�Ɋ|����̂̓r���[�s�񂾂�
            element.svpos = mul(viewproj, float4(plusPos, 1));
            element.worldpos = float4(plusPos, 1);
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