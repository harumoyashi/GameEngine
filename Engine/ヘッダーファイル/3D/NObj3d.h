#pragma once
#include "NDX12.h"
#include "NMaterial.h"
#include "NModelManager.h"
#include "NRootParam.h"
#include "NGPipeline.h"
#include "NMatrix4.h"
#include "NLightGroup.h"
#include "NConstBuff.h"

#include<memory>
#include <wrl.h>

class NBaseCollider;

class NObj3d
{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_HEAP_PROPERTIES heapProp_{};	//ヒープ
	D3D12_RESOURCE_DESC resDesc_{};		//リソース

	//定数バッファ//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans_;
	std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor_;
	std::unique_ptr<NConstBuff<ConstBuffDataMaterial>> cbMaterial_;
	std::unique_ptr<NConstBuff<ConstBuffDataSkin>> cbSkin_;

	NMatrix4 matWorld_;	//3D変換行列

	NTexture* texture_;				//テクスチャ
	IModel* model_;					//モデル
	static ModelFormat modelFormat;	//モデルの形式格納用
	std::string objName_;			//デバッグ用に名前つける

	// ライト
	static NLightGroup* sLightGroup;

public:
	NVec3 scale_ = { 1.0f,1.0f,1.0f };		//スケーリング倍率
	NVec3 rotation_ = { 0.0f,0.0f,0.0f };	//回転角
	NVec3 position_ = { 0.0f,0.0f,0.0f };	//座標

	NObj3d* parent_ = nullptr;	//親のポインタ
	NColor color_;				//色

public:
	NObj3d();
	virtual ~NObj3d();
	//初期化
	virtual bool Init();
#pragma region 更新まわり
	//更新
	virtual void Update();
	//キーボード操作
	void MoveKey();
	//ワールド行列の合成
	void UpdateMatrix();
	//定数バッファへ送信
	void TransferMatrix();
	//色情報転送
	void TransferColor();
	//光情報転送
	void TransferMaterial();
	//スキン情報転送(FBXのみ)
	void TransferSkin();
#pragma endregion
#pragma region 描画まわり
	//共通グラフィックスコマンド
	static void CommonBeginDraw();
	//ブレンドモード設定
	static void SetBlendMode(BlendMode blendMode);
	//描画
	virtual void Draw();
	//シェーダーリソースビューの設定コマンド
	void SetSRVHeap(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
	//頂点バッファビューの設定コマンド
	void SetVB(const D3D12_VERTEX_BUFFER_VIEW* vbView);
	//インデックスバッファビューの設定コマンド
	void SetIB(const D3D12_INDEX_BUFFER_VIEW& ibView);
	//定数バッファビュー(CBV)の設定コマンド
	void SetCBV();
	//描画コマンド
	void DrawCommand(const uint32_t indexSize);
#pragma endregion

	// ゲッター //
	//ワールド行列取得
	const NMatrix4& GetMatWorld() { return matWorld_; }

	// セッター //
	//モデルの設定
	void SetModel(const std::string& modelname);
	//テクスチャの設定
	void SetTexture(const std::string& texname);
	//ワールド行列設定
	void SetMatWorld(const NMatrix4& matWorld) { matWorld_ = matWorld; }
	//ライトを設定
	static void SetLightGroup(NLightGroup* lightGroup) { sLightGroup = lightGroup; }
};