//#include "GaussianBlur.hlsli"

//Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
//SamplerState smp : register(s0); //0番スロットに設定されたサンプラー


//float4 main(VSOutput input) : SV_TARGET
//{
//	 ////ガウシアンブラー//
//  //  float totalWeight = 0, _Sigma = 0.001, _StepWidth = 0.001; //Bloomはブラーを大げさに
//    float4 col = float4(0, 0, 0, 0);

//  //  for (float py = _Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
//  //  {
//  //      for (float px = _Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
//  //      {
//  //          float2 pickUV = input.uv + float2(px, py);
//  //          float weight = Gaussian(input.uv, pickUV, _Sigma);
//  //          col += float4(tex.Sample(smp, pickUV)) * weight; //Gaussianで取得した「重み」を色にかける
//  //          totalWeight += weight; //かけた「重み」の合計値を控えとく
//  //      }
//  //  }
//  //  col.rgb = col.rgb / totalWeight; //かけた「重み」分、結果から割る
//    return col;
//}

float4 main() : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}