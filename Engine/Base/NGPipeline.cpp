#include "NGPipeline.h"

std::map<std::string, NGPipeline> psoMap;	//パイプラインステートのマップ

using bDesc = PipelineDesc::Blend::BlendDesc;	//長すぎるから省略

void NGPipeline::Create(PipelineDesc desc, std::string id)
{
	psoMap[id] = NGPipeline();
	NGPipeline* ptr = &psoMap[id];
	ptr->SetDesc(desc);
	ptr->Create();
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC* NGPipeline::GetDesc(std::string id)
{
	return &psoMap[id].psDesc_;
}

ID3D12PipelineState* NGPipeline::GetState(std::string id)
{
	return psoMap[id].pso_.Get();
}

NGPipeline* NGPipeline::GetGPipeline(std::string id)
{
	return &psoMap[id];
}

void NGPipeline::Create()
{
	NDX12::GetInstance()->GetDevice()->
		CreateGraphicsPipelineState(&psDesc_, IID_PPV_ARGS(&pso_));
}

void NGPipeline::SetDesc(PipelineDesc desc)
{
	//-------------------------------- シェーダーの設定 --------------------------------//
	psDesc_.VS.pShaderBytecode = desc.shader.pShader->GetVSBlob()->GetBufferPointer();
	psDesc_.VS.BytecodeLength = desc.shader.pShader->GetVSBlob()->GetBufferSize();
	psDesc_.PS.pShaderBytecode = desc.shader.pShader->GetPSBlob()->GetBufferPointer();
	psDesc_.PS.BytecodeLength = desc.shader.pShader->GetPSBlob()->GetBufferSize();
	if (desc.shader.pShader->GetGSBlob() != nullptr)	//ジオメトリシェーダーがあるなら
	{
		psDesc_.GS.pShaderBytecode = desc.shader.pShader->GetGSBlob()->GetBufferPointer();
		psDesc_.GS.BytecodeLength = desc.shader.pShader->GetGSBlob()->GetBufferSize();
	}

	//-------------------------------- 図形の形状設定 --------------------------------//
	//トライアングルストリップを切り離すかどうか
	psDesc_.IBStripCutValue = desc.render.IBStripCutValue;
	//トポロジー指定
	psDesc_.PrimitiveTopologyType = desc.render.PrimitiveTopologyType;

	//-------------------------------- ラスタライザの設定 --------------------------------//
	// サンプルマスクの設定
	psDesc_.SampleMask = desc.render.SampleMask;
	// ラスタライザの設定
	psDesc_.RasterizerState.CullMode = desc.render.RasterizerState.CullMode;

	// ポリゴン内塗りつぶしするか
	psDesc_.RasterizerState.FillMode = desc.render.RasterizerState.FillMode;
	// 深度クリッピングを有効にするか
	psDesc_.RasterizerState.DepthClipEnable = desc.render.RasterizerState.DepthClipEnable;

	//-------------------------------- レンダーターゲット回りの設定 --------------------------------//
	psDesc_.NumRenderTargets = desc.render.NumRenderTargets;
	for (uint32_t i = 0; i < (uint32_t)psDesc_.NumRenderTargets; i++)
	{
		psDesc_.RTVFormats[i] = desc.render.RTVFormat;
	}

	//-------------------------------- サンプリング情報の設定 --------------------------------//
	psDesc_.SampleDesc = desc.render.SampleDesc;

	//-------------------------------- 頂点レイアウトの設定 --------------------------------//
	psDesc_.InputLayout = desc.render.InputLayout;

	//-------------------------------- ブレンドデスクの設定 --------------------------------//
	psDesc_.BlendState.AlphaToCoverageEnable = desc.blend.isAlphaToCoverage;		//網羅率考慮してブレンドするか
	psDesc_.BlendState.IndependentBlendEnable = desc.blend.isIndependentBlend;		//それぞれのレンダーターゲットに別々のブレンドするか

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = psDesc_.BlendState.RenderTarget[0];
	blendDesc.BlendEnable = desc.blend.isBlend;				//ブレンドを有効にするか
	blendDesc.LogicOpEnable = desc.blend.isLogicOp;			//論理演算するか
	blendDesc.RenderTargetWriteMask =
		desc.blend.RenderTargetWriteMask;											//マスク値

	//指定してたブレンド情報を設定
	blendDesc.BlendOpAlpha = desc.blend.blendDesc.BlendOpAlpha;
	blendDesc.SrcBlendAlpha = desc.blend.blendDesc.SrcBlendAlpha;
	blendDesc.DestBlendAlpha = desc.blend.blendDesc.DestBlendAlpha;

	blendDesc.BlendOp = desc.blend.blendDesc.BlendOp;
	blendDesc.SrcBlend = desc.blend.blendDesc.SrcBlend;
	blendDesc.DestBlend = desc.blend.blendDesc.DestBlend;

	//設定したブレンドを適用(レンダーターゲットの数だけ)
	//レンダーターゲットごとにブレンドモード変えれるようにしたいな～(願望)
	for (uint32_t i = 0; i < (uint32_t)psDesc_.NumRenderTargets; i++)
	{
		psDesc_.BlendState.RenderTarget[i] = blendDesc;
	}

	//-------------------------------- 深度情報の設定 --------------------------------//
	psDesc_.DepthStencilState.DepthEnable = desc.depth.DepthStencilState.DepthEnable;
	psDesc_.DepthStencilState.DepthWriteMask = desc.depth.DepthStencilState.DepthWriteMask;
	psDesc_.DepthStencilState.DepthFunc = desc.depth.DepthStencilState.DepthFunc;
	psDesc_.DSVFormat = desc.depth.DSVFormat;

	//-------------------------------- ルートシグネチャの設定　--------------------------------//
	psDesc_.pRootSignature = desc.rootSig.GetRootSignature();
}

void NGPipeline::SetVertLayoutObj()
{
	// 頂点レイアウト
	//座標
	vertLayoutObj_[0] = {
	"POSITION",										//セマンティック名
	0,												//同名のセマンティックがあるとき使うインデックス
	DXGI_FORMAT_R32G32B32_FLOAT,					//要素数とビット数を表す
	0,												//入力スロットインデックス
	D3D12_APPEND_ALIGNED_ELEMENT,					//データのオフセット地(左のは自動設定)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別
	0												//一度に描画するインスタンス数(0でよい)
	};// (1行で書いたほうが見やすい)
	//座標以外に色、テクスチャUVなどを渡す場合はさらに続ける
	//法線ベクトル
	vertLayoutObj_[1] = {
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	//UV
	vertLayoutObj_[2] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
}

void NGPipeline::SetVertLayoutSprite()
{
	// 頂点レイアウト
	//座標
	vertLayoutSprite_[0] = {
	"POSITION",										//セマンティック名
	0,												//同名のセマンティックがあるとき使うインデックス
	DXGI_FORMAT_R32G32B32_FLOAT,					//要素数とビット数を表す
	0,												//入力スロットインデックス
	D3D12_APPEND_ALIGNED_ELEMENT,					//データのオフセット地(左のは自動設定)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別
	0												//一度に描画するインスタンス数(0でよい)
	};// (1行で書いたほうが見やすい)
	//座標以外に色、テクスチャUVなどを渡す場合はさらに続ける
	//UV
	vertLayoutSprite_[1] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
}

void NGPipeline::SetVertLayoutPostEffect()
{
	// 頂点レイアウト
	//座標
	vertLayoutPostEffect_[0] = {
	"POSITION",										//セマンティック名
	0,												//同名のセマンティックがあるとき使うインデックス
	DXGI_FORMAT_R32G32B32_FLOAT,					//要素数とビット数を表す
	0,												//入力スロットインデックス
	D3D12_APPEND_ALIGNED_ELEMENT,					//データのオフセット地(左のは自動設定)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別
	0												//一度に描画するインスタンス数(0でよい)
	};// (1行で書いたほうが見やすい)
	//座標以外に色、テクスチャUVなどを渡す場合はさらに続ける
	//UV
	vertLayoutPostEffect_[1] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
}

void NGPipeline::SetVertLayoutParticle()
{
	// 頂点レイアウト
	//座標
	vertLayoutParticle_[0] = {
	"POSITION",										//セマンティック名
	0,												//同名のセマンティックがあるとき使うインデックス
	DXGI_FORMAT_R32G32B32_FLOAT,					//要素数とビット数を表す
	0,												//入力スロットインデックス
	D3D12_APPEND_ALIGNED_ELEMENT,					//データのオフセット地(左のは自動設定)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別
	0												//一度に描画するインスタンス数(0でよい)
	};// (1行で書いたほうが見やすい)
	//座標以外に色、テクスチャUVなどを渡す場合はさらに続ける
	//回転情報
	vertLayoutParticle_[1] = {
		"ROT",0,
		DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	//色
	vertLayoutParticle_[2] = {
		"COLOR",0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	//大きさ
	vertLayoutParticle_[3] = {
		"TEXCOORD", 0,
		DXGI_FORMAT_R32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
}

PipeLineManager* PipeLineManager::GetInstance()
{
	static PipeLineManager instance;
	return &instance;
}

void PipeLineManager::CreateAll()
{
	NGPipeline pipeLine;		//頂点レイアウト設定用
	pipeLine.SetVertLayoutObj();
	pipeLine.SetVertLayoutSprite();
	pipeLine.SetVertLayoutPostEffect();
	pipeLine.SetVertLayoutParticle();
#pragma region デフォルト3D
	//シェーダー生成
	NShader::GetInstance()->CreateShader("Obj", "Obj", false);

	PipelineDesc objDesc;
	//頂点レイアウト設定
	objDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	objDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//ルートシグネチャ設定
	NRootSignature rootSigObj;
	rootSigObj.SetSamplerDesc(false);
	//テクスチャ1個、行列、マテリアル、色、光源
	rootSigObj.SetRootParam(1,4);
	rootSigObj.Create();
	objDesc.rootSig = rootSigObj;

	//シェーダー設定
	objDesc.shader.pShader = NShader::GetInstance()->GetShader("Obj");

	//カリング設定
	objDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
	//レンダーターゲット数設定
	objDesc.render.NumRenderTargets = 2;

	//深度テストする
	objDesc.depth.DepthStencilState.DepthEnable = true;

	//パイプライン生成
	NGPipeline::Create(objDesc, "Obj");
#pragma endregion
#pragma region タイリング3D(背景オブジェクトとかに使う)
	//シェーダー生成
	NShader::GetInstance()->CreateShader("Tile", "Tile", false);

	PipelineDesc tileDesc;
	//頂点レイアウト設定
	tileDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	tileDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//ルートシグネチャ設定
	NRootSignature rootSigTile;
	rootSigTile.SetSamplerDesc(true);
	//テクスチャ1個、行列、マテリアル、色、光源
	rootSigTile.SetRootParam(1, 4);
	rootSigTile.Create();
	tileDesc.rootSig = rootSigTile;

	//シェーダー設定
	tileDesc.shader.pShader = NShader::GetInstance()->GetShader("Tile");

	//カリング設定
	tileDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
	//レンダーターゲット数設定
	tileDesc.render.NumRenderTargets = 2;

	//深度テストする
	tileDesc.depth.DepthStencilState.DepthEnable = true;
	
	//パイプライン生成
	NGPipeline::Create(tileDesc, "TileObj");
#pragma endregion
#pragma region デフォルト2D
	//シェーダー生成
	NShader::GetInstance()->CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteDesc;
	//頂点レイアウト設定
	spriteDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutSprite_;
	spriteDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutSprite_);

	//ルートシグネチャ設定
	NRootSignature rootSigSprite;
	rootSigSprite.SetSamplerDesc(true);
	//テクスチャ1個、行列、色
	rootSigSprite.SetRootParam(1, 2);
	rootSigSprite.Create();
	spriteDesc.rootSig = rootSigSprite;

	//シェーダー設定
	spriteDesc.shader.pShader = NShader::GetInstance()->GetShader("Sprite");

	//深度情報設定
	spriteDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	spriteDesc.depth.DepthStencilState.DepthEnable = false;

	//レンダーターゲット数設定
	spriteDesc.render.NumRenderTargets = 2;

	//カリング設定
	spriteDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//パイプライン生成
	NGPipeline::Create(spriteDesc, "Sprite");
#pragma endregion
#pragma region ポストエフェクト
	//シェーダー生成
	NShader::GetInstance()->CreateShader("PostEffect", "CG4", false);

	PipelineDesc postEffectDesc;
	//頂点レイアウト設定
	postEffectDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutPostEffect_;
	postEffectDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutPostEffect_);

	//ルートシグネチャ設定
	NRootSignature rootSigPostEffect;
	rootSigPostEffect.SetSamplerDesc(false);
	//テクスチャ2個、行列、色
	rootSigPostEffect.SetRootParam(2, 2);
	rootSigPostEffect.Create();
	postEffectDesc.rootSig = rootSigPostEffect;

	//シェーダー設定
	postEffectDesc.shader.pShader = NShader::GetInstance()->GetShader("Sprite");

	//深度情報設定
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//レンダーターゲット数設定
	postEffectDesc.render.NumRenderTargets = 2;

	//カリング設定
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//パイプライン生成
	NGPipeline::Create(postEffectDesc, "PostEffect");
#pragma endregion
#pragma region ガウシアンブラー
	//シェーダー生成
	NShader::GetInstance()->CreateShader("Gaussian", "GaussianBlur", false);

	//頂点レイアウト設定
	postEffectDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutPostEffect_;
	postEffectDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutPostEffect_);

	//ルートシグネチャ設定
	postEffectDesc.rootSig = rootSigPostEffect;

	//シェーダー設定
	postEffectDesc.shader.pShader = NShader::GetInstance()->GetShader("Sprite");

	//深度情報設定
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//カリング設定
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//パイプライン生成
	NGPipeline::Create(postEffectDesc, "Gaussian");
#pragma endregion
#pragma region ラジアルブラー
	//シェーダー生成
	NShader::GetInstance()->CreateShader("Radial", "RadialBlur", false);

	//頂点レイアウト設定
	postEffectDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutPostEffect_;
	postEffectDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutPostEffect_);

	//ルートシグネチャ設定
	postEffectDesc.rootSig = rootSigPostEffect;

	//シェーダー設定
	postEffectDesc.shader.pShader = NShader::GetInstance()->GetShader("Sprite");

	//深度情報設定
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//カリング設定
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//パイプライン生成
	NGPipeline::Create(postEffectDesc, "Radial");
#pragma endregion
#pragma region パーティクル3D
	//シェーダー生成
	NShader::GetInstance()->CreateShader("Particle3d", "Particle3D", true);

	PipelineDesc particleDesc;
	//頂点レイアウト設定
	particleDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutParticle_;
	particleDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutParticle_);

	particleDesc.render.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//particleDesc.blend.blendDesc = 
	//	bDesc::GetBlendMode(bDesc::BlendMode::Add);

	//ルートシグネチャ設定
	NRootSignature rootSigParticle;
	rootSigParticle.SetSamplerDesc(false);
	//テクスチャ2個、行列、光源
	rootSigParticle.SetRootParam(2, 2);
	rootSigParticle.Create();
	particleDesc.rootSig = rootSigParticle;

	//シェーダー設定
	particleDesc.shader.pShader = NShader::GetInstance()->GetShader("Particle3d");

	//深度テストする
	particleDesc.depth.DepthStencilState.DepthEnable = true;

	//カリング設定
	particleDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//パイプライン生成
	NGPipeline::Create(particleDesc, "Particle3d");
#pragma endregion
}

bDesc bDesc::GetBlendMode(BlendMode blendMode)
{
	bDesc desc;
	switch (blendMode)
	{
	case BlendMode::Alpha:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//加算
		desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				//ソースのアルファ値
		desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;			//1.0f-ソースのアルファ値
		break;

	case BlendMode::Add:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//加算
		desc.SrcBlend = D3D12_BLEND_ONE;					//ソースの値を100%使う
		desc.DestBlend = D3D12_BLEND_ONE;					//デストの値を100%使う
		break;

	case BlendMode::Sub:
		desc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;			//減算
		desc.SrcBlend = D3D12_BLEND_ONE;					//ソースの値を100%使う
		desc.DestBlend = D3D12_BLEND_ONE;					//デストの値を100%使う

		desc.BlendOpAlpha = D3D12_BLEND_OP_REV_SUBTRACT;	//減算
		desc.SrcBlendAlpha = D3D12_BLEND_ONE;				//ソースの値を100%使う
		desc.DestBlendAlpha = D3D12_BLEND_ONE;				//デストの値を100%使う
		break;

	case BlendMode::Inv:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//加算
		desc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;			//色反転(1-RGB)
		desc.DestBlend = D3D12_BLEND_ZERO;					//デストの値を0%使う
		break;

	default:
		break;
	}

	return desc;
}