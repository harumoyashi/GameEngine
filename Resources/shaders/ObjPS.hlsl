#include "OBJ.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input) : SV_TARGET
{
    PSOutput output;
    
	// テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);
    
	// 光沢度
    const float shininess = 4.0f;
	// 頂点から視点への方向ベクトル
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	
	// 環境反射光
    float3 ambient = m_ambient;
    
	// シェーディングによる色
    float4 shadecolor = float4(ambient, m_color.a);
	
    //平行光源
    if (dirLight.active)
    {
            // ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(dirLight.lightv, input.normal);
	        // 反射光ベクトル
        float3 reflect = normalize(-dirLight.lightv + 2 * dotlightnormal * input.normal);
            // 拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
	        // 鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	        // 全て加算する
        shadecolor.rgb += (diffuse + specular) * dirLight.lightcolor;
        //shadecolor.a = m_color.a;
    }
    
    //点光源
    if (pointLight.active)
    {
            // ライトのベクトル
        float3 lightv = pointLight.lightpos - input.worldpos.xyz;
            //ベクトルの長さ
        float d = length(lightv);
            //正規化し、単位ベクトルにする
        lightv = normalize(lightv);
            //距離減衰係数
        float atten = 1.0f / (pointLight.lightatten.x + pointLight.lightatten.y * d +
            pointLight.lightatten.z * d * d);
            // ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(lightv, input.normal);
	        // 反射光ベクトル
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
            // 拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
	        // 鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	        // 全て加算する
        shadecolor.rgb += atten * (diffuse + specular) * pointLight.lightcolor;
        //shadecolor.a = m_color.a;
    }
    
    //スポットライト
    if (spotLight.active)
    {
            // ライトのベクトル
        float3 lightv = spotLight.lightpos - input.worldpos.xyz;
            //ベクトルの長さ
        float d = length(lightv);
            //正規化し、単位ベクトルにする
        lightv = normalize(lightv);
            //距離減衰係数
        float atten = saturate(1.0f / (spotLight.lightatten.x + spotLight.lightatten.y * d +
            spotLight.lightatten.z * d * d));
            //角度減衰
        float cos = dot(lightv, spotLight.lightv);
            //減衰開始角度から減衰終了角度にかけて減衰
            //減衰開始角度の内側は1倍　減衰終了角度の外側は0倍の輝度
        float angleatten = smoothstep(spotLight.lightfactoranglecos.y, spotLight.lightfactoranglecos.x, cos);
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
        shadecolor.rgb += atten * (diffuse + specular) * spotLight.lightcolor;
        //shadecolor.a = m_color.a;
    }
    
    //丸影
    if (circleShadow.active)
    {
            //オブジェクト表面からキャスターへのベクトル
        float3 casterv = circleShadow.casterPos - input.worldpos.xyz;
            //投影方向での距離
        float d = dot(casterv, circleShadow.dir);
            //距離減衰係数
        float atten = saturate(1.0f / (circleShadow.atten.x + circleShadow.atten.y * d +
            circleShadow.atten.z * d * d));
            //距離がマイナスなら0にする
        atten *= step(0, d);
            //仮想ライトの座標
        float3 lightpos = circleShadow.casterPos + circleShadow.dir * circleShadow.distanceCasterLight;
            //オブジェクト表面からライトへのベクトル(単位ベクトル)
        float3 lightv = normalize(lightpos - input.worldpos.xyz);
            //角度減衰
        float cos = dot(lightv, circleShadow.dir);
            //減衰開始角度から減衰終了角度にかけて減衰
            //減衰開始角度の内側は1倍　減衰終了角度の外側は0倍の輝度
        float angleatten = smoothstep(circleShadow.factoranglecos.y, circleShadow.factoranglecos.x, cos);
            //角度減衰を乗算
        atten *= angleatten;
	        // 全て減算する
        shadecolor.rgb -= atten;
    }

    // シェーディング色で描画
    float4 color = shadecolor * texcolor * m_color;
    output.target0 = color;
    output.target1 = color;
    return output;
    //return shadecolor * texcolor * m_color;
}