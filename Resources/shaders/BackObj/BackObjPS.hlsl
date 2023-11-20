#include "BackObj.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

PSOutput main(GSOutput input) : SV_TARGET
{
    PSOutput output;
    
	// テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv * float2(input.scale.x, input.scale.z));
    float z = sin(input.worldpos.z / (input.scale.z / 20.f));

    texcolor.rgb = float3(z, z, z);
    
	// 光沢度
    const float shininess = 4.0f;
	// 頂点から視点への方向ベクトル
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	
	// 環境反射光
    float3 ambient = m_ambient * ambientColor;
    
	// シェーディングによる色
    float4 shadecolor = float4(ambient, m_color.a);
	
    //平行光源
    for (uint i = 0; i < DIRLIGHT_NUM; i++)
    {
        // ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
	    // 反射光ベクトル
        float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
        // 拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
	    // 鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	    // 全て加算する
        shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor * dirLights[i].active; //アクティブの時のみ反映されるように(if文禁止！)
    }
    
     //点光源
    for (uint j = 0; j < POINTLIGHT_NUM; j++)
    {
        // ライトのベクトル
        float3 lightv = pointLights[j].lightpos - input.worldpos.xyz;
        //ベクトルの長さ
        float d = length(lightv);
        //正規化し、単位ベクトルにする
        lightv = normalize(lightv);
        //距離減衰係数
        float atten = 1.0f / (pointLights[j].lightatten.x + pointLights[j].lightatten.y * d +
            pointLights[j].lightatten.z * d * d);
        // ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(lightv, input.normal);
	    // 反射光ベクトル
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
        // 拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
	    // 鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	    // 全て加算する
        shadecolor.rgb += atten * (diffuse + specular) * pointLights[j].lightcolor * pointLights[j].active; //アクティブの時のみ反映されるように(if文禁止！)
    }
    
    //スポットライト
    for (uint k = 0; k < SPOTLIGHT_NUM; k++)
    {
        // ライトのベクトル
        float3 lightv = spotLights[k].lightpos - input.worldpos.xyz;
        //ベクトルの長さ
        float d = length(lightv);
        //正規化し、単位ベクトルにする
        lightv = normalize(lightv);
        //距離減衰係数
        float atten = saturate(1.0f / (spotLights[k].lightatten.x + spotLights[k].lightatten.y * d +
            spotLights[k].lightatten.z * d * d));
        //角度減衰
        float cos = dot(lightv, spotLights[k].lightv);
        //減衰開始角度から減衰終了角度にかけて減衰
        //減衰開始角度の内側は1倍　減衰終了角度の外側は0倍の輝度
        float angleatten = smoothstep(spotLights[k].lightfactoranglecos.y, spotLights[k].lightfactoranglecos.x, cos);
        //角度減衰を乗算
        atten *= angleatten;
        // ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(lightv, input.normal);
	    // 反射光ベクトル
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
        // 拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
	    // 鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	    // 全て加算する
        shadecolor.rgb += atten * (diffuse + specular) * spotLights[k].lightcolor * spotLights[k].active; //アクティブの時のみ反映されるように(if文禁止！)
    }
    
    //丸影
    for (uint l = 0; l < CIRCLESHADOW_NUM; l++)
    {
        //オブジェクト表面からキャスターへのベクトル
        float3 casterv = circleShadows[l].casterPos - input.worldpos.xyz;
        //投影方向での距離
        float d = dot(casterv, circleShadows[l].dir);
        //距離減衰係数
        float atten = saturate(1.0f / (circleShadows[l].atten.x + circleShadows[l].atten.y * d +
            circleShadows[l].atten.z * d * d));
        //距離がマイナスなら0にする
        atten *= step(0, d);
        //仮想ライトの座標
        float3 lightpos = circleShadows[l].casterPos + circleShadows[l].dir * circleShadows[l].distanceCasterLight;
        //オブジェクト表面からライトへのベクトル(単位ベクトル)
        float3 lightv = normalize(lightpos - input.worldpos.xyz);
        //角度減衰
        float cos = dot(lightv, circleShadows[l].dir);
        //減衰開始角度から減衰終了角度にかけて減衰
        //減衰開始角度の内側は1倍　減衰終了角度の外側は0倍の輝度
        float angleatten = smoothstep(circleShadows[l].factoranglecos.y, circleShadows[l].factoranglecos.x, cos);
        //角度減衰を乗算
        atten *= angleatten;
	    // 全て減算する
        shadecolor.rgb -= atten * circleShadows[l].active; //アクティブの時のみ反映されるように(if文禁止！)
    }

    
    // シェーディング色で描画
    float4 color = shadecolor * texcolor * m_color;
    color += 0.2f;
    output.target0 = color;
    output.target1 = color;
    return output;
}