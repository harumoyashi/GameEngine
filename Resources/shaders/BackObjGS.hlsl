#include "BackObj.hlsli"

float Clamp(float value, float min, float max)
{
    // 値が最大値を上回っていたら最大値を返す
    if (value >= max)
        return max;

	// 値が最小値を下回っていたら最小値を返す
    if (value <= min)
        return min;

	// どちらにも当てはまらなかったら値をそのまま返す
    return value;
};

//一度にいじる頂点数
static const uint vnum = 3;

[maxvertexcount(vnum)]
void main(
	triangle VSOutput input[3], //ポリゴンを形成する三角形ごとに処理される
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element; //出力用頂点データ
    
     //ふよふよタイマー回す
    float floatingTimer = 0, maxFloatingTimer = 0;
    bool isTimerPlus = true; //タイマー足すか引くかフラグ
        
    if (maxFloatingTimer <= 0)
    {
        maxFloatingTimer = 120.f;
    }
        
    if (isTimerPlus)
    {
        floatingTimer++;
    }
    else
    {
        floatingTimer--;
    }
    
    if (isTimerPlus && floatingTimer >= maxFloatingTimer)
    {
        isTimerPlus = false;
    }
    else if (isTimerPlus == false && floatingTimer <= 0)
    {
        isTimerPlus = true;
    }
   
    for (uint i = 0; i < vnum; i++)
    {
         //法線にワールド行列によるスケーリング・回転を適用
        float4 wnormal = normalize(mul(world, float4(input[i].normal, 0)));
        
        //ワールド行列からスケールを抽出
        float x = sqrt(pow(world[0][0], 2) + pow(world[0][1], 2) + pow(world[0][2], 2));
        float y = sqrt(pow(world[1][0], 2) + pow(world[1][1], 2) + pow(world[1][2], 2));
        float z = sqrt(pow(world[2][0], 2) + pow(world[2][1], 2) + pow(world[2][2], 2));
    
        float3 scale = { x, y, z };
        
        //ビュー、射影変換
        //オブジェクトに近いポリゴンほど高く浮く処理
        float3 centerPos =
        (input[0].pos + input[1].pos + input[2].pos) / 3.f; //ポリゴンの中心点
        centerPos = mul(world, float4(centerPos, 1)).xyz; //ワールド座標に直す
        float maxDist = 2.f; //浮く範囲
        
        float3 objToPolyVec, plusVec;
        float objToPolyDist;
        
        objToPolyVec = centerPos; //オブジェクトとポリゴンの中心点とのベクトル
        objToPolyDist = length(objToPolyVec); //オブジェクトとポリゴンの中心点との距離
        objToPolyDist = Clamp(objToPolyDist, 0.f, maxDist); //大きくなりすぎないように

        objToPolyDist = maxDist - objToPolyDist; //オブジェクトに近い程大きい値に
            
        objToPolyVec = normalize(objToPolyVec);
            
        plusVec = objToPolyVec * objToPolyDist; //最終的にプレイヤーから近いほど遠ざかるベクトルを足す
        plusVec.y = -abs(objToPolyDist) * 0.5f;
        
        ////浮いてるならさらにふよふよさせる
        //if (objToPolyDist > 0)
        //{
        //    plusVec += (floatingTimer / maxFloatingTimer) * objToPolyVec * 0.2f;
        //}
        
        if (isAvoid)
        {
            //ワールド座標
            float4 wpos = mul(world, float4(input[i].pos, 1));
            //plusVecがワールド座標基準だからワールド座標に直したものに足す
            float3 plusPos = wpos.xyz + plusVec;
            
            //もうワールド座標に直してるからシステム座標に掛けるのはビュー行列だけ
            element.svpos = mul(viewproj, float4(plusPos, 1));
            element.worldpos = float4(plusPos, 1);
            element.normal = input[i].normal;
            element.uv = input[i].uv;
            element.scale = scale;
        }
        else
        {
            //ワールド座標
            float4 wpos = mul(world, float4(input[i].pos, 1));
            
            element.svpos = mul(viewproj, wpos);
            element.worldpos = wpos;
            element.normal = input[i].normal;
            element.uv = input[i].uv;
            element.scale = scale;
        }
        
        output.Append(element);
    }
}