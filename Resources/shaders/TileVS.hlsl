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
    //法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, float4(pos, 1));
    
    VSOutput output; // ピクセルシェーダに渡す値
    output.svpos = mul(mul(viewproj, world), float4(pos, 1));
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = uv;
    
    //ワールド行列からスケールを抽出
    float x = sqrt(pow(world[0][0], 2) + pow(world[0][1], 2) + pow(world[0][2], 2));
    float y = sqrt(pow(world[1][0], 2) + pow(world[1][1], 2) + pow(world[1][2], 2));
    float z = sqrt(pow(world[2][0], 2) + pow(world[2][1], 2) + pow(world[2][2], 2));
    
    float3 scale = { x, y, z };
    
    output.scale = scale;
    output.divide = divide;
    output.activityArea = activityArea;
	
    return output;
}