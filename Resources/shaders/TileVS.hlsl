#include "Tile.hlsli"

VSOutput main(VSInput input)
{
    VSOutput element;
    
    //法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, float4(input.normal, 0)));
    float4 wpos = mul(world, float4(input.pos, 1));
    
    element.svpos = mul(mul(viewproj, world), float4(input.pos, 1));
    element.worldpos = wpos;
    element.normal = wnormal.xyz;
    element.uv = input.uv;
    
    return element;
}