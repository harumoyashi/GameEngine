#pragma once
#include "NDX12.h"
#include "NRootParam.h"
#include "NGPipeline.h"
#include "NMatrix4.h"
#include "NColor.h"
#include "NConstBuff.h"
#include "NVertex.h"

#include <d3dx12.h>
#include <wrl.h>

class NSprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//頂点まわり//
	uint32_t singleSizeVB;						//頂点バッファ1個当たりのサイズ
	uint32_t sizeVB;							//頂点バッファ全体のサイズ
	NVertexUV vertices[4]{};				//頂点代入用
	D3D12_HEAP_PROPERTIES heapPropVert{};	//ヒープ
	D3D12_RESOURCE_DESC resDescVert{};		//リソース
	ComPtr<ID3D12Resource> vertBuff;		//頂点バッファ
	NVertexUV* vertMap = nullptr;			//マップ用
	D3D12_VERTEX_BUFFER_VIEW vbView{};		//頂点バッファビュー

	//定数バッファまわり//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform2D>> cbTrans;	//2D変換行列
	std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor;	//色

	//行列//
	NMatrix4 matWorld{};		//変換行列
	NMatrix4 matProjection{};	//平行投影保管用

	//生成時の設定用//
	NVector2 anchorPoint = { 0.5f,0.5f };	//アンカーポイント(0.0f~1.0fじゃないと画像から出る)
	bool isFlipX = false;	//左右反転フラグ
	bool isFlipY = false;	//上下反転フラグ
	NVector2 texLeftTop = { 0,0 };			//テクスチャ左上座標
	NVector2 texSize = { 100,100 };			//テクスチャ切り出しサイズ

	NVector2 size = {};		//スプライトの大きさ

public:
	//変換用//
	float rotation = 0.0f;	//Z軸の回転角
	NVector2 position = { 0.0f,0.0f };	//座標
	bool isInvisible = false;	//非表示にするフラグ

	std::string texHandle = "";	//テクスチャ指定用
	NColor color;

public:
	NSprite();
	~NSprite();
#pragma region 初期化
	//スプライト生成
	void CreateSprite(std::string texHandle = "error");
	//テクスチャサイズに合わせてスプライト生成
	//アンカーポイントも設定できる(設定しないと中心になる)
	//上下左右の反転フラグも設定できる(設定しないと反転しない)
	void CreateSprite(std::string texHandle,
		NVector2 anchorPoint, bool isFlipX = false, bool isFlipY = false);
	//テクスチャを切り取ってスプライト生成(アニメーション、フォントなど)
	//アンカーポイントも設定できる(設定しないと中心になる)
	//上下左右の反転フラグも設定できる(設定しないと反転しない)
	void CreateClipSprite(std::string texHandle, NVector2 texLeftTop,
		NVector2 texSize, NVector2 anchorPoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

private:
	//頂点データ設定
	void SetVert();
	//頂点用ヒープ設定
	void SetVertHeap();
	//頂点用リソース設定
	void SetVertResource();
	//バッファ作成
	void CreateVertBuff();
	//テクスチャサイズに合わせる
	void MatchTexSize(ComPtr<ID3D12Resource> texBuff);
	//アンカーポイント適用
	void SetAncor(NVector2 anchorPoint);
	//上下左右反転フラグ適用
	void SetIsFlip(bool isFlipX, bool isFlipY);
	//切り抜き範囲適用
	void SetClipRange(NVector2 texLeftTop, NVector2 texSize);
	//切り抜かない場合テクスチャサイズに合わせる
	void SetClipRange();
	//マッピング
	void VertMaping();
	//頂点バッファビュー作成
	void CreateVertBuffView();
	
	//テクスチャハンドルをセット
	void SetTexHandle(std::string texHandle);
#pragma endregion
public:
#pragma region 更新
	//ワールド行列の合成
	void Update();
	//頂点バッファ転送
	void TransferVertex();
	//サイズ指定
	void SetSize(float x, float y);
	//座標指定
	void SetPos(float x, float y);
#pragma endregion
#pragma region 描画
	//共通グラフィックスコマンド
	void CommonBeginDraw();
	//描画コマンド
	void Draw();
#pragma endregion
	inline NVector2 GetTexSize() { return texSize; }
	inline NVector2 GetSize() { return size; }
};