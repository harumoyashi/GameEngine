#pragma once
#include <DirectXMath.h>
#include "NDX12.h"
#include "NTexture.h"
#include "NRootSignature.h"
#include "NRootParam.h"
#include "NShader.h"
#include <d3dx12.h>

#include <wrl.h>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

struct PipelineDesc
{
	struct Shader {
		NShader* pShader;
	} shader;

	struct Blend {
		//網羅率考慮してブレンドするか
		bool isAlphaToCoverage = false;
		//それぞれのレンダーターゲットに別々のブレンドするか
		bool isIndependentBlend = false;
		//マスク値：RBGA全てのチャンネルを描画
		UINT8 RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		//ブレンドを有効にするかフラグ
		bool isBlend = true;
		//論理演算を有効にするかフラグ
		bool isLogicOp = false;

		//ブレンド情報
		struct BlendDesc {			//何もいじらんとαブレンドになる
			D3D12_BLEND_OP BlendOpAlpha = D3D12_BLEND_OP_ADD;
			D3D12_BLEND SrcBlendAlpha = D3D12_BLEND_ONE;
			D3D12_BLEND DestBlendAlpha = D3D12_BLEND_ZERO;

			D3D12_BLEND_OP BlendOp = D3D12_BLEND_OP_ADD;
			D3D12_BLEND SrcBlend = D3D12_BLEND_SRC_ALPHA;
			D3D12_BLEND DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		}blendDesc;
	} blend;

	struct Render {
		//トライアングルストリップを切り離すかどうか
		D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBStripCutValue =
			D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;		//カットなし
		//トポロジー指定
		D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopologyType =
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//-------------------- サンプルマスク --------------------//
		UINT SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
		//-------------------- ラスタライザ --------------------//
		D3D12_CULL_MODE CullMode = D3D12_CULL_MODE_NONE;	// 背面カリングしない
		// ポリゴン内塗りつぶし(D3D12_FILL_MODE_WIREFRAMEにするとワイヤーフレームに)
		D3D12_FILL_MODE FillMode = D3D12_FILL_MODE_SOLID;
		// 深度クリッピングを有効に
		bool isDepthClip = true;

		//レンダーターゲット数
		UINT NumRenderTargets = 1;	//描画対象1つ(マルチレンダーターゲットやるなら増やす)
		//レンダーターゲット数によって増やす可能性あり
		DXGI_FORMAT RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;	//0~255指定のRGBA
		//アンチエイリアシングのためのサンプル数
		DXGI_SAMPLE_DESC SampleDesc = DXGI_SAMPLE_DESC{
			1,	//1ピクセルにつき1回サンプリング
			0	//最低クオリティ
		};

		//ここはパイプラインマネージャー側で設定する
		D3D12_INPUT_LAYOUT_DESC InputLayout;
	} render;

	struct Depth {
		//-------------------- デプスステンシルステート --------------------//
		bool isDepth = true;	//深度テストするか
		D3D12_DEPTH_WRITE_MASK DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可
		D3D12_COMPARISON_FUNC DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければ合格

		DXGI_FORMAT DSVFormat = DXGI_FORMAT_D32_FLOAT;						//深度値フォーマット
	} depth;

	NRootSignature rootSig;
};

class NGPipeline
{
private:
	//パイプラインデスク
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc_{};
	//パイプラインステート
	ComPtr<ID3D12PipelineState> pso_ = nullptr;

public:
	//頂点レイアウトたち(必要な分だけ用意する)
	D3D12_INPUT_ELEMENT_DESC vertLayoutObj_[3];
	D3D12_INPUT_ELEMENT_DESC vertLayoutSprite_[2];
	D3D12_INPUT_ELEMENT_DESC vertLayoutPostEffect_[2];

public:
	//パイプライン生成、指定したIDで登録
	static void Create(PipelineDesc desc, std::string id);
	//指定したIDのパイプラインデスクを取得
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetDesc(std::string id);
	//指定したIDのパイプラインステートを取得
	static ID3D12PipelineState* GetState(std::string id);
	//指定したIDのパイプラインを取得
	static NGPipeline* GetGPipeline(std::string id);

	NGPipeline() {};

private:
	//パイプライン生成
	void Create();
	//パイプラインデスクの設定
	void SetDesc(PipelineDesc desc);

public:
#pragma region 頂点レイアウトまわり
	//3Dオブジェクト用頂点レイアウト設定
	void SetVertLayoutObj();
	//スプライト用頂点レイアウト設定
	void SetVertLayoutSprite();
	//ポストエフェクト用頂点レイアウト設定
	void SetVertLayoutPostEffect();

#pragma endregion
#pragma region パイプライン生成
	//3Dオブジェクト用パイプライン生成
	PipelineSet CreatePipeline3d();
	//スプライト用パイプライン生成
	PipelineSet CreatePipelineSprite();
	//ポストエフェクト用パイプライン生成
	PipelineSet CreatePipelinePostEffect();
	//ガウシアンブラー用パイプライン生成
	PipelineSet CreatePipelineGaussian();
	//ラジアルブラー用パイプライン生成
	PipelineSet CreatePipelineRadial();
	//タイル用パイプライン生成
	PipelineSet CreatePipelineTile();
#pragma endregion
};


class PipeLineManager
{
public:
	static PipeLineManager* GetInstance();
	//パイプライン全種つくる
	void CreateAll();
};

//ブレンドモードを扱いやすくする用
namespace BlendUtil
{
	enum class BlendMode
	{
		Alpha,	//αブレンド
		Add,	//加算
		Sub,	//減算
		Inv,	//反転
	};

	//指定したブレンドモードのブレンド情報を取得
	static PipelineDesc::Blend::BlendDesc GetBlendMode(BlendMode blendMode);
};