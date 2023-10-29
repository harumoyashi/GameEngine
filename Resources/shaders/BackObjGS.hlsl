#include "BackObj.hlsli"

//一度にいじる頂点数
static const uint vnum = 3;

[maxvertexcount(3)] //上面(三角) * 1 + 側面(四角) * 3 = 15(一度にいじる頂点数)
void main(
	triangle VSOutput input[3], //ポリゴンを形成する三角形ごとに処理される
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element; //出力用頂点データ
    
    for (uint i = 0; i < vnum; i++)
    {
    //ワールド行列からスケールを抽出
        float x = sqrt(pow(world[0][0], 2) + pow(world[0][1], 2) + pow(world[0][2], 2));
        float y = sqrt(pow(world[1][0], 2) + pow(world[1][1], 2) + pow(world[1][2], 2));
        float z = sqrt(pow(world[2][0], 2) + pow(world[2][1], 2) + pow(world[2][2], 2));
    
        float3 scale = { x, y, z };
    
        float3 wpos0 = mul(world, float4(input[0].pos, 1)).xyz;
        float3 wpos1 = mul(world, float4(input[1].pos, 1)).xyz;
        float3 wpos2 = mul(world, float4(input[2].pos, 1)).xyz;
    
        if (isAvoid)
        {
            //ワールド座標
            float4 wpos = mul(world, float4(input[i].pos, 1));
            
            //もうワールド座標に直してるからシステム座標に掛けるのはビュー行列だけ
            element.svpos = mul(viewproj, wpos);
            element.worldpos = wpos;
            element.normal = input[i].normal;
            element.uv = input[i].uv;
            element.scale = scale;
        }
        else
        {
            //ワールド座標
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