#pragma once
#include "NDX12.h"
#include "NRootParam.h"
#include "NGPipeline.h"
#include "NMatrix4.h"
#include "NColor.h"
#include "NConstBuff.h"
#include "NVertex.h"
#include "NVertexBuff.h"

#include <d3dx12.h>
#include <wrl.h>

class NSprite
{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//頂点まわり//
	uint32_t singleSizeVB_;						//頂点バッファ1個当たりのサイズ
	uint32_t singleVB_;							//頂点バッファ全体のサイズ
	std::vector<NVertexUV> vertices_{4};		//頂点代入用
	D3D12_HEAP_PROPERTIES heapPropVert_{};	//ヒープ
	D3D12_RESOURCE_DESC resDescVert_{};		//リソース
	NVertexBuff vertexBuff_;
	NVertexUV* vertMap_ = nullptr;			//マップ用

	//定数バッファまわり//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform2D>> cbTrans_;	//2D変換行列
	std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor_;	//色

	//行列//
	NMatrix4 matWorld_{};		//変換行列
	NMatrix4 matProjection_{};	//平行投影保管用

	//生成時の設定用//
	NVector2 anchorPoint_ = { 0.5f,0.5f };	//アンカーポイント(0.0f~1.0fじゃないと画像から出る)
	bool isFlipX_ = false;	//左右反転フラグ
	bool isFlipY_ = false;	//上下反転フラグ
	NVector2 texLeftTop_ = { 0,0 };			//テクスチャ左上座標
	NVector2 texSize_ = { 100,100 };			//テクスチャ切り出しサイズ

	NVector2 size_ = {};		//スプライトの大きさ

public:
	//変換用//
	float rotation_ = 0.0f;	//Z軸の回転角
	NVector2 position_ = { 0.0f,0.0f };	//座標
	bool isInvisible_ = false;	//非表示にするフラグ

	std::string texHandle_ = "";	//テクスチャ指定用
	NColor color_;

public:
	NSprite();
	~NSprite();
#pragma region 初期化
	//スプライト生成
	void CreateSprite(const std::string& texHandle = "error");
	//テクスチャサイズに合わせてスプライト生成
	//アンカーポイントも設定できる(設定しないと中心になる)
	//上下左右の反転フラグも設定できる(設定しないと反転しない)
	void CreateSprite(const std::string& texHandle,
		const NVector2& anchorPoint, const bool isFlipX = false, const bool isFlipY = false);
	//テクスチャを切り取ってスプライト生成(アニメーション、フォントなど)
	//アンカーポイントも設定できる(設定しないと中心になる)
	//上下左右の反転フラグも設定できる(設定しないと反転しない)
	void CreateClipSprite(const std::string& texHandle, const NVector2& texLeftTop,
		const NVector2& texSize, const NVector2& anchorPoint = { 0.5f,0.5f }, const bool isFlipX = false, const bool isFlipY = false);

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
	void MatchTexSize(const ComPtr<ID3D12Resource>& texBuff);
	//アンカーポイント適用
	void SetAncor(const NVector2& anchorPoint);
	//上下左右反転フラグ適用
	void SetIsFlip(const bool isFlipX, const bool isFlipY);
	//切り抜き範囲適用
	void SetClipRange(const NVector2& texLeftTop, const NVector2& texSize);
	//切り抜かない場合テクスチャサイズに合わせる
	void SetClipRange();
	//頂点バッファビュー作成
	void CreateVertBuffView();
	
	//テクスチャハンドルをセット
	void SetTexHandle(const std::string& texHandle);
#pragma endregion
public:
#pragma region 更新
	//ワールド行列の合成
	void Update();
	//頂点バッファ転送
	void TransferVertex();
	//サイズ指定
	void SetSize(const float x, const float y);
	//座標指定
	void SetPos(const float x, const float y);
#pragma endregion
#pragma region 描画
	//共通グラフィックスコマンド
	void CommonBeginDraw();
	//描画コマンド
	void Draw();
#pragma endregion
	inline const NVector2& GetTexSize()const { return texSize_; }
	inline const NVector2& GetSize()const { return size_; }
};