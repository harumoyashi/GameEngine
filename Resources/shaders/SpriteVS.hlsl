#include "Sprite.hlsli"

VSOutput main(float3 pos:POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;	//ピクセルシェーダーに渡す値
    float4 p = (pos, 0);
	output.svpos = mul(mat, p);	//座標に行列を乗算
	output.uv = uv;
	return output;
}