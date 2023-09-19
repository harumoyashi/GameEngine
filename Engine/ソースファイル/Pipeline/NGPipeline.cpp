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

void NGPipeline::SetVertLayoutFbx()
{
	// 頂点レイアウト
	//座標
	vertLayoutFbx_[0] = {
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
	vertLayoutFbx_[1] = {
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	//UV
	vertLayoutFbx_[2] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	//骨の識別番号
	vertLayoutFbx_[3] = {
		"BONEINDICES",0,
		DXGI_FORMAT_R32G32B32A32_UINT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	//骨が与える影響度
	vertLayoutFbx_[4] = {
		"BONEWEIGHTS",0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,0,
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
	pipeLine.SetVertLayoutFbx();
	pipeLine.SetVertLayoutSprite();
	pipeLine.SetVertLayoutPostEffect();
	pipeLine.SetVertLayoutParticle();
#pragma region デフォルト3D
	//シェーダー生成
	NShader::CreateShader("Obj", "Obj", false);

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
	objDesc.shader.pShader = NShader::GetShader("Obj");

	//カリング設定
	objDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
	//レンダーターゲット数設定
	objDesc.render.NumRenderTargets = 2;

	//深度テストする
	objDesc.depth.DepthStencilState.DepthEnable = true;

	//ブレンドモード設定
	objDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//パイプライン生成
	NGPipeline::Create(objDesc, "ObjNone");
#pragma endregion
#pragma region αブレンド3D
	//シェーダー生成
	NShader::CreateShader("Obj", "Obj", false);

	PipelineDesc objAlphaDesc;
	//頂点レイアウト設定
	objAlphaDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	objAlphaDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//ルートシグネチャ設定
	NRootSignature rootSigObjAlpha;
	rootSigObjAlpha.SetSamplerDesc(false);
	//テクスチャ1個、行列、マテリアル、色、光源
	rootSigObjAlpha.SetRootParam(1, 4);
	rootSigObjAlpha.Create();
	objAlphaDesc.rootSig = rootSigObjAlpha;

	//シェーダー設定
	objAlphaDesc.shader.pShader = NShader::GetShader("Obj");

	//カリング設定
	objAlphaDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
	//レンダーターゲット数設定
	objAlphaDesc.render.NumRenderTargets = 2;

	//深度テストする
	objAlphaDesc.depth.DepthStencilState.DepthEnable = true;

	//ブレンドモード設定
	objAlphaDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Alpha);

	//パイプライン生成
	NGPipeline::Create(objAlphaDesc, "ObjAlpha");
#pragma endregion
#pragma region 加算3D
	//シェーダー生成
	NShader::CreateShader("Obj", "Obj", false);

	PipelineDesc objAddDesc;
	//頂点レイアウト設定
	objAddDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	objAddDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//ルートシグネチャ設定
	NRootSignature rootSigObjAdd;
	rootSigObjAdd.SetSamplerDesc(false);
	//テクスチャ1個、行列、マテリアル、色、光源
	rootSigObjAdd.SetRootParam(1, 4);
	rootSigObjAdd.Create();
	objAddDesc.rootSig = rootSigObjAdd;

	//シェーダー設定
	objAddDesc.shader.pShader = NShader::GetShader("Obj");

	//カリング設定
	objAddDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
	//レンダーターゲット数設定
	objAddDesc.render.NumRenderTargets = 2;

	//深度テストする
	objAddDesc.depth.DepthStencilState.DepthEnable = true;

	//ブレンドモード設定
	objAddDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Add);

	//パイプライン生成
	NGPipeline::Create(objAddDesc, "ObjAdd");
#pragma endregion
#pragma region 減算3D
	//シェーダー生成
	NShader::CreateShader("Obj", "Obj", false);

	PipelineDesc objSubDesc;
	//頂点レイアウト設定
	objSubDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	objSubDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//ルートシグネチャ設定
	NRootSignature rootSigObjSub;
	rootSigObjSub.SetSamplerDesc(false);
	//テクスチャ1個、行列、マテリアル、色、光源
	rootSigObjSub.SetRootParam(1, 4);
	rootSigObjSub.Create();
	objSubDesc.rootSig = rootSigObjSub;

	//シェーダー設定
	objSubDesc.shader.pShader = NShader::GetShader("Obj");

	//カリング設定
	objSubDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
	//レンダーターゲット数設定
	objSubDesc.render.NumRenderTargets = 2;

	//深度テストする
	objSubDesc.depth.DepthStencilState.DepthEnable = true;

	//ブレンドモード設定
	objSubDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Sub);

	//パイプライン生成
	NGPipeline::Create(objSubDesc, "ObjSub");
#pragma endregion
#pragma region 色反転3D
	//シェーダー生成
	NShader::CreateShader("Obj", "Obj", false);

	PipelineDesc objInvDesc;
	//頂点レイアウト設定
	objInvDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	objInvDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//ルートシグネチャ設定
	NRootSignature rootSigObjInv;
	rootSigObjInv.SetSamplerDesc(false);
	//テクスチャ1個、行列、マテリアル、色、光源
	rootSigObjInv.SetRootParam(1, 4);
	rootSigObjInv.Create();
	objInvDesc.rootSig = rootSigObjInv;

	//シェーダー設定
	objInvDesc.shader.pShader = NShader::GetShader("Obj");

	//カリング設定
	objInvDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
	//レンダーターゲット数設定
	objInvDesc.render.NumRenderTargets = 2;

	//深度テストする
	objInvDesc.depth.DepthStencilState.DepthEnable = true;

	//ブレンドモード設定
	objInvDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Inv);

	//パイプライン生成
	NGPipeline::Create(objInvDesc, "ObjInv");
#pragma endregion
#pragma region デフォルトFBX
	//シェーダー生成
	NShader::CreateShader("Fbx", "Fbx", false);

	PipelineDesc fbxDesc;
	//頂点レイアウト設定
	fbxDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutFbx_;
	fbxDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutFbx_);

	//ルートシグネチャ設定
	NRootSignature rootSigFbx;
	rootSigFbx.SetSamplerDesc(false);
	//テクスチャ1個、行列、マテリアル、色、光源、スキン
	rootSigFbx.SetRootParam(1, 5);
	rootSigFbx.Create();
	fbxDesc.rootSig = rootSigFbx;

	//シェーダー設定
	fbxDesc.shader.pShader = NShader::GetShader("Fbx");

	//カリング設定
	fbxDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
	//レンダーターゲット数設定
	fbxDesc.render.NumRenderTargets = 2;

	//深度テストする
	fbxDesc.depth.DepthStencilState.DepthEnable = true;

	//ブレンドモード設定
	fbxDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//パイプライン生成
	NGPipeline::Create(fbxDesc, "FbxNone");
#pragma endregion
//#pragma region αブレンドFBX
//	//シェーダー生成
//	NShader::CreateShader("Fbx", "Fbx", false);
//
//	PipelineDesc fbxAlphaDesc;
//	//頂点レイアウト設定
//	fbxAlphaDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutFbx_;
//	fbxAlphaDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutFbx_);
//
//	//ルートシグネチャ設定
//	NRootSignature rootSigFbxAlpha;
//	rootSigFbxAlpha.SetSamplerDesc(false);
//	//テクスチャ1個、行列、マテリアル、色、光源、スキン
//	rootSigFbx.SetRootParam(1, 5);
//	rootSigFbxAlpha.Create();
//	fbxAlphaDesc.rootSig = rootSigFbxAlpha;
//
//	//シェーダー設定
//	fbxAlphaDesc.shader.pShader = NShader::GetShader("Fbx");
//
//	//カリング設定
//	fbxAlphaDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
//	//レンダーターゲット数設定
//	fbxAlphaDesc.render.NumRenderTargets = 2;
//
//	//深度テストする
//	fbxAlphaDesc.depth.DepthStencilState.DepthEnable = true;
//
//	//ブレンドモード設定
//	fbxAlphaDesc.blend.blendDesc =
//		bDesc::GetBlendMode(BlendMode::Alpha);
//
//	//パイプライン生成
//	NGPipeline::Create(fbxAlphaDesc, "FbxAlpha");
//#pragma endregion
//#pragma region 加算FBX
//	//シェーダー生成
//	NShader::CreateShader("Fbx", "Fbx", false);
//
//	PipelineDesc fbxAddDesc;
//	//頂点レイアウト設定
//	fbxAddDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutFbx_;
//	fbxAddDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutFbx_);
//
//	//ルートシグネチャ設定
//	NRootSignature rootSigFbxAdd;
//	rootSigFbxAdd.SetSamplerDesc(false);
//	//テクスチャ1個、行列、マテリアル、色、光源、スキン
//	rootSigFbx.SetRootParam(1, 5);
//	rootSigFbxAdd.Create();
//	fbxAddDesc.rootSig = rootSigFbxAdd;
//
//	//シェーダー設定
//	fbxAddDesc.shader.pShader = NShader::GetShader("Fbx");
//
//	//カリング設定
//	fbxAddDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
//	//レンダーターゲット数設定
//	fbxAddDesc.render.NumRenderTargets = 2;
//
//	//深度テストする
//	fbxAddDesc.depth.DepthStencilState.DepthEnable = true;
//
//	//ブレンドモード設定
//	fbxAddDesc.blend.blendDesc =
//		bDesc::GetBlendMode(BlendMode::Add);
//
//	//パイプライン生成
//	NGPipeline::Create(fbxAddDesc, "FbxAdd");
//#pragma endregion
//#pragma region 減算FBX
//	//シェーダー生成
//	NShader::CreateShader("Fbx", "Fbx", false);
//
//	PipelineDesc fbxSubDesc;
//	//頂点レイアウト設定
//	fbxSubDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutFbx_;
//	fbxSubDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutFbx_);
//
//	//ルートシグネチャ設定
//	NRootSignature rootSigFbxSub;
//	rootSigFbxSub.SetSamplerDesc(false);
//	//テクスチャ1個、行列、マテリアル、色、光源、スキン
//	rootSigFbx.SetRootParam(1, 5);
//	rootSigFbxSub.Create();
//	fbxSubDesc.rootSig = rootSigFbxSub;
//
//	//シェーダー設定
//	fbxSubDesc.shader.pShader = NShader::GetShader("Fbx");
//
//	//カリング設定
//	fbxSubDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
//	//レンダーターゲット数設定
//	fbxSubDesc.render.NumRenderTargets = 2;
//
//	//深度テストする
//	fbxSubDesc.depth.DepthStencilState.DepthEnable = true;
//
//	//ブレンドモード設定
//	fbxSubDesc.blend.blendDesc =
//		bDesc::GetBlendMode(BlendMode::Sub);
//
//	//パイプライン生成
//	NGPipeline::Create(fbxSubDesc, "FbxSub");
//#pragma endregion
//#pragma region 色反転FBX
//	//シェーダー生成
//	NShader::CreateShader("Fbx", "Fbx", false);
//
//	PipelineDesc fbxInvDesc;
//	//頂点レイアウト設定
//	fbxInvDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutFbx_;
//	fbxInvDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutFbx_);
//
//	//ルートシグネチャ設定
//	NRootSignature rootSigFbxInv;
//	rootSigFbxInv.SetSamplerDesc(false);
//	//テクスチャ1個、行列、マテリアル、色、光源、スキン
//	rootSigFbx.SetRootParam(1, 5);
//	rootSigFbxInv.Create();
//	fbxInvDesc.rootSig = rootSigFbxInv;
//
//	//シェーダー設定
//	fbxInvDesc.shader.pShader = NShader::GetShader("Fbx");
//
//	//カリング設定
//	fbxInvDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
//	//レンダーターゲット数設定
//	fbxInvDesc.render.NumRenderTargets = 2;
//
//	//深度テストする
//	fbxInvDesc.depth.DepthStencilState.DepthEnable = true;
//
//	//ブレンドモード設定
//	fbxInvDesc.blend.blendDesc =
//		bDesc::GetBlendMode(BlendMode::Inv);
//
//	//パイプライン生成
//	NGPipeline::Create(fbxInvDesc, "FbxInv");
//#pragma endregion
#pragma region タイリング3D(背景オブジェクトとかに使う)
	//シェーダー生成
	NShader::CreateShader("Tile", "Tile", false);

	PipelineDesc tileDesc;
	//頂点レイアウト設定
	tileDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutObj_;
	tileDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutObj_);

	//ルートシグネチャ設定
	NRootSignature rootSigTile;
	rootSigTile.SetSamplerDesc(true);
	//テクスチャ1個、行列、マテリアル、色、光源、タイル用の情報
	rootSigTile.SetRootParam(1, 5);
	rootSigTile.Create();
	tileDesc.rootSig = rootSigTile;

	//シェーダー設定
	tileDesc.shader.pShader = NShader::GetShader("Tile");

	//カリング設定
	tileDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;		//背面カリングする
	//レンダーターゲット数設定
	tileDesc.render.NumRenderTargets = 2;

	//深度テストする
	tileDesc.depth.DepthStencilState.DepthEnable = true;

	//ブレンドモード設定
	tileDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//パイプライン生成
	NGPipeline::Create(tileDesc, "TileObjNone");
#pragma endregion
#pragma region デフォルト2D
	//シェーダー生成
	NShader::CreateShader("Sprite", "Sprite", false);

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
	spriteDesc.shader.pShader = NShader::GetShader("Sprite");

	//深度情報設定
	spriteDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	spriteDesc.depth.DepthStencilState.DepthEnable = false;

	//レンダーターゲット数設定
	spriteDesc.render.NumRenderTargets = 2;

	//カリング設定
	spriteDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドモード設定
	spriteDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//パイプライン生成
	NGPipeline::Create(spriteDesc, "SpriteNone");
#pragma endregion
#pragma region αブレンド2D
	//シェーダー生成
	NShader::CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteAlphaDesc;
	//頂点レイアウト設定
	spriteAlphaDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutSprite_;
	spriteAlphaDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutSprite_);

	//ルートシグネチャ設定
	NRootSignature rootSigSpriteAlpha;
	rootSigSpriteAlpha.SetSamplerDesc(true);
	//テクスチャ1個、行列、色
	rootSigSpriteAlpha.SetRootParam(1, 2);
	rootSigSpriteAlpha.Create();
	spriteAlphaDesc.rootSig = rootSigSpriteAlpha;

	//シェーダー設定
	spriteAlphaDesc.shader.pShader = NShader::GetShader("Sprite");

	//深度情報設定
	spriteAlphaDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	spriteAlphaDesc.depth.DepthStencilState.DepthEnable = false;

	//レンダーターゲット数設定
	spriteAlphaDesc.render.NumRenderTargets = 2;

	//カリング設定
	spriteAlphaDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドモード設定
	spriteAlphaDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Alpha);

	//パイプライン生成
	NGPipeline::Create(spriteAlphaDesc, "SpriteAlpha");
#pragma endregion
#pragma region 加算2D
	//シェーダー生成
	NShader::CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteAddDesc;
	//頂点レイアウト設定
	spriteAddDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutSprite_;
	spriteAddDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutSprite_);

	//ルートシグネチャ設定
	NRootSignature rootSigSpriteAdd;
	rootSigSpriteAdd.SetSamplerDesc(true);
	//テクスチャ1個、行列、色
	rootSigSpriteAdd.SetRootParam(1, 2);
	rootSigSpriteAdd.Create();
	spriteAddDesc.rootSig = rootSigSpriteAdd;

	//シェーダー設定
	spriteAddDesc.shader.pShader = NShader::GetShader("Sprite");

	//深度情報設定
	spriteAddDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	spriteAddDesc.depth.DepthStencilState.DepthEnable = false;

	//レンダーターゲット数設定
	spriteAddDesc.render.NumRenderTargets = 2;

	//カリング設定
	spriteAddDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドモード設定
	spriteAddDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Add);

	//パイプライン生成
	NGPipeline::Create(spriteAddDesc, "SpriteAdd");
#pragma endregion
#pragma region 減算2D
	//シェーダー生成
	NShader::CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteSubDesc;
	//頂点レイアウト設定
	spriteSubDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutSprite_;
	spriteSubDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutSprite_);

	//ルートシグネチャ設定
	NRootSignature rootSigSpriteSub;
	rootSigSpriteSub.SetSamplerDesc(true);
	//テクスチャ1個、行列、色
	rootSigSpriteSub.SetRootParam(1, 2);
	rootSigSpriteSub.Create();
	spriteSubDesc.rootSig = rootSigSpriteSub;

	//シェーダー設定
	spriteSubDesc.shader.pShader = NShader::GetShader("Sprite");

	//深度情報設定
	spriteSubDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	spriteSubDesc.depth.DepthStencilState.DepthEnable = false;

	//レンダーターゲット数設定
	spriteSubDesc.render.NumRenderTargets = 2;

	//カリング設定
	spriteSubDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドモード設定
	spriteSubDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Sub);

	//パイプライン生成
	NGPipeline::Create(spriteSubDesc, "SpriteSub");
#pragma endregion
#pragma region 色反転2D
	//シェーダー生成
	NShader::CreateShader("Sprite", "Sprite", false);

	PipelineDesc spriteInvDesc;
	//頂点レイアウト設定
	spriteInvDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutSprite_;
	spriteInvDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutSprite_);

	//ルートシグネチャ設定
	NRootSignature rootSigSpriteInv;
	rootSigSpriteInv.SetSamplerDesc(true);
	//テクスチャ1個、行列、色
	rootSigSpriteInv.SetRootParam(1, 2);
	rootSigSpriteInv.Create();
	spriteInvDesc.rootSig = rootSigSpriteInv;

	//シェーダー設定
	spriteInvDesc.shader.pShader = NShader::GetShader("Sprite");

	//深度情報設定
	spriteInvDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	spriteInvDesc.depth.DepthStencilState.DepthEnable = false;

	//レンダーターゲット数設定
	spriteInvDesc.render.NumRenderTargets = 2;

	//カリング設定
	spriteInvDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドモード設定
	spriteInvDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Inv);

	//パイプライン生成
	NGPipeline::Create(spriteInvDesc, "SpriteInv");
#pragma endregion
#pragma region ポストエフェクト
	//シェーダー生成
	NShader::CreateShader("Bloom", "Bloom", false);

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
	postEffectDesc.shader.pShader = NShader::GetShader("Bloom");

	//深度情報設定
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//レンダーターゲット数設定
	postEffectDesc.render.NumRenderTargets = 2;

	//カリング設定
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドモード設定
	postEffectDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//パイプライン生成
	NGPipeline::Create(postEffectDesc, "Bloom");
#pragma endregion
#pragma region ガウシアンブラー
	//シェーダー生成
	NShader::CreateShader("Gaussian", "GaussianBlur", false);

	//頂点レイアウト設定
	postEffectDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutPostEffect_;
	postEffectDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutPostEffect_);

	//ルートシグネチャ設定
	postEffectDesc.rootSig = rootSigPostEffect;

	//シェーダー設定
	postEffectDesc.shader.pShader = NShader::GetShader("Gaussian");

	//深度情報設定
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//カリング設定
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドモード設定
	postEffectDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//パイプライン生成
	NGPipeline::Create(postEffectDesc, "Gaussian");
#pragma endregion
#pragma region ラジアルブラー
	//シェーダー生成
	NShader::CreateShader("Radial", "RadialBlur", false);

	//頂点レイアウト設定
	postEffectDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutPostEffect_;
	postEffectDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutPostEffect_);

	//ルートシグネチャ設定
	postEffectDesc.rootSig = rootSigPostEffect;

	//シェーダー設定
	postEffectDesc.shader.pShader = NShader::GetShader("Radial");

	//深度情報設定
	postEffectDesc.depth.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//深度テストしない
	postEffectDesc.depth.DepthStencilState.DepthEnable = false;

	//カリング設定
	postEffectDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドモード設定
	postEffectDesc.blend.blendDesc = 
		bDesc::GetBlendMode(BlendMode::None);

	//パイプライン生成
	NGPipeline::Create(postEffectDesc, "Radial");
#pragma endregion
#pragma region デフォルトパーティクル3D
	//シェーダー生成
	NShader::CreateShader("Particle3d", "Particle3D", true);

	PipelineDesc particleDesc;
	//頂点レイアウト設定
	particleDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutParticle_;
	particleDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutParticle_);

	particleDesc.render.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//ルートシグネチャ設定
	NRootSignature rootSigParticle;
	rootSigParticle.SetSamplerDesc(false);
	//テクスチャ2個、行列、光源
	rootSigParticle.SetRootParam(2, 2);
	rootSigParticle.Create();
	particleDesc.rootSig = rootSigParticle;

	//シェーダー設定
	particleDesc.shader.pShader = NShader::GetShader("Particle3d");

	//深度テストする
	particleDesc.depth.DepthStencilState.DepthEnable = true;

	//カリング設定
	particleDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドモード設定
	particleDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::None);

	//パイプライン生成
	NGPipeline::Create(particleDesc, "Particle3dNone");
#pragma endregion
#pragma region αブレンドパーティクル3D
	//シェーダー生成
	NShader::CreateShader("Particle3d", "Particle3D", true);

	PipelineDesc particleAlphaDesc;
	//頂点レイアウト設定
	particleAlphaDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutParticle_;
	particleAlphaDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutParticle_);

	particleAlphaDesc.render.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//ルートシグネチャ設定
	NRootSignature rootSigParticleAlpha;
	rootSigParticleAlpha.SetSamplerDesc(false);
	//テクスチャ2個、行列、光源
	rootSigParticleAlpha.SetRootParam(2, 2);
	rootSigParticleAlpha.Create();
	particleAlphaDesc.rootSig = rootSigParticleAlpha;

	//シェーダー設定
	particleAlphaDesc.shader.pShader = NShader::GetShader("Particle3d");

	//深度テストする
	particleAlphaDesc.depth.DepthStencilState.DepthEnable = true;

	//カリング設定
	particleAlphaDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドモード設定
	particleAlphaDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Alpha);

	//パイプライン生成
	NGPipeline::Create(particleAlphaDesc, "Particle3dAlpha");
#pragma endregion
#pragma region 加算パーティクル3D
	//シェーダー生成
	NShader::CreateShader("Particle3d", "Particle3D", true);

	PipelineDesc particleAddDesc;
	//頂点レイアウト設定
	particleAddDesc.render.InputLayout.pInputElementDescs = pipeLine.vertLayoutParticle_;
	particleAddDesc.render.InputLayout.NumElements = _countof(pipeLine.vertLayoutParticle_);

	particleAddDesc.render.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//ルートシグネチャ設定
	NRootSignature rootSigParticleAdd;
	rootSigParticleAdd.SetSamplerDesc(false);
	//テクスチャ2個、行列、光源
	rootSigParticleAdd.SetRootParam(2, 2);
	rootSigParticleAdd.Create();
	particleAddDesc.rootSig = rootSigParticleAdd;

	//シェーダー設定
	particleAddDesc.shader.pShader = NShader::GetShader("Particle3d");

	//深度テストする
	particleAddDesc.depth.DepthStencilState.DepthEnable = true;

	//カリング設定
	particleAddDesc.render.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドモード設定
	particleAddDesc.blend.blendDesc =
		bDesc::GetBlendMode(BlendMode::Add);

	//パイプライン生成
	NGPipeline::Create(particleAddDesc, "Particle3dAdd");
#pragma endregion
}

bDesc bDesc::GetBlendMode(BlendMode blendMode)
{
	bDesc desc;
	switch (blendMode)
	{
	case BlendMode::None:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//下2つの値を足す
		desc.SrcBlend = D3D12_BLEND_ONE;					//ソースの値全部使う
		desc.DestBlend = D3D12_BLEND_ZERO;					//デストの値使わない
		break;

	case BlendMode::Alpha:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//下2つの値を足す
		desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				//ソースのアルファ値
		desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;			//1.0f-ソースのアルファ値
		break;

	case BlendMode::Add:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//下2つの値を足す
		desc.SrcBlend = D3D12_BLEND_ONE;					//ソースの値を100%使う
		desc.DestBlend = D3D12_BLEND_ONE;					//デストの値を100%使う
		break;

	case BlendMode::Sub:
		desc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;			//下2つの値を引く
		desc.SrcBlend = D3D12_BLEND_ONE;					//ソースの値を100%使う
		desc.DestBlend = D3D12_BLEND_ONE;					//デストの値を100%使う

		desc.BlendOpAlpha = D3D12_BLEND_OP_REV_SUBTRACT;	//下2つの値を引く
		desc.SrcBlendAlpha = D3D12_BLEND_ONE;				//ソースの値を100%使う
		desc.DestBlendAlpha = D3D12_BLEND_ONE;				//デストの値を100%使う
		break;

	case BlendMode::Inv:
		desc.BlendOp = D3D12_BLEND_OP_ADD;					//下2つの値を足す
		desc.SrcBlend = D3D12_BLEND_INV_SRC_COLOR;			//色反転(1-RGB)
		desc.DestBlend = D3D12_BLEND_ZERO;					//デストの値を0%使う
		break;

	default:
		break;
	}

	return desc;
}