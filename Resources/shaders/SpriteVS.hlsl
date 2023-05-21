#include "Sprite.hlsli"

VSOutput main(float4 pos:POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;	//ピクセルシェーダーに渡す値
    //float4 p = mul(mat, float4(pos, 1)); //座標に行列を乗算
    //output.svpos = float3(p.x,p.y,p.z);
    output.svpos = pos;
	output.uv = uv;
	return output;
}