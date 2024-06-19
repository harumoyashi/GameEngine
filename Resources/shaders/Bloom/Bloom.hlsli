//定数バッファ用構造体
cbuffer ConstBufferDataMatrix : register(b0)
{
    matrix mat;
};

cbuffer ConstBufferDataColor : register(b1)
{
    float4 color; //色(RGBA)
};


struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};

//ランダム
float random(float2 p, float extend = 1.0f);
float random(float2 p, float extend)
{
    return frac(sin(dot(p, float2(12.9898f, 78.233f))) * (43758.5453f + extend));
}

//ノイズ
float noise(float2 st, float extend = 1.0f);
float noise(float2 st, float extend)
{
    float2 p = floor(st);
    return random(p, extend); //
}