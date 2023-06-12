#pragma once
#include "NDX12.h"
#include "NMaterial.h"
#include "NModel.h"
#include "NRootParam.h"
#include "NGPipeline.h"
#include "NMatrix4.h"
#include "NDirectionalLight.h"
#include "NPointLight.h"
#include "NSpotLight.h"
#include "NCircleShadow.h"
#include "NConstBuff.h"

#include<memory>
#include <wrl.h>

class NObj3d
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_HEAP_PROPERTIES heapProp_{};	//ヒープ
	D3D12_RESOURCE_DESC resDesc_{};		//リソース

	//定数バッファ//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans_;
	std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor_;
	std::unique_ptr<NConstBuff<ConstBuffDataMaterial>> cbMaterial_;

	NMatrix4 matWorld_;	//3D変換行列

	// ライト
	static NDirectionalLight* sDirectionalLight;
	static NPointLight* sPointLight;
	static NSpotLight* sSpotLight;
	static NCircleShadow* sCircleShadow;

public:
	NVector3 scale_ = { 1.0f,1.0f,1.0f };	//スケーリング倍率
	NVector3 rotation_ = { 0.0f,0.0f,0.0f };	//回転角
	NVector3 position_ = { 0.0f,0.0f,0.0f };	//座標

	NObj3d* parent_ = nullptr;	//親のポインタ

	uint32_t texNum_ = 0;	//テクスチャ指定用

	NModel model_;
	NColor color_;

public:
	NObj3d();
	~NObj3d();
#pragma region 初期化まわり
	//初期化
	bool Init();

	NObj3d* Create();

#pragma endregion
#pragma region 更新まわり
	//更新
	void Update();
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

#pragma endregion
#pragma region 描画まわり
	//共通グラフィックスコマンド
	void CommonBeginDraw();
	//描画
	void Draw();
	void SetSRVHeap();
	void SetSRVHeap(D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	//頂点バッファビューの設定コマンド
	void SetVB(D3D12_VERTEX_BUFFER_VIEW vbView);
	//インデックスバッファビューの設定コマンド
	void SetIB(D3D12_INDEX_BUFFER_VIEW ibView);
	//定数バッファビュー(CBV)の設定コマンド
	void SetCBV();
	//描画コマンド
	void DrawCommand(uint32_t indexSize);
#pragma endregion
	inline void SetModel(NModel model) { model_ = model; }
	//ライトのセット
	static void SetNDirectionalLight(NDirectionalLight* directionalLight) {
		NObj3d::sDirectionalLight = directionalLight;
	}
	static void SetNPointLight(NPointLight* pointLight) {
		NObj3d::sPointLight = pointLight;
	}
	static void SetNSpotLight(NSpotLight* spotLight) {
		NObj3d::sSpotLight = spotLight;
	}
	static void SetNCircleShadow(NCircleShadow* circleShadow) {
		NObj3d::sCircleShadow = circleShadow;
	}

	inline void SetMatWorld(NMatrix4 matWorld) { matWorld_ = matWorld; }
};