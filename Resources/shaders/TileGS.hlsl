#include "Tile.hlsli"

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
   
    for (uint i = 0; i < vnum; i++)
    {
         //法線にワールド行列によるスケーリング・回転を適用
        float4 wnormal = normalize(mul(world, float4(input[i].normal, 0)));
        float4 wpos = (0,0,0,0);
        
        //ワールド行列からスケールを抽出
        float x = sqrt(pow(world[0][0], 2) + pow(world[0][1], 2) + pow(world[0][2], 2));
        float y = sqrt(pow(world[1][0], 2) + pow(world[1][1], 2) + pow(world[1][2], 2));
        float z = sqrt(pow(world[2][0], 2) + pow(world[2][1], 2) + pow(world[2][2], 2));
    
        float3 scale = { x, y, z };
        
        float4 offset = (input[i].normal, 0);
    
        //ビュー、射影変換
        //オブジェクトに近いポリゴンほど高く浮く処理
        float3 centerPos =
        (input[0].pos.xyz + input[1].pos.xyz + input[2].pos.xyz) / 3.f; //ポリゴンの中心点
        float maxDist = 2.f; //浮く範囲
        
        float3 plusVec;
        for (uint j = 0; j < 1; j++)
        {
            float3 objToPolyVec = objPos[j] - centerPos; //オブジェクトとポリゴンの中心点とのベクトル
            float objToPolyDist = length(objToPolyVec); //オブジェクトとポリゴンの中心点との距離
            objToPolyDist = Clamp(objToPolyDist, 0.f, maxDist); //大きくなりすぎないように

            objToPolyDist = maxDist - objToPolyDist; //オブジェクトに近い程大きい値に
            
            objToPolyVec = normalize(objToPolyVec);
            
            plusVec = objToPolyVec * objToPolyDist; //最終的にプレイヤーから近いほど遠ざかるベクトルを足す
        }
        
        float3 plusPos = input[i].pos + plusVec;
        
        wpos = mul(world, float4(input[i].pos, 1));
        
        element.svpos = mul(mul(viewproj, world), float4(plusPos, 1));
        element.worldpos = wpos;
        //element.worldpos = mul(world, float4(input[i].pos, 1));
        element.normal = input[i].normal;
        element.uv = input[i].uv;
        element.scale = scale;
        
        output.Append(element);
    }
}