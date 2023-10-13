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

VSOutput main(float3 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    VSOutput output; // ジオメトリシェーダに渡す値
    output.svpos = float4(pos, 1);
    output.normal = normal;
    output.uv = uv;
    
    return output;
}