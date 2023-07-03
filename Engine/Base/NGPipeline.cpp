#include "NGPipeline.h"
#include "NShader.h"

void NGPipeline::SetVertLayout3d()
{
	// 頂点レイアウト
	//座標
	vertLayout3d_[0] = {
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
	vertLayout3d_[1] = {
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	//UV
	vertLayout3d_[2] = {
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

void NGPipeline::SetShader()
{
	// シェーダーの設定
	pipelineDesc_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = vsBlob_->GetBufferSize();
	pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();
}

void NGPipeline::SetRasterizer(bool isCull)
{
	// サンプルマスクの設定
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	if (isCull)
	{
		pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// 背面をカリング
	}
	else
	{
		pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// カリングしない
	}
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// ポリゴン内塗りつぶし(D3D12_FILL_MODE_WIREFRAMEにするとワイヤーフレームに)
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
}

void NGPipeline::SetBlend(const bool is3d)
{
	pipelineDesc_.BlendState.AlphaToCoverageEnable = false;			//網羅率考慮してブレンドするか
	pipelineDesc_.BlendState.IndependentBlendEnable = false;			//それぞれのレンダーターゲットに別々のブレンドするか

	//レンダーターゲットのブレンド設定
	blendDesc_.BlendEnable = true;					//ブレンドを有効にする
	blendDesc_.LogicOpEnable = false;				//論理演算するか
	blendDesc_.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//マスク値：RBGA全てのチャンネルを描画

	blendDesc_.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blendDesc_.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blendDesc_.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	//アルファブレンド
	blendDesc_.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc_.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blendDesc_.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-ソースのアルファ値

	//設定したブレンドを適用
	pipelineDesc_.BlendState.RenderTarget[0] = blendDesc_;
	if (is3d)
	{
		pipelineDesc_.BlendState.RenderTarget[1] = blendDesc_;
	}
}

void NGPipeline::SetInputLayout(const bool is3d)
{
	if (is3d)
	{
		pipelineDesc_.InputLayout.pInputElementDescs = vertLayout3d_;
		pipelineDesc_.InputLayout.NumElements = _countof(vertLayout3d_);
	}
	else
	{
		pipelineDesc_.InputLayout.pInputElementDescs = vertLayoutSprite_;
		pipelineDesc_.InputLayout.NumElements = _countof(vertLayoutSprite_);
	}
}

void NGPipeline::SetInputLayoutPostEffect()
{
	pipelineDesc_.InputLayout.pInputElementDescs = vertLayoutPostEffect_;
	pipelineDesc_.InputLayout.NumElements = _countof(vertLayoutPostEffect_);
}

void NGPipeline::SetTopology()
{
	//トライアングルストリップを切り離すかどうか
	pipelineDesc_.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;	//カットなし
	//トポロジー指定
	pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
}

void NGPipeline::SetDepth(const bool isDepth)
{
	if (isDepth)
	{
		pipelineDesc_.DepthStencilState.DepthEnable = true;						//深度テストするか
	}
	else
	{
		pipelineDesc_.DepthStencilState.DepthEnable = false;						//深度テストするか
	}
	pipelineDesc_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可
	pipelineDesc_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;			//小さければ合格
	pipelineDesc_.DSVFormat = DXGI_FORMAT_D32_FLOAT;								//深度値フォーマット
}

void NGPipeline::SetRenderTarget(const uint32_t RTNum)
{
	pipelineDesc_.NumRenderTargets = RTNum;								//描画対象は2つ

	for (uint32_t i = 0; i < RTNum; i++)
	{
		pipelineDesc_.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255指定のRGBA
	}
}

void NGPipeline::SetAntiAliasing()
{
	pipelineDesc_.SampleDesc.Count = 1;		//1ピクセルにつき1回サンプリング
	pipelineDesc_.SampleDesc.Quality = 0;	//最低クオリティ
}

void NGPipeline::SetRootSignature(const uint32_t texNum, const uint32_t constantNum)
{
	rootParams_.SetDescRange(texNum);
	rootParams_.SetRootParam(texNum, constantNum);

	pipelineSet_.rootSig_.SetRootSignature(errorBlob_, rootParams_.entity_, samplerDesc_);
	pipelineSet_.rootSig_.CreateRootSignature();

	pipelineDesc_.pRootSignature = pipelineSet_.rootSig_.entity_.Get();
}

void NGPipeline::CreatePS()
{
	HRESULT result;

	result = NDX12::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineSet_.pipelineState_));
	assert(SUCCEEDED(result));
}

void NGPipeline::SetTexSampler(const bool isTiling)
{
	if (isTiling)
	{
		samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返しする（タイリング）
		samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返しする（タイリング）
	}
	else
	{
		samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;					//横繰り返ししない（タイリングなし）
		samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;					//縦繰り返ししない（タイリングなし）
	}
	samplerDesc_.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し（タイリング）
	samplerDesc_.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc_.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//全てリニア補間
	samplerDesc_.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc_.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc_.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc_.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダからのみ使用可能
}

PipelineSet NGPipeline::CreatePipeline3d()
{
	//シェーダー
	LoadVertShader3d();
	LoadPixelShader3d();

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
	SetTexSampler(false);

	//ルートシグネチャ
	//テクスチャ1個、行列、マテリアル、色、光源
	SetRootSignature(1, 4);

	//パイプラインステート生成
	CreatePS();

	return pipelineSet_;
}

PipelineSet NGPipeline::CreatePipelineSprite()
{
	//シェーダー
	LoadVertShaderSprite();
	LoadPixelShaderSprite();

	//頂点レイアウト設定
	SetVertLayoutSprite();

	//パイプラインステート
	SetShader();
	SetRasterizer(false);
	SetBlend(false);
	SetInputLayout(false);
	SetTopology();
	SetRenderTarget(2);
	SetAntiAliasing();
	SetDepth(false);

	//テクスチャサンプラーの設定
	SetTexSampler(false);

	//ルートシグネチャ
	//テクスチャ1個、行列、マテリアル、色
	SetRootSignature(1, 3);

	//パイプラインステート生成
	CreatePS();

	return pipelineSet_;
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
	NShader::GetInstance()->CreateShader("3d","Obj",false);
#pragma endregion
#pragma region デフォルト2D
	NShader::GetInstance()->CreateShader("Sprite", "Sprite", false);
#pragma endregion
#pragma region ポストエフェクト
	NShader::GetInstance()->CreateShader("PostEffect", "CG4", false);
#pragma endregion
#pragma region ガウシアンブラー
	NShader::GetInstance()->CreateShader("Gaussian", "GaussianBlur", false);
#pragma endregion
#pragma region ラジアルブラー
	NShader::GetInstance()->CreateShader("Radial", "RadialBlur", false);
#pragma endregion
#pragma region パーティクル3D
	NShader::GetInstance()->CreateShader("Particle3d", "Particle3D", true);
#pragma endregion
}

void PipeLineManager::Init()
{
	pipelineSet3d_ = pipeline3d_.CreatePipeline3d();
	pipelineSetSprite_ = pipelineSprite_.CreatePipelineSprite();
	pipelineSetPostEffect_ = pipelinePostEffect_.CreatePipelinePostEffect();
	pipelineSetGaussian_ = pipelineGaussian_.CreatePipelineGaussian();
	pipelineSetRadial_ = pipelineRadial_.CreatePipelineRadial();
	pipelineSetTile_ = pipelineTile_.CreatePipelineTile();
}

const PipelineSet& PipeLineManager::GetPipelineSet(std::string name) const
{
	if (name == "3d")
	{
		return pipelineSet3d_;
	}
	else if (name == "Sprite")
	{
		return pipelineSetSprite_;
	}
	else if (name == "PostEffect")
	{
		return pipelineSetPostEffect_;
	}
	else if (name == "Gaussian")
	{
		return pipelineSetGaussian_;
	}
	else if (name == "Radial")
	{
		return pipelineSetRadial_;
	}
	else if (name == "Tile")
	{
		return pipelineSetTile_;
	}

	//もし該当する名前がなければ異常終了
	std::exit(EXIT_FAILURE);
	//一応返り値設定
	return pipelineSet3d_;
}
