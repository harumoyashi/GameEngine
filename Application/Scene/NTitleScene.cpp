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

NTitleScene::NTitleScene()
{
}

NTitleScene::~NTitleScene()
{
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
	NCamera::sCurrentCamera = &camera;
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
	levelData = LevelDataLoader::GetInstance()->Load("C:/Users/K021G1126/source/repos/GE3/directX_CG/", "levelEditor.json");
	//SetObject(levelData.get());
	LevelDataLoader::GetInstance()->SetObject(levelData.get(), levelDataobj);
	for (auto& lo : levelDataobj)
	{
		lo->SetModel(model[0]);
	}


	for (uint32_t i = 0; i < maxObj; i++)
	{
		obj.emplace_back();
		obj[i] = std::make_unique<NObj3d>();
		obj[i]->Init();
	}
	obj[0]->SetModel(model[0]);
	obj[1]->SetModel(model[0]);
	obj[2]->SetModel(model[1]);

#pragma region オブジェクトの初期値設定
	obj[0]->position_ = { 0,2,0 };
	obj[1]->position_ = { 2,0,0 };
	obj[2]->position_ = { 0,0,0 };
	obj[2]->scale_ = { 10,0.1f,10 };

	//設定したのを適用
	for (uint32_t i = 0; i < maxObj; i++)
	{
		obj[i]->Update();
	}

	sphere.pos = obj[0]->position_;
	sphere.radius = obj[0]->scale_.x;
	plane.normal = { 0,1,0 };
	plane.distance = obj[2]->position_.Length();
#pragma endregion

	//FBX読み込み
	if (!loader.Load(importSetting))
	{

	}

	// メッシュの数だけ頂点バッファを用意する
	vertexBuffers_.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		/*auto size = meshes[i].vertices.size();
		auto stride = sizeof(NVertexAssimp);
		auto vertices_ = meshes[i].vertices.data();
		auto pVB = new NVertexBuff(vertices_, (unsigned int)size);*/
		NVertexBuff pVB = NVertexBuff(meshes[i].vertices);
		/*if (!pVB->IsValid())
		{
			printf("頂点バッファの生成に失敗\n");
		}*/

		vertexBuffers_.emplace_back(pVB);
	}

	// メッシュの数だけインデックスバッファを用意する
	indexBuffers_.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		/*auto size = sizeof(uint32_t) * meshes[i].indices.size();
		auto indices_ = meshes[i].indices.data();*/
		NIndexBuff pIB = NIndexBuff(meshes[i].indices);
		/*if (!pIB->IsValid())
		{
			printf("インデックスバッファの生成に失敗\n");
			return false;
		}*/

		indexBuffers_.emplace_back(pIB);
	}

	// モデルのサイズが違うので、ちゃんと映るようにこの辺の値を変えておく
	/*auto eyePos = XMVectorSet(0.0f, 120.0, 75.0, 0.0f);
	auto targetPos = XMVectorSet(0.0f, 120.0, 0.0, 0.0f);
	auto upward = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	auto fov = XMConvertToRadians(60);*/

	////assimpの四角形//
	//std::vector<NVertexPNU> vertices{4};
	//vertices[0].pos = NVector3(-1.0f, 1.0f, 0.0f);
	//vertices[0].uv = NVector2(0.0f, 1.0f);

	//vertices[1].pos = NVector3(1.0f, 1.0f, 0.0f);
	//vertices[1].uv = NVector2(1.0f, 1.0f);

	//vertices[2].pos = NVector3(1.0f, -1.0f, 0.0f);
	//vertices[2].uv = NVector2(1.0f,  0.0f);

	//vertices[3].pos = NVector3(-1.0f, -1.0f, 0.0f);
	//vertices[3].uv = NVector2(0.0f, 0.0f);

	//uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };

	//Mesh mesh = {}; // これまでの四角形を入れるためのメッシュ構造体
	//mesh.vertices = vertices;
	//mesh.indices = std::vector<uint32_t>(std::begin(indices), std::end(indices));
	////meshes.clear(); // ちょっと無駄感あるが、一旦四角形で試したいのでAssimpLoaderで読み込んだモデルのメッシュを一旦クリア
	////meshes.shrink_to_fit(); // 中身をゼロにする
	//meshes.push_back(mesh);

	//// メッシュの数だけ頂点バッファを用意する
	//vertexBuffers_.reserve(meshes.size());
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	//auto size = sizeof(NVertexPNU) * meshes[i].vertices.size();
	//	//auto vertices = meshes[i].vertices.data();
	//	//NVertexBuff pVB = NVertexBuff(meshes[i].vertices.data(), (unsigned int)size);
	//	NVertexBuff pVB = NVertexBuff(mesh.vertices);
	//	vertexBuffers_.emplace_back(pVB);
	//}

	//// メッシュの数だけインデックスバッファを用意する
	//indexBuffers_.reserve(meshes.size());
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto size = sizeof(uint32_t) * meshes[i].indices.size();
	//	//auto indices = meshes[i].indices.data();
	//	auto pIB = NIndexBuff(meshes[i].indices.data(), (unsigned int)size);

	//	indexBuffers_.emplace_back(pIB);
	//}

	//3D行列仮設定
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform>>();
	cbTrans_->Init();

	// 定数バッファへデータ転送
	cbTrans_->constMap_ = nullptr;
	cbTrans_->constBuff_->Map(0, nullptr, (void**)&cbTrans_->constMap_);

	NMatrix4 mat;
	cbTrans_->constMap_->viewproj = NCamera::sCurrentCamera->GetMatView() * NCamera::sCurrentCamera->GetMatProjection();
	cbTrans_->constMap_->world = mat.Identity();
	cbTrans_->constMap_->cameraPos = NCamera::sCurrentCamera->GetPos();

	cbTrans_->Unmap();

	//マテリアル仮設定
	cbMaterial_ = std::make_unique<NConstBuff<ConstBuffDataMaterial>>();
	cbMaterial_->Init();

	cbMaterial_->constMap_ = nullptr;
	cbMaterial_->constBuff_->Map(0, nullptr, (void**)&cbMaterial_->constMap_);

	cbMaterial_->constMap_->ambient = { 0.8f,0.8f,0.8f };
	cbMaterial_->constMap_->diffuse = { 0.3f,0.3f,0.3f };
	cbMaterial_->constMap_->specular = { 0.5f,0.5f,0.5f };
	cbMaterial_->constMap_->alpha = 1.0f;

	cbMaterial_->Unmap();

	//色仮設定
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbColor_->Init();

	cbColor_->constMap_ = nullptr;
	cbColor_->constBuff_->Map(0, nullptr, (void**)&cbColor_->constMap_);

	cbColor_->constMap_->color.SetColor();

	cbColor_->Unmap();

#pragma region オブジェクトの初期値設定

#pragma endregion
	//背景スプライト生成

	//前景スプライト生成
	foreSprite[0] = std::make_unique<NSprite>();
	foreSprite[0]->CreateSprite("hamu", { 0,0 });
	foreSprite[0]->SetPos(0, 0);
	foreSprite[0]->SetSize(100, 100);
	foreSprite[0]->color_.SetColor255(255, 255, 255, 255);

#pragma endregion
	// ライト生成
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init();
	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());

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
	//lightGroup_.get()->sDirLights.SetLightColor({ 0,1.0f,0 });
	lightGroup_->Update();

#pragma region 行列の計算
	//ビュー行列の再生成
	//NCamera::sCurrentCamera->SetEye({ 0.0f, 120.0f, 75.0f });
	camera.CreateMatView();
	NCamera::sCurrentCamera = &camera;

	if (isCol)
	{
		obj[0]->color_.SetColor255(255, 0, 0, 255);
	}
	else
	{
		obj[0]->color_.SetColor255(255, 255, 255, 255);
	}
	obj[2]->color_.SetColor255(255, 255, 255, 255);

	obj[0]->MoveKey();

	sphere.pos = obj[0]->position_;
	plane.distance = obj[2]->position_.Dot(plane.normal);

	for (auto& o : obj)
	{
		o->Update();
	}

	for (auto& lo : levelDataobj)
	{
		lo->Update();
	}

	isCol = NCollision::Sphere2PlaneCol(sphere, plane);

	foreSprite[0]->Update();
#pragma endregion
}

void NTitleScene::Draw()
{
#pragma region グラフィックスコマンド
	//背景スプライト

	//3Dオブジェクト
	/*for (size_t i = 0; i < obj.size(); i++)
	{
		obj[i]->CommonBeginDraw();
		obj[i]->Draw();
	}*/

	//for (size_t i = 0; i < levelDataobj.size(); i++)
	//{
	//	levelDataobj[i]->CommonBeginDraw();
	//	levelDataobj[i]->Draw();
	//}

	////メッシュの数だけインデックス分の描画を行う処理を回す
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto vbView_ = vertexBuffers_[i]->view; // そのメッシュに対応する頂点バッファ
	//	auto ibView = indexBuffers_s[i]->view; // そのメッシュに対応する頂点バッファ

	//	// パイプラインステートとルートシグネチャの設定コマンド
	//	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSet3d().pipelineState_.Get());
	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSet3d().rootSig_.entity_.Get());

	//	//ルートパラメータ2番に3D変換行列の定数バッファを渡す
	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTrans_->constBuff_->GetGPUVirtualAddress());
	//	//マテリアルとかカラーも送んなきゃいけないけど一旦後回し

	//	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//	NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);

	//	NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced((UINT)meshes[i].indices_.size(), 1, 0, 0, 0); // インデックスの数分描画する
	//}

	//メッシュの数だけインデックス分の描画を行う処理を回す
	for (size_t i = 0; i < meshes.size(); i++)
	{
		// パイプラインステートとルートシグネチャの設定コマンド
		NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSet3d().pipelineState_.Get());
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSet3d().rootSig_.entity_.Get());

		// プリミティブ形状の設定コマンド
		NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
		NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
		
		//ルートパラメータ0番にマテリアルの定数バッファを渡す
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbMaterial_->constBuff_->GetGPUVirtualAddress());
		//ルートパラメータ2番に色情報の定数バッファを渡す
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
		//ルートパラメータ2番に3D変換行列の定数バッファを渡す
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, cbTrans_->constBuff_->GetGPUVirtualAddress());
		
		NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBuffers_[i].view_);
		NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&indexBuffers_[i].view_);

		//指定のヒープにあるSRVをルートパラメータ1番に設定
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(
			1, NTextureManager::GetInstance()->textureMap_["error"].gpuHandle_);

		NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced((UINT)meshes[i].indices.size(), 1, 0, 0, 0); // インデックスの数分描画する
	}

	//前景スプライト
	foreSprite[0]->Draw();

	// 4.描画コマンドここまで
#pragma endregion
}

void NTitleScene::Reset()
{
	lightGroup_->Init();
	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());
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

		levelDataobj.back()->position_ = objectData.trans;
		levelDataobj.back()->rotation_ = objectData.rot;
		levelDataobj.back()->scale_ = objectData.scale;
		//obj.back()->SetMatWorld(objectData.matWorld_);
	/*}*/
	}
}
