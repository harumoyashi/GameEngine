#include "Fbx.hlsli"

SkinOutput ComputeSkin(VSInputPNUBone vsInput)
{
    SkinOutput output = (SkinOutput) 0;
    
    uint iBone; // �v�Z����{�[���ԍ�
    float weight; // �{�[���E�F�C�g�i�d�݁j
    matrix skinMat; // �X�L�j���O�s��
    
    // �{�[��0
    iBone = vsInput.boneIndices.x;
    weight = vsInput.boneWeights.x;
    skinMat = skinningMat[iBone];
    output.pos += weight * mul(skinMat, vsInput.pos);
    output.normal += weight * mul((float3x3) skinMat, vsInput.normal);
    
    // �{�[��1
    iBone = vsInput.boneIndices.y;
    weight = vsInput.boneWeights.y;
    skinMat = skinningMat[iBone];
    output.pos += weight * mul(skinMat, vsInput.pos);
    output.normal += weight * mul((float3x3) skinMat, vsInput.normal);
    
    // �{�[��2
    iBone = vsInput.boneIndices.z;
    weight = vsInput.boneWeights.z;
    skinMat = skinningMat[iBone];
    output.pos += weight * mul(skinMat, vsInput.pos);
    output.normal += weight * mul((float3x3) skinMat, vsInput.normal);
    
    // �{�[��3
    iBone = vsInput.boneIndices.w;
    weight = vsInput.boneWeights.w;
    skinMat = skinningMat[iBone];
    output.pos += weight * mul(skinMat, vsInput.pos);
    output.normal += weight * mul((float3x3) skinMat, vsInput.normal);
    
    return output;
}

VSOutput main(VSInputPNUBone vsInput)
{
    // �X�L�j���O�v�Z
    SkinOutput skinned = ComputeSkin(vsInput);
    
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p		
    float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));
    float4 wpos = mul(world, skinned.pos);
    float4 vertexPos = mul(mul(viewproj, world), skinned.pos);

    // �s�N�Z���V�F�[�_�[�ɓn���l
    VSOutput output = (VSOutput) 0;
    output.svpos = vertexPos;
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = vsInput.uv;
	
    return output;
}