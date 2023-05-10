#include "CG5.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    // テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);

    //点光源
    if (pointLights[0].active == true)
    {
        //ライトのベクトル
        float3 lightv = pointLights[0].lightpos - input.worldpos.xyz;
        //ベクトルの長さ
        float d = length(lightv);
        //正規化し、単位ベクトルにする
        //lightv = normalize(lightv);
        //距離減衰係数
        float atten = 1.0f / (pointLights[0].lightatten.x + pointLights[0].lightatten.y * d +
            pointLights[0].lightatten.z * d * d);
           
	    // ライトに向かうベクトルと法線の内積
        //float3 dotlightnormal = dot(lightv, input.normal);
	    // 反射光ベクトル
        //float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
        // 拡散反射光
        //float3 diffuse = dotlightnormal * m_diffuse;
	    // 鏡面反射光
        //float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	    // 全て加算する
        //shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor * m_color;
        //shadecolor.a = m_alpha;
        
	    //環境光
        float4 _LightColor = (pointLights[0].lightcolor, 1);
        float4 ambient = texcolor * 0.3f * _LightColor;

	    //拡散反射光
	    //intensity = 明るさ
	    //90°超えると内積の値がマイナスになるので、saturateで0~1にする
        //float4 LightPos = (lightpos, 0);
        float intensity = saturate(dot(normalize(input.normal), pointLights[0].lightpos));
				
	        //明るさが一定以上なら明るく、それ未満なら暗く
        float _Threshold = 0.1f;
        float t = _Threshold; //閾値
        float u = _Threshold + 0.05f; //閾値
        intensity = smoothstep(t, u, intensity); //とぅーーーーん

        float4 diffuse = texcolor * intensity * _LightColor;

	    //鏡面反射光
	    //カメラの座標から、ワールド変換した頂点座標を引くことで、頂点からカメラまでのベクトルを算出する
        //頂点から視点への方向ベクトル
        float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
        float3 lightDir = normalize(pointLights[0].lightpos);
        input.normal = normalize(input.normal);
        float3 reflecDir = -lightDir + 2 * input.normal * dot(input.normal, lightDir);
        float4 specular = pow(saturate(dot(reflecDir, eyedir)), 20) * _LightColor;

	    //視線と法線の内積が小さい時だけ光らせるように
        float4 rimlight = 1.0f - saturate(dot(eyedir, input.normal));
        
	    //トゥーンっぽくするのもよき
	    //明るさが一定以上なら明るく、それ未満なら暗く
        float t2 = _Threshold; //閾値
        float u2 = _Threshold + 0.05f;
        rimlight = smoothstep(t, u, rimlight);
        
	    //powで値を絞るとそれっぽく
        float _Pow = 4.0f;
        rimlight = pow(rimlight, _Pow);

        float4 _RimColor = (1, 0, 0, 1);
        //float4 ads = atten * (1.0f - rimlight) * (ambient + diffuse + specular) + rimlight * _RimColor;
        //float4 ads = (1.0f - rimlight) * (ambient + diffuse + specular) + rimlight * _RimColor;
        //float4 ads = ambient + diffuse + specular;
        float4 ads = ambient;

        return ads;
    }
    else
    {
        return texcolor;
    }
}