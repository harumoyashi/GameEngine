//�萔�o�b�t�@�p�\����
cbuffer ConstBufferDataMatrix : register(b0)
{
    matrix mat;
};

cbuffer ConstBufferDataColor : register(b1)
{
    float4 color; //�F(RGBA)
};


struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};

//�����_��
float random(float2 p, float extend = 1.0f);
float random(float2 p, float extend)
{
    return frac(sin(dot(p, float2(12.9898f, 78.233f))) * (43758.5453f + extend));
}

//�m�C�Y
float noise(float2 st, float extend = 1.0f);
float noise(float2 st, float extend)
{
    float2 p = floor(st);
    return random(p, extend); //
}