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
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element; //出力用頂点データ
   
    for (uint i = 0; i < vnum; i++)
    {
        //中心からのオフセットをスケーリング
        float3 offset = offset_array[i] * input[0].scale;
        
        //オフセット分ずらす(ワールド座標)
        element.svpos = input[0].svpos + (offset, 0);
        
        //ビュー、射影変換
        element.svpos = mul(viewproj, element.svpos);
        element.worldpos = input[0].worldpos;
        element.normal = normal_array[i];
        element.uv = uv_array[i];
        element.scale = input[0].scale;
        element.divide = input[0].divide;
        element.activityArea = input[0].activityArea;
        
        output.Append(element);
    }
}