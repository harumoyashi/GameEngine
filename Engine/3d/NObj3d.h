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

	D3D12_HEAP_PROPERTIES heapProp{};	//ヒープ
	D3D12_RESOURCE_DESC resDesc{};		//リソース

	NConstBuff<ConstBuffDataTransform>* cbTrans;		//定数バッファのGPUリソースのポインタ

	NMatrix4 matWorld;	//3D変換行列

	// ライト
	static NDirectionalLight* directionalLight;
	static NPointLight* pointLight;
	static NSpotLight* spotLight;
	static NCircleShadow* circleShadow;

public:
	NVector3 scale = { 1.0f,1.0f,1.0f };	//スケーリング倍率
	NVector3 rotation = { 0.0f,0.0f,0.0f };	//回転角
	NVector3 position = { 0.0f,0.0f,0.0f };	//座標

	NObj3d* parent = nullptr;	//親のポインタ

	int texNum = 0;	//テクスチャ指定用

	NModel* model;

public:
	NObj3d();
	~NObj3d();
#pragma region 初期化まわり
	//初期化
	bool Init();

	NObj3d* Create();

#pragma endregion
#pragma region 更新まわり
	//キーボード操作
	void MoveKey();
	//ワールド行列の合成
	void UpdateMatrix();
	//定数バッファへ送信
	void TransferMatrix();
#pragma endregion
#pragma region 描画まわり
	//共通グラフィックスコマンド
	void CommonBeginDraw();
	//描画
	void Draw();
	//定数バッファビュー(CRV)の設定コマンド(マテリアル)
	void SetMaterialCBV(NMaterial material);
	void SetSRVHeap();
	void SetSRVHeap(D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	//頂点バッファビューの設定コマンド
	void SetVB(D3D12_VERTEX_BUFFER_VIEW vbView);
	//インデックスバッファビューの設定コマンド
	void SetIB(D3D12_INDEX_BUFFER_VIEW ibView);
	//定数バッファビュー(CRV)の設定コマンド(3D変換行列)
	void SetMatCBV();
	//描画コマンド
	void DrawCommand(UINT indexSize);
#pragma endregion
	inline void SetModel(NModel* model) { this->model = model; }
	//ライトのセット
	static void SetNDirectionalLight(NDirectionalLight* directionalLight) {
		NObj3d::directionalLight = directionalLight;
	}
	static void SetNPointLight(NPointLight* pointLight) {
		NObj3d::pointLight = pointLight;
	}
	static void SetNSpotLight(NSpotLight* spotLight) {
		NObj3d::spotLight = spotLight;
	}
	static void SetNCircleShadow(NCircleShadow* circleShadow) {
		NObj3d::circleShadow = circleShadow;
	}

	void SetMatWorld(NMatrix4 matWorld) { this->matWorld = matWorld; }
};