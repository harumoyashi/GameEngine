#include "Tile.hlsli"

VSOutput main(VSInput input)
{
    VSOutput element;
    
    float4 wnormal = normalize(mul(world, float4(input.normal, 0)));
    
    element.svpos = mul(mul(viewproj, world), float4(input.pos, 1));
    element.normal = wnormal.xyz;
    element.uv = input.uv;
    
    return element;
}