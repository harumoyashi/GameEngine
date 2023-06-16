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
	//NAudioManager::Play("RetroBGM",true,0.2f);
#pragma endregion
#pragma region	カメラ初期化
	camera.SetEye({ 0.0f, 100.0f, 300.0f });
	camera.SetTarget({ 0.0f, 100.0f, 0.0f });
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
	levelData = NLevelDataLoader::GetInstance()->Load("C:/Users/K021G1126/source/repos/GE3/directX_CG/", "levelEditor.json");
	//SetObject(levelData.get());
	NLevelDataLoader::GetInstance()->SetObject(levelData.get(), levelDataobj);
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

	assimpModel.Init();

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

	assimpModel.Update();
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

	//assimpモデル描画//
	assimpModel.Draw();

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
