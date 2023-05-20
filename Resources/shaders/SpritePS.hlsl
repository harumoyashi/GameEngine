#include "Sprite.hlsli"

Texture2D<float4>tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);	//0番スロットに設定されたサンプラー

float3 main(VSOutput input) : SV_TARGET
{
    float4 col = (tex.Sample(smp, input.uv)) * color; //定数バッファに送った色変えると反映される
    return float3(col.x, col.y, col.z);

}