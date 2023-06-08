#include "NDX12.h"
#include "NGameScene.h"
#include "NSceneManager.h"

NGameScene* NGameScene::GetInstance()
{
	static NGameScene instance;
	return &instance;
}

void NGameScene::Init()
{
#pragma region	オーディオ初期化
	audio = NAudio::GetInstance();
#pragma endregion
#pragma region	カメラ初期化
	camera.ProjectiveProjection();
	camera.CreateMatView();
	NCamera::sNowCamera = &camera;
#pragma endregion
#pragma region 描画初期化処理
	//マテリアル(定数バッファ)
	
	//立方体情報
	
	//モデル情報
	for (uint32_t i = 0; i < maxModel; i++)
	{
		model.emplace_back();
	}
	model[0].Create("sphere");
	model[1].Create("Cube");
	model[2].Create("busterSword");

	//オブジェクト
	for (uint32_t i = 0; i < maxObj; i++)
	{
		obj.emplace_back();
		obj[i] = std::make_unique<NObj3d>();
		obj[i]->Init();
	}
	obj[0]->SetModel(model[0]);
	obj[1]->SetModel(model[1]);
	obj[2]->SetModel(model[0]);
	obj[3]->SetModel(model[2]);

#pragma region オブジェクトの初期値設定
	obj[0]->position = { 0,0,0 };
	obj[1]->position = { 0,-2,0 };
	obj[1]->scale = { 10,0.1f,10 };
	obj[2]->position = { 2,0,0 };
	obj[3]->position = { -2,3,0 };
	//設定したのを適用
	for (uint32_t i = 0; i < maxObj; i++)
	{
		obj[i]->Update();
	}

#pragma endregion
	//背景スプライト生成

	//前景スプライト生成

#pragma endregion
	// ライト生成
	directionalLight = std::make_unique<NDirectionalLight>();
	directionalLight->Initialize();
	pointLight = std::make_unique<NPointLight>();
	pointLight->Initialize();
	spotLight = std::make_unique<NSpotLight>();
	spotLight->Initialize();
	circleShadow = std::make_unique<NCircleShadow>();
	circleShadow->Initialize();
	// 3Dオブジェクトにライトをセット
	NObj3d::SetNDirectionalLight(directionalLight.get());
	NObj3d::SetNPointLight(pointLight.get());
	NObj3d::SetNSpotLight(spotLight.get());
	NObj3d::SetNCircleShadow(circleShadow.get());
}

void NGameScene::Update()
{
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneManager::SetScene(TITLESCENE);
	}
#pragma region 行列の計算
	//ビュー行列の再生成
	camera.CreateMatView();
	NCamera::sNowCamera = &camera;

	obj[0]->MoveKey();
	obj[3]->MoveKey();

	for (size_t i = 0; i < maxObj; i++)
	{
		obj[i]->Update();
	}
#pragma endregion

	//ライトたちの更新
	directionalLight->Update();
	pointLight->Update();
	spotLight->Update();
	circleShadow->Update();
}

void NGameScene::Draw()
{
#pragma region グラフィックスコマンド
	//背景スプライト

	//3Dオブジェクト
	for (size_t i = 0; i < maxObj; i++)
	{
		obj[i]->CommonBeginDraw();
		obj[i]->Draw();
	}
	
	//前景スプライト

	// 4.描画コマンドここまで
#pragma endregion
}

void NGameScene::Reset()
{
	// 3Dオブジェクトにライトをセット
	NObj3d::SetNDirectionalLight(directionalLight.get());
	NObj3d::SetNPointLight(pointLight.get());
	NObj3d::SetNSpotLight(spotLight.get());
	NObj3d::SetNCircleShadow(circleShadow.get());
}

void NGameScene::Finalize()
{
}