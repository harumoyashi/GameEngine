#include "Tile.hlsli"

//四角形の頂点数
static const uint vnum = 6;

//センターからのオフセット
static const float3 offset_array[vnum] =
{
    float3(-0.5f, 0.f, -0.5f), //左下
    float3(-0.5f, 0.f, +0.5f), //左上
    float3(+0.5f, 0.f, +0.5f), //右上
    
    float3(+0.5f, 0.f, -0.5f), //右下
    float3(-0.5f, 0.f, -0.5f), //左下
    float3(+0.5f, 0.f, +0.5f), //右上
};

//左上が0,0　右下が1,1
static const float2 uv_array[vnum] =
{
    float2(0.0f, 1.0f), //左下
    float2(0.0f, 0.0f), //左上
    float2(1.0f, 0.0f), //右上
    
    float2(1.0f, 1.0f), //右下
    float2(0.0f, 1.0f), //左下
    float2(1.0f, 0.0f), //右上
};

static const float3 normal_array[vnum] =
{
    float3(0.0f, -1.0f, 0.0f), //左下
    float3(0.0f, -1.0f, 0.0f), //左上
    float3(0.0f, -1.0f, 0.0f), //右上
    
    float3(0.0f, -1.0f, 0.0f), //右下
    float3(0.0f, -1.0f, 0.0f), //左下
    float3(0.0f, -1.0f, 0.0f), //右上
};

[maxvertexcount(vnum)]
void main(
	triangle VSOutput input[3],
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
    
        //中心からのオフセットをスケーリング
        float3 offset = offset_array[i] * scale;
        
        //オフセット分ずらす(ワールド座標)
        float4 svpos = input[0].svpos + (offset, 0);
        
        //ビュー、射影変換
        svpos = mul(viewproj, svpos);
        element.worldpos = mul(world, svpos);
        element.normal = normal_array[i];
        element.uv = uv_array[i];
        element.scale = scale;
        
        output.Append(element);
    }
}