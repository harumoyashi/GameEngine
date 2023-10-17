#include "Tile.hlsli"

VSOutput main(VSInput input)
{
    VSOutput element;
    
    //�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(input.normal, 0)));
    float4 wpos = mul(world, float4(input.pos, 1));
    
    element.svpos = mul(mul(viewproj, world), float4(input.pos, 1));
    element.worldpos = wpos;
    element.normal = wnormal.xyz;
    element.uv = input.uv;
    
    return element;
}