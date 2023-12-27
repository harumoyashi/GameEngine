#pragma once
#include <DirectXMath.h>
#include "NTexture.h"
#include "NRootSignature.h"
#include "NRootParam.h"
#include "NShader.h"
#include "NUtil.h"
#include <d3dx12.h>

#include <wrl.h>

// グラフィックスパイプライン //

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

			//指定したブレンドモードのブレンド情報を取得
			static PipelineDesc::Blend::BlendDesc GetBlendMode(BlendMode blendMode);
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
		D3D12_RASTERIZER_DESC RasterizerState = D3D12_RASTERIZER_DESC{
			D3D12_FILL_MODE_SOLID,	// ポリゴン内塗りつぶし(D3D12_FILL_MODE_WIREFRAMEにするとワイヤーフレームに)
			D3D12_CULL_MODE_NONE,	// 背面カリングしない
			0,
			0,
			0.f,
			0.f,
			true,					// 深度クリッピングを有効に
			0,
			0,
			0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		};

		//レンダーターゲット数
		UINT NumRenderTargets = 1;	//描画対象1つ(マルチレンダーターゲットやるなら増やす)
		//レンダーターゲット数によって増やす可能性あり
		DXGI_FORMAT RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255指定のRGBA
		//アンチエイリアシングのためのサンプル数
		DXGI_SAMPLE_DESC SampleDesc = DXGI_SAMPLE_DESC{
			1,	//1ピクセルにつき1回サンプリング
			0	//最低クオリティ
		};

		//ここはパイプラインマネージャー側で設定する
		D3D12_INPUT_LAYOUT_DESC InputLayout{};
	} render;

	struct Depth {
		//-------------------- デプスステンシルステート --------------------//
		D3D12_DEPTH_STENCIL_DESC DepthStencilState = D3D12_DEPTH_STENCIL_DESC{
			true,							//深度テストするか
			D3D12_DEPTH_WRITE_MASK_ALL,		//書き込み許可
			D3D12_COMPARISON_FUNC_LESS,		//小さければ合格
			false,
			0,
			0,
			D3D12_DEPTH_STENCILOP_DESC{},
			D3D12_DEPTH_STENCILOP_DESC{}
		};

		DXGI_FORMAT DSVFormat = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	} depth;

	NRootSignature rootSig;
};

class NGPipeline final
{
public:
	//パイプラインデスク
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc_{};
	//パイプラインステート
	ComPtr<ID3D12PipelineState> pso_ = nullptr;

public:
	//頂点レイアウトたち(必要な分だけ用意する)
	D3D12_INPUT_ELEMENT_DESC vertLayoutObj_[3];
	D3D12_INPUT_ELEMENT_DESC vertLayoutFbx_[5];
	D3D12_INPUT_ELEMENT_DESC vertLayoutSprite_[2];
	D3D12_INPUT_ELEMENT_DESC vertLayoutPostEffect_[2];
	D3D12_INPUT_ELEMENT_DESC vertLayoutParticle_[4];
	D3D12_INPUT_ELEMENT_DESC vertLayoutGPUParticle_[5];

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
	//obj形式用頂点レイアウト設定
	void SetVertLayoutObj();
	//FBX形式用頂点レイアウト設定
	void SetVertLayoutFbx();
	//スプライト用頂点レイアウト設定
	void SetVertLayoutSprite();
	//ポストエフェクト用頂点レイアウト設定
	void SetVertLayoutPostEffect();
	//3Dパーティクル用頂点レイアウト設定
	void SetVertLayoutParticle();
	//GPUパーティクル用頂点レイアウト設定
	void SetVertLayoutGPUParticle();
#pragma endregion
};


class PipeLineManager final
{
public:
	static PipeLineManager* GetInstance();
	//パイプライン全種つくる
	void CreateAll();

private:
	PipeLineManager() = default;
	PipeLineManager(const PipeLineManager&) = delete;
	~PipeLineManager() = default;
	PipeLineManager& operator=(const PipeLineManager&) = delete;
};