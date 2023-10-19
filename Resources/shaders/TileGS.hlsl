#include "Tile.hlsli"

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
        //ワールド行列からスケールを抽出
        float x = sqrt(pow(world[0][0], 2) + pow(world[0][1], 2) + pow(world[0][2], 2));
        float y = sqrt(pow(world[1][0], 2) + pow(world[1][1], 2) + pow(world[1][2], 2));
        float z = sqrt(pow(world[2][0], 2) + pow(world[2][1], 2) + pow(world[2][2], 2));
    
        float3 scale = { x, y, z };
        
        float4 offset = (input[i].normal, 0);
    
        //ビュー、射影変換
        //オブジェクトに近いポリゴンほど高く浮く処理
        //for (uint i = 0; i < maxObj; i++)
        //{
        //    float　objToPolygon = (objPos[i] - 三角形の中心座標).length();
        //    if (objToPolygon < 一定の値)
        //    {
        //        element.svpos = input[i].svpos + float4(input[i].normal, 0) * (一定の値 - objToPolygon);

        //    }
        //}
        
        element.svpos = input[i].svpos + float4(input[i].normal, 0) * (float) i * divide;
        element.worldpos = input[i].worldpos;
        element.normal = input[i].normal;
        element.uv = input[i].uv;
        element.scale = scale;
        
        output.Append(element);
    }
}