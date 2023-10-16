#include "Tile.hlsli"

float4 Vec4TimeMat(float4 v, float4x4 m)
{
    float4 result = v;
    result.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0];
    result.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1];
    result.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2];
    result.w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3];
    return result;
}

VSOutput main(VSOutput input)
{
    float4 svpos = mul(viewproj, input.svpos);
    float4 wpos = mul(world, svpos);
    input.svpos = wpos;
    input.normal = input.normal;
    input.uv = input.uv;
    
    return input;
}