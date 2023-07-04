#include "NGPipeline.h"

std::map<std::string, NGPipeline> psoMap;	//パイプラインステートのマップ

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
	pipelineDesc_.VS.pShaderBytecode = desc.shader.pShader->GetVSBlob()->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = desc.shader.pShader->GetVSBlob()->GetBufferSize();
	pipelineDesc_.PS.pShaderBytecode = desc.shader.pShader->GetPSBlob()->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = desc.shader.pShader->GetPSBlob()->GetBufferSize();
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
	psDesc_.RasterizerState.CullMode = desc.render.CullMode;

	// ポリゴン内塗りつぶしするか
	psDesc_.RasterizerState.FillMode = desc.render.FillMode;
	// 深度クリッピングを有効にするか
	psDesc_.RasterizerState.DepthClipEnable = desc.render.isDepthClip;

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
	psDesc_.BlendState.RenderTarget->BlendEnable = desc.blend.isBlend;				//ブレンドを有効にするか
	psDesc_.BlendState.RenderTarget->LogicOpEnable = desc.blend.isLogicOp;			//論理演算するか
	psDesc_.BlendState.RenderTarget->RenderTargetWriteMask =
		desc.blend.RenderTargetWriteMask;											//マスク値

	//指定してたブレンド情報を設定
	psDesc_.BlendState.RenderTarget->BlendOpAlpha = desc.blend.blendDesc.BlendOpAlpha;
	psDesc_.BlendState.RenderTarget->SrcBlendAlpha = desc.blend.blendDesc.SrcBlendAlpha;
	psDesc_.BlendState.RenderTarget->DestBlendAlpha = desc.blend.blendDesc.DestBlendAlpha;

	psDesc_.BlendState.RenderTarget->BlendOp = desc.blend.blendDesc.BlendOp;
	psDesc_.BlendState.RenderTarget->SrcBlend = desc.blend.blendDesc.SrcBlend;
	psDesc_.BlendState.RenderTarget->DestBlend = desc.blend.blendDesc.DestBlend;

	//設定したブレンドを適用(レンダーターゲットの数だけ)
	//レンダーターゲットごとにブレンドモード変えれるようにしたいな～(願望)
	for (uint32_t i = 0; i < (uint32_t)psDesc_.NumRenderTargets; i++)
	{
		psDesc_.BlendState.RenderTarget[i] = blendDesc_;
	}

	//-------------------------------- 深度情報の設定 --------------------------------//
	psDesc_.DepthStencilState.DepthEnable = desc.depth.isDepth;
	psDesc_.DepthStencilState.DepthWriteMask = desc.depth.DepthWriteMask;
	psDesc_.DepthStencilState.DepthFunc = desc.depth.DepthFunc;
	psDesc_.DSVFormat = desc.depth.DSVFormat;

	//-------------------------------- ルートシグネチャの設定　--------------------------------//
	psDesc_.pRootSignature = desc.rootSig.entity_.Get();
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
	"SV_POSITION",									//セマンティック名
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

PipelineSet NGPipeline::CreatePipelinePostEffect()
{
	//シェーダー
	LoadVertShaderPostEffect();
	LoadPixelShaderPostEffect();

	//頂点レイアウト設定
	SetVertLayoutPostEffect();

	//パイプラインステート
	SetShader();
	SetRasterizer(false);
	SetBlend(false);
	SetInputLayoutPostEffect();
	SetTopology();
	SetRenderTarget(1);
	SetAntiAliasing();
	SetDepth(false);

	//テクスチャサンプラーの設定
	SetTexSampler(false);

	//ルートシグネチャ
	//テクスチャ2個、行列、色
	SetRootSignature(2, 2);

	//パイプラインステート生成
	CreatePS();

	return pipelineSet_;
}

PipelineSet NGPipeline::CreatePipelineGaussian()
{
	//シェーダー
	LoadVertShaderGaussian();
	LoadPixelShaderGaussian();

	//頂点レイアウト設定
	SetVertLayoutPostEffect();

	//パイプラインステート
	SetShader();
	SetRasterizer(false);
	SetBlend(false);
	SetInputLayoutPostEffect();
	SetTopology();
	SetRenderTarget(1);
	SetAntiAliasing();
	SetDepth(false);

	//テクスチャサンプラーの設定
	SetTexSampler(false);

	//ルートシグネチャ
	//テクスチャ2個、行列、色
	SetRootSignature(2, 2);

	//パイプラインステート生成
	CreatePS();

	return pipelineSet_;
}

PipelineSet NGPipeline::CreatePipelineRadial()
{
	//シェーダー
	LoadVertShaderRadial();
	LoadPixelShaderRadial();

	//頂点レイアウト設定
	SetVertLayoutPostEffect();

	//パイプラインステート
	SetShader();
	SetRasterizer(false);
	SetBlend(false);
	SetInputLayoutPostEffect();
	SetTopology();
	SetRenderTarget(1);
	SetAntiAliasing();
	SetDepth(false);

	//テクスチャサンプラーの設定
	SetTexSampler(false);

	//ルートシグネチャ
	//テクスチャ2個、行列、色
	SetRootSignature(2, 2);

	//パイプラインステート生成
	CreatePS();

	return pipelineSet_;
}

PipelineSet NGPipeline::CreatePipelineTile()
{
	//シェーダー
	LoadVertShaderTile();
	LoadPixelShaderTile();

	//頂点レイアウト設定
	SetVertLayout3d();

	//パイプラインステート
	SetShader();
	SetRasterizer(true);
	SetBlend(true);
	SetInputLayout(true);
	SetTopology();
	SetRenderTarget(2);
	SetAntiAliasing();
	SetDepth(true);

	//テクスチャサンプラーの設定
	SetTexSampler(true);

	//ルートシグネチャ
	//テクスチャ1個、行列、マテリアル、色、光源
	SetRootSignature(1, 4);

	//パイプラインステート生成
	CreatePS();

	return pipelineSet_;
}

PipeLineManager::PipeLineManager()
{
}

PipeLineManager::~PipeLineManager()
{
}

PipeLineManager* PipeLineManager::GetInstance()
{
	static PipeLineManager instance;
	return &instance;
}

void PipeLineManager::CreateAll()
{
#pragma region デフォルト3D
	//シェーダー生成
	NShader::GetInstance()->CreateShader("obj", "Obj", false);

	PipelineDesc objDesc;
	NGPipeline objPipeLine;
	//頂点レイアウト設定
	objPipeLine.SetVertLayoutObj();
	objDesc.render.InputLayout.pInputElementDescs = objPipeLine.vertLayoutObj_;
	objDesc.render.InputLayout.NumElements = _countof(objPipeLine.vertLayoutObj_);

	//ルートシグネチャ設定
	NRootSignature rootSig;
	rootSig.SetSamplerDesc(false);
	//テクスチャ1個、行列、マテリアル、色、光源
	rootSig.GetRootParam().SetRootParam(1,4);
	rootSig.Create();
	objDesc.rootSig = rootSig;

	//シェーダー設定
	objDesc.shader.pShader = NShader::GetInstance()->GetShader("obj");

	//カリング設定
	objDesc.render.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
	//レンダーターゲット数設定
	objDesc.render.NumRenderTargets = 2;

	//深度テストする
	objDesc.depth.isDepth = true;

	//パイプライン生成
	NGPipeline::Create(objDesc, "obj");
#pragma endregion
#pragma region デフォルト2D
	//シェーダー生成
	NShader::GetInstance()->CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteDesc;
	NGPipeline spritePipeLine;
	//頂点レイアウト設定
	spriteDesc.render.InputLayout.pInputElementDescs = spritePipeLine.vertLayoutSprite_;
	spriteDesc.render.InputLayout.NumElements = _countof(spritePipeLine.vertLayoutSprite_);

	//ルートシグネチャ設定
	NRootSignature rootSig;
	rootSig.SetSamplerDesc(false);
	//テクスチャ1個、行列、マテリアル、色
	rootSig.GetRootParam().SetRootParam(1, 3);
	rootSig.Create();
	spriteDesc.rootSig = rootSig;

	//シェーダー設定
	spriteDesc.shader.pShader = NShader::GetInstance()->GetShader("sprite");

	//深度情報設定
	spriteDesc.depth.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	spriteDesc.depth.isDepth = false;

	//カリング設定
	spriteDesc.render.CullMode = D3D12_CULL_MODE_NONE;

	//パイプライン生成
	NGPipeline::Create(spriteDesc, "sprite");
#pragma endregion
#pragma region ポストエフェクト
	//シェーダー生成
	NShader::GetInstance()->CreateShader("PostEffect", "CG4", false);
#pragma endregion
#pragma region ガウシアンブラー
	//シェーダー生成
	NShader::GetInstance()->CreateShader("Gaussian", "GaussianBlur", false);
#pragma endregion
#pragma region ラジアルブラー
	//シェーダー生成
	NShader::GetInstance()->CreateShader("Radial", "RadialBlur", false);
#pragma endregion
#pragma region パーティクル3D
	//シェーダー生成
	NShader::GetInstance()->CreateShader("Particle3d", "Particle3D", true);
#pragma endregion
}

PipelineDesc::Blend::BlendDesc BlendUtil::GetBlendMode(BlendMode blendMode)
{
	PipelineDesc::Blend::BlendDesc desc;
	switch (blendMode)
	{
	case BlendUtil::BlendMode::Alpha:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//加算
		desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				//ソースのアルファ値
		desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;			//1.0f-ソースのアルファ値
		break;

	case BlendUtil::BlendMode::Add:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//加算
		desc.SrcBlend = D3D12_BLEND_ONE;					//ソースの値を100%使う
		desc.DestBlend = D3D12_BLEND_ONE;					//デストの値を100%使う
		break;

	case BlendUtil::BlendMode::Sub:
		desc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;			//減算
		desc.SrcBlend = D3D12_BLEND_ONE;					//ソースの値を100%使う
		desc.DestBlend = D3D12_BLEND_ONE;					//デストの値を100%使う

		desc.BlendOpAlpha = D3D12_BLEND_OP_REV_SUBTRACT;	//減算
		desc.SrcBlendAlpha = D3D12_BLEND_ONE;				//ソースの値を100%使う
		desc.DestBlendAlpha = D3D12_BLEND_ONE;				//デストの値を100%使う
		break;

	case BlendUtil::BlendMode::Inv:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//加算
		desc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;			//色反転(1-RGB)
		desc.DestBlend = D3D12_BLEND_ZERO;					//デストの値を0%使う
		break;

	default:
		break;
	}

	return desc;
}
