#include "NDX12.h"
#include "NTitleScene.h"
#include "NSceneManager.h"
#include "NAudioManager.h"
#include "imgui.h"
#include "NInput.h"
#include "NQuaternion.h"
#include "NMathUtil.h"

NTitleScene* NTitleScene::GetInstance()
{
	static NTitleScene instance;
	return &instance;
}

NTitleScene::NTitleScene() :
	cbTrans(new NConstBuff<ConstBuffDataTransform>)
{
}

NTitleScene::~NTitleScene()
{
	delete cbTrans;
	for (size_t i = 0; i < vertexBuffers.size(); i++)
	{
		delete vertexBuffers[i];
		delete indexBuffers[i];
	}
}

void NTitleScene::Init()
{
#pragma region	オーディオ初期化
	NAudio::GetInstance()->Init();
	//NAudioManager::Play("WinSE",true,0.2f);
#pragma endregion
#pragma region	カメラ初期化
	camera.ProjectiveProjection();
	camera.CreateMatView();
	NCamera::nowCamera = &camera;
#pragma endregion
#pragma region 描画初期化処理
	//マテリアル(定数バッファ)

	//モデル情報
	for (size_t i = 0; i < maxModel; i++)
	{
		model.emplace_back();
	}
	model[0].Create("sphere");
	model[1].Create("Cube");

	//オブジェクト
	// レベルデータからの読み込み
	levelData = std::make_unique<LevelData>();
	levelData = LevelDataLoader::GetInstance()->Load("C:/Users/K021G1126/source/repos/GE3/directX_CG/","levelEditor.json");
	SetObject(levelData.get());

	for (int i = 0; i < maxObj; i++)
	{
		obj.emplace_back();
		obj[i] = std::make_unique<NObj3d>();
		obj[i]->Init();
	}
	obj[0]->SetModel(model[0]);
	obj[1]->SetModel(model[0]);
	obj[2]->SetModel(model[1]);

#pragma region オブジェクトの初期値設定
	obj[0]->position = { 0,2,0 };
	obj[1]->position = { 2,0,0 };
	obj[2]->position = { 0,0,0 };
	obj[2]->scale = { 10,0.1f,10 };

	//設定したのを適用
	for (int i = 0; i < maxObj; i++)
	{
		obj[i]->Update();
	}

	sphere.pos = obj[0]->position;
	sphere.radius = obj[0]->scale.x;
	plane.normal = { 0,1,0 };
	plane.distance = obj[2]->position.Length();
#pragma endregion

	//FBX読み込み
	/*if (!loader.Load(importSetting))
	{

	}*/

	//// メッシュの数だけ頂点バッファを用意する
	//vertexBuffers.reserve(meshes.size());
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto size = meshes[i].vertices.size();
	//	auto stride = sizeof(NVertexAssimp);
	//	auto vertices = meshes[i].vertices.data();
	//	auto pVB = new NVertexBuff(vertices, (unsigned int)size);
	//	/*if (!pVB->IsValid())
	//	{
	//		printf("頂点バッファの生成に失敗\n");
	//	}*/

	//	vertexBuffers.push_back(pVB);
	//}

	//// メッシュの数だけインデックスバッファを用意する
	//indexBuffers.reserve(meshes.size());
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto size = sizeof(uint32_t) * meshes[i].indices.size();
	//	auto indices = meshes[i].indices.data();
	//	auto pIB = new NIndexBuff(indices, (unsigned int)size);
	//	/*if (!pIB->IsValid())
	//	{
	//		printf("インデックスバッファの生成に失敗\n");
	//		return false;
	//	}*/

	//	indexBuffers.push_back(pIB);
	//}

	//// モデルのサイズが違うので、ちゃんと映るようにこの辺の値を変えておく
	//auto eyePos = XMVectorSet(0.0f, 120.0, 75.0, 0.0f);
	//auto targetPos = XMVectorSet(0.0f, 120.0, 0.0, 0.0f);
	//auto upward = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//auto fov = XMConvertToRadians(60);

	NVertexAssimp vertices[4] = {};
	vertices[0].pos = NVector3(-1.0f, 1.0f, 0.0f);
	vertices[0].color = NColor(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].pos = NVector3(1.0f, 1.0f, 0.0f);
	vertices[1].color = NColor(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].pos = NVector3(1.0f, -1.0f, 0.0f);
	vertices[2].color = NColor(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[3].pos = NVector3(-1.0f, -1.0f, 0.0f);
	vertices[3].color = NColor(1.0f, 0.0f, 1.0f, 1.0f);

	uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };

	Mesh mesh = {}; // これまでの四角形を入れるためのメッシュ構造体
	mesh.vertices = std::vector<NVertexAssimp>(std::begin(vertices), std::end(vertices));
	mesh.indices = std::vector<uint32_t>(std::begin(indices), std::end(indices));
	meshes.clear(); // ちょっと無駄感あるが、一旦四角形で試したいのでAssimpLoaderで読み込んだモデルのメッシュを一旦クリア
	meshes.shrink_to_fit(); // 中身をゼロにする
	meshes.push_back(mesh);

	// メッシュの数だけ頂点バッファを用意する
	vertexBuffers.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		auto size = sizeof(NVertexAssimp) * meshes[i].vertices.size();
		auto vertices = meshes[i].vertices.data();
		auto pVB = new NVertexBuff(vertices, (unsigned int)size);

		vertexBuffers.push_back(pVB);
	}

	// メッシュの数だけインデックスバッファを用意する
	indexBuffers.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		auto size = sizeof(uint32_t) * meshes[i].indices.size();
		auto indices = meshes[i].indices.data();
		auto pIB = new NIndexBuff(indices, (unsigned int)size);

		indexBuffers.push_back(pIB);
	}

	cbTrans->Init();

	// 定数バッファへデータ転送
	cbTrans->constMap = nullptr;
	cbTrans->constBuff->Map(0, nullptr, (void**)&cbTrans->constMap);

	NMatrix4 mat;
	cbTrans->constMap->viewproj = NCamera::nowCamera->GetMatView() * NCamera::nowCamera->GetMatProjection();
	cbTrans->constMap->world = mat.Identity();
	cbTrans->constMap->cameraPos = NCamera::nowCamera->GetPos();

	cbTrans->Unmap();

#pragma region オブジェクトの初期値設定

#pragma endregion
	//背景スプライト生成

	//前景スプライト生成
	foreSprite[0] = std::make_unique<NSprite>();
	foreSprite[0]->CreateSprite("hamu", { 0,0 });
	foreSprite[0]->SetPos(0, 0);
	foreSprite[0]->SetSize(100, 100);
	foreSprite[0]->SetColor(255, 255, 255, 255);

#pragma endregion
	// ライト生成
	directionalLight = std::make_unique<NDirectionalLight>();
	directionalLight->Initialize();
	/*pointLight = std::make_unique<NPointLight>();
	pointLight->Initialize();
	spotLight = std::make_unique<NSpotLight>();
	spotLight->Initialize();
	circleShadow = std::make_unique<NCircleShadow>();
	circleShadow->Initialize();*/
	// 3Dオブジェクトにライトをセット
	NObj3d::SetNDirectionalLight(directionalLight.get());
	/*NObj3d::SetNPointLight(pointLight.get());
	NObj3d::SetNSpotLight(spotLight.get());
	NObj3d::SetNCircleShadow(circleShadow.get());*/

	timer.SetMaxTimer(10);
}

void NTitleScene::Update()
{
	ImGui::ShowDemoWindow();

	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneManager::SetScene(GAMESCENE);
	}

	if (NInput::IsKeyDown(DIK_RETURN))
	{
		NAudioManager::Play("WinSE", false, 0.5f);
	}

	//ライトたちの更新
	directionalLight->Update();
	/*pointLight->Update();
	spotLight->Update();
	circleShadow->Update();*/

#pragma region 行列の計算
	//ビュー行列の再生成
	camera.CreateMatView();
	NCamera::nowCamera = &camera;

	if (isCol)
	{
		obj[0]->color.SetColor(255, 0, 0, 255);
	}
	else
	{
		obj[0]->color.SetColor(255, 255, 255, 255);
	}
	obj[2]->color.SetColor(255, 255, 255, 255);

	obj[0]->MoveKey();

	sphere.pos = obj[0]->position;
	plane.distance = obj[2]->position.Dot(plane.normal);

	for (auto& o : obj)
	{
		o->Update();
	}

	for (auto& lo : levelDataobj)
	{
		lo->Update();
	}

	isCol = NCollision::Sphere2PlaneCol(sphere, plane);

	foreSprite[0]->UpdateMatrix();
#pragma endregion
}

void NTitleScene::Draw()
{
#pragma region グラフィックスコマンド
	//背景スプライト

	//3Dオブジェクト
	for (size_t i = 0; i < obj.size(); i++)
	{
		obj[i]->CommonBeginDraw();
		obj[i]->Draw();
	}

	for (size_t i = 0; i < levelDataobj.size(); i++)
	{
		levelDataobj[i]->CommonBeginDraw();
		levelDataobj[i]->Draw();
	}

	////メッシュの数だけインデックス分の描画を行う処理を回す
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto vbView = vertexBuffers[i]->view; // そのメッシュに対応する頂点バッファ
	//	auto ibView = indexBuffers[i]->view; // そのメッシュに対応する頂点バッファ

	//	// パイプラインステートとルートシグネチャの設定コマンド
	//	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSet3d().pipelineState.Get());
	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSet3d().rootSig.entity.Get());

	//	//ルートパラメータ2番に3D変換行列の定数バッファを渡す
	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTrans->constBuff->GetGPUVirtualAddress());
	//	//マテリアルとかカラーも送んなきゃいけないけど一旦後回し

	//	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//	NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);

	//	NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced((UINT)meshes[i].indices.size(), 1, 0, 0, 0); // インデックスの数分描画する
	//}

	// メッシュの数だけインデックス分の描画を行う処理を回す
	for (size_t i = 0; i < meshes.size(); i++)
	{
		auto vbView = vertexBuffers[i]->view; // そのメッシュに対応する頂点バッファ
		auto ibView = indexBuffers[i]->view; // そのメッシュに対応する頂点バッファ

		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSet3d().rootSig.entity.Get());
		NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSet3d().pipelineState.Get());
		//ルートパラメータ2番に3D変換行列の定数バッファを渡す
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTrans->constBuff->GetGPUVirtualAddress());
		NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
		NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);

		NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced((UINT)meshes[i].indices.size(), 1, 0, 0, 0); // インデックスの数分描画する
	}

	//前景スプライト
	foreSprite[0]->Draw();

	// 4.描画コマンドここまで
#pragma endregion
}

void NTitleScene::Reset()
{
	directionalLight->Initialize();
	/*pointLight->Initialize();
	spotLight->Initialize();
	circleShadow->Initialize();*/
	// 3Dオブジェクトにライトをセット
	NObj3d::SetNDirectionalLight(directionalLight.get());
	/*NObj3d::SetNPointLight(pointLight.get());
	NObj3d::SetNSpotLight(spotLight.get());
	NObj3d::SetNCircleShadow(circleShadow.get());*/
}

void NTitleScene::Finalize()
{
}

void NTitleScene::SetObject(LevelData* levelData)
{
	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects)
	{
		/*NModel* model = nullptr;
		decltype(levelData->models)::iterator it = levelData->models.find(objectData.filename);
		if (it != levelData->models.end())
		{*/
		/*model = &it->second;*/
		//モデルを指定して3Dオブジェクトを生成
		//配列に登録してく
		levelDataobj.emplace_back();
		levelDataobj.back() = std::make_unique<NObj3d>();
		levelDataobj.back()->Init();
		levelDataobj.back()->SetModel(model[0]);

		levelDataobj.back()->position = objectData.trans;
		levelDataobj.back()->rotation = objectData.rot;
		levelDataobj.back()->scale = objectData.scale;
		//obj.back()->SetMatWorld(objectData.matWorld);
	/*}*/
	}
}
