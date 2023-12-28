#pragma once
#include "NSprite.h"

// ポストエフェクトのひな形 //

class IPostEffect
	: public NSprite
{
protected:
	static NVertexBuff<NVertexUV> vertexBuff_;		//頂点バッファ

	//定数バッファまわり//
	static std::unique_ptr<NConstBuff<ConstBuffDataTransform2D>> cbTrans_;	//2D変換行列
	static std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor_;

	//行列//
	static NMatrix4 matWorld_;		//変換行列
	static NMatrix4 matProjection_;	//平行投影保管用

	//変換用//
	static float rotation_;		//Z軸の回転角
	static NVec2 position_;		//座標
	static NColor color_;		//色

	//テクスチャバッファ
	static const uint32_t texNum_ = 2;
	static ComPtr<ID3D12Resource> texBuff_[texNum_];
	//SRV用デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	//深度バッファ
	static ComPtr<ID3D12Resource> depthBuff_;
	//RTV用デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
	//DSV用デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

	//画面クリアカラー
	static const float kClearColor[4];

	//パイプラインの名前
	static std::string pipelineName_;

	static bool isActive_;	//ポストエフェクト有効フラグ

public:
	//コンストラクタ
	IPostEffect();
	//デストラクタ
	virtual ~IPostEffect() = default;

	//初期化
	static void Init();
	//更新
	static void Update();

	//テクスチャ生成
	static void CreateTexture();
	//レンダーターゲットビュー生成
	static void CreateRTV();
	//深度バッファ生成
	static void CreateDepthBuff();
	//デプスステンシルビュー生成
	static void CreateDSV();
	//描画
	static void Draw();

	//描画前処理
	static void PreDrawScene();
	//描画後処理
	static void PostDrawScene();

	// ゲッター //
	//ポストエフェクト有効フラグ
	static bool GetIsActive() { return isActive_; }

	// セッター //
	//ポストエフェクト有効フラグ設定
	static void SetIsActive(bool isActive) { isActive_ = isActive; }
};