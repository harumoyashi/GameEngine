cbuffer cbuff0 : register(b0)
{
    matrix mat; //ビュープロジェクション行列
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 pos : POSITION;      //システム用頂点座標
    float3 rot : ROT;           //回転情報
    float4 color : COLOR;       //色
    float scale : SCALE;        //スケール
};

struct GSOutput
{
    float4 svpos : SV_POSITION; //システム用頂点座標
    float4 color : COLOR;       //色
    float2 uv : TEXCOORD;       //uv値
};
