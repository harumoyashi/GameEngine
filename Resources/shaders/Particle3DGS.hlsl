#include "Particle3D.hlsli"

//四角形の頂点数
static const uint vnum = 24;

//センターからのオフセット
static const float4 offset_array[vnum] =
{
    //--------------- 手前 ---------------//
    float4(-0.5f, -0.5f, +0.5f, 0), //左下
    float4(-0.5f, +0.5f, +0.5f, 0), //左上
    float4(+0.5f, -0.5f, +0.5f, 0), //右下
    float4(+0.5f, +0.5f, +0.5f, 0), //右上
    
    //---------------  奥  ---------------//
    float4(+0.5f, -0.5f, -0.5f, 0), //左下
    float4(+0.5f, +0.5f, -0.5f, 0), //左上
    float4(-0.5f, -0.5f, -0.5f, 0), //右下
    float4(-0.5f, +0.5f, -0.5f, 0), //右上
    
    //---------------  右  ---------------//
    float4(+0.5f, -0.5f, -0.5f, 0), //左下
    float4(+0.5f, +0.5f, -0.5f, 0), //左上
    float4(+0.5f, -0.5f, +0.5f, 0), //右下
    float4(+0.5f, +0.5f, +0.5f, 0), //右上
    
    //---------------  左  ---------------//
    float4(-0.5f, -0.5f, +0.5f, 0), //左下
    float4(-0.5f, +0.5f, +0.5f, 0), //左上
    float4(-0.5f, -0.5f, -0.5f, 0), //右下
    float4(-0.5f, +0.5f, -0.5f, 0), //右上
    
    //---------------  上  ---------------//
    float4(-0.5f, -0.5f, -0.5f, 0), //左下
    float4(-0.5f, -0.5f, +0.5f, 0), //左上
    float4(+0.5f, -0.5f, -0.5f, 0), //右下
    float4(+0.5f, -0.5f, +0.5f, 0), //右上
    
    //---------------  下  ---------------//
    float4(-0.5f, +0.5f, +0.5f, 0), //左下
    float4(-0.5f, +0.5f, -0.5f, 0), //左上
    float4(+0.5f, +0.5f, +0.5f, 0), //右下
    float4(+0.5f, +0.5f, -0.5f, 0), //右上
};

//左上が0,0　右下が1,1
static const float2 uv_array[vnum] =
{
    //--------------- 手前 ---------------//
    float2(0.0f, 1.0f), //左下
    float2(0.0f, 0.0f), //左上
    float2(1.0f, 1.0f), //右下
    float2(1.0f, 0.0f), //右上
    //---------------  奥  ---------------//
    float2(0.0f, 1.0f), //左下
    float2(0.0f, 0.0f), //左上
    float2(1.0f, 1.0f), //右下
    float2(1.0f, 0.0f), //右上
    //---------------  右  ---------------//
    float2(0.0f, 1.0f), //左下
    float2(0.0f, 0.0f), //左上
    float2(1.0f, 1.0f), //右下
    float2(1.0f, 0.0f), //右上
    //---------------  左  ---------------//
    float2(0.0f, 1.0f), //左下
    float2(0.0f, 0.0f), //左上
    float2(1.0f, 1.0f), //右下
    float2(1.0f, 0.0f), //右上
    //---------------  上  ---------------//
    float2(0.0f, 1.0f), //左下
    float2(0.0f, 0.0f), //左上
    float2(1.0f, 1.0f), //右下
    float2(1.0f, 0.0f), //右上
    //---------------  下  ---------------//
    float2(0.0f, 1.0f), //左下
    float2(0.0f, 0.0f), //左上
    float2(1.0f, 1.0f), //右下
    float2(1.0f, 0.0f), //右上
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
        float4 offset = offset_array[i] * input[0].scale;
        //オフセット分ずらす(ワールド座標)
        element.svpos = input[0].pos + offset;
        
        //ビュー、射影変換
        element.svpos = mul(mat,element.svpos);
        element.uv = uv_array[i];
        element.color = input[0].color;
        //element.color = (0.0f, 0.0f, 0.0f, 1.0f);
        output.Append(element);      
    }
}