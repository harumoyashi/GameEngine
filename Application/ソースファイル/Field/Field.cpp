#include "Field.h"
#include "Player.h"
#include "EnemyFactory.h"

#include "NAudioManager.h"

#include "NImGuiManager.h"
#include "imgui.h"

Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}

void Field::Init()
{
	isStart_ = false;
	isGoal_ = false;

#pragma region �I�u�W�F�N�g�̐���
	fieldObj_ = std::make_unique<NTile>();
	fieldObj_->Init();
	fieldObj_->SetModel("plane");
	fieldObj_->SetTexture("tile");
	fieldObj_->color_.SetColor255(5, 5, 5, 255);
	fieldObj_->scale_ = { 100.0f,0.01f,1000.0f };
	fieldObj_->position_ = { 0,-0.1f,fieldObj_->scale_.z - 100.0f };
	fieldObj_->SetDivide(tileDivide_);
	fieldObj_->SetActivityArea(activityAreaX_);

	lines_.clear();	//���S�������Ă��琶��������
	for (uint32_t i = 0; i < (uint32_t)LineType::MaxSize; i++)
	{
		lines_.emplace_back();
		lines_.back().line = std::make_unique<NObj3d>();
		lines_.back().line->Init();
		lines_.back().line->SetModel("plane");
		lines_.back().line->scale_ = { fieldObj_->scale_.x * 0.1f,1.0f, 0.05f };
		lines_.back().text = std::make_unique<NObj3d>();
		lines_.back().text->Init();
		lines_.back().text->SetModel("plane");
		lines_.back().text->scale_ = { 1.5f,1.0f,0.25f };

		lines_.back().isSlide = false;
		lines_.back().slidePos = 0.0f;
		lines_.back().slideTimer.Reset();
	}

	checkPoints_.clear();	//���S�������Ă��琶��������
	for (uint32_t i = 0; i < checkPointNum; i++)
	{
		checkPoints_.emplace_back();
		checkPoints_.back().line = std::make_unique<NObj3d>();
		checkPoints_.back().line->Init();
		checkPoints_.back().line->SetModel("plane");
		checkPoints_.back().line->scale_ = { fieldObj_->scale_.x * 0.1f,1.0f, 0.05f };
		checkPoints_.back().text = std::make_unique<NObj3d>();
		checkPoints_.back().text->Init();
		checkPoints_.back().text->SetModel("plane");
		checkPoints_.back().text->scale_ = { 2.5f,1.0f,0.25f };
		checkPoints_.back().text->SetTexture("checkpoint");

		checkPoints_.back().isSlide = false;
		checkPoints_.back().slidePos = 0.0f;
		checkPoints_.back().slideTimer.Reset();
	}
#pragma endregion
#pragma region �e�I�u�W�F�N�g�̐ݒ�
	lines_[(uint32_t)LineType::Start].text->SetTexture("start");
	lines_[(uint32_t)LineType::Start].offset = 6.0f;
	lines_[(uint32_t)LineType::Start].text->position_ = { lines_[(uint32_t)LineType::Start].offset,0, startPosZ_ - 0.5f };
	lines_[(uint32_t)LineType::Start].line->position_ =
	{
		Player::GetInstance()->GetPos().x + lines_[(uint32_t)LineType::Start].slidePos,0, startPosZ_
	};
	lines_[(uint32_t)LineType::Start].text->position_ =
	{
		Player::GetInstance()->GetPos().x +
		lines_[(uint32_t)LineType::Start].offset +
		lines_[(uint32_t)LineType::Start].slidePos,
		0, startPosZ_ - 0.5f
	};

	lines_[(uint32_t)LineType::Goal].text->SetTexture("goal");
	lines_[(uint32_t)LineType::Goal].offset = 6.0f;
	lines_[(uint32_t)LineType::Goal].text->position_ = { lines_[(uint32_t)LineType::Goal].offset,0, goalPosZ_ - 0.5f };
	lines_[(uint32_t)LineType::Goal].line->position_ =
	{
		Player::GetInstance()->GetPos().x + lines_[(uint32_t)LineType::Goal].slidePos,0, goalPosZ_
	};
	lines_[(uint32_t)LineType::Goal].text->position_ =
	{
		Player::GetInstance()->GetPos().x +
		lines_[(uint32_t)LineType::Goal].offset +
		lines_[(uint32_t)LineType::Goal].slidePos,
		0, goalPosZ_ - 0.5f
	};
#pragma endregion
}

void Field::Update()
{
#pragma region ���W��K�p
	lines_[(uint32_t)LineType::Start].line->position_ =
	{
		Player::GetInstance()->GetPos().x + lines_[(uint32_t)LineType::Start].slidePos,0, startPosZ_
	};
	lines_[(uint32_t)LineType::Start].text->position_ =
	{
		Player::GetInstance()->GetPos().x +
		lines_[(uint32_t)LineType::Start].offset +
		lines_[(uint32_t)LineType::Start].slidePos,
		0, startPosZ_ - 0.5f
	};

	lines_[(uint32_t)LineType::Goal].line->position_ =
	{
		Player::GetInstance()->GetPos().x + lines_[(uint32_t)LineType::Goal].slidePos,0, goalPosZ_
	};
	lines_[(uint32_t)LineType::Goal].text->position_ =
	{
		Player::GetInstance()->GetPos().x +
		lines_[(uint32_t)LineType::Goal].offset +
		lines_[(uint32_t)LineType::Goal].slidePos,
		0, goalPosZ_ - 0.5f
	};

	for (uint32_t i = 0; i < checkPointNum; i++)
	{
		//�X�^�[�g����S�[���܂ł�n+1�������āA1~n-1�Ԗڂ̐��ɃX�^�[�g�ʒu���������W���`�F�b�N�|�C���g�ɂ���
		float posZ = startPosZ_ + (goalPosZ_ - startPosZ_) / (float)(checkPointNum + 1) * (float)(i + 1);
		checkPoints_[i].line->position_ =
		{
			Player::GetInstance()->GetPos().x + checkPoints_[i].slidePos,0, posZ
		};
		checkPoints_[i].text->position_ =
		{
			Player::GetInstance()->GetPos().x +
			checkPoints_[i].offset +
			checkPoints_[i].slidePos,
			0, posZ - 0.5f
		};
	}
#pragma endregion 
	//�X�^�[�g���ĂȂ��Ƃ�
	if (isStart_ == false)
	{
		//���𒴂�����X�^�[�g��������true��
		if (startPosZ_ <= Player::GetInstance()->GetPos().z)
		{
			isStart_ = true;
			lines_[(uint32_t)LineType::Start].isSlide = true;
		}
	}

	//�X�^�[�g�����Ȃ�
	if (isStart_)
	{
		//��ʍ��O�܂łԂ���΂�
		lines_[(uint32_t)LineType::Start].slidePos =
			NEasing::InQuad(0.0f,
				-fieldObj_->scale_.x,
				lines_[(uint32_t)LineType::Start].slideTimer.GetTimeRate());
		//��ʍ��O�܂łԂ���΂�
		for (uint32_t i = 0; i < checkPointNum; i++)
		{
			checkPoints_[i].slidePos =
				NEasing::InQuad(0.0f, -fieldObj_->scale_.x, checkPoints_[i].slideTimer.GetTimeRate());
		}

		//�X���C�h����������
		if (lines_[(uint32_t)LineType::Start].slideTimer.GetEnd())
		{
		}

		//���𒴂�����X�^�[�g��������true��
		for (uint32_t i = 0; i < checkPointNum; i++)
		{
			//�X�^�[�g����S�[���܂ł�n+1�������āA1~n-1�Ԗڂ̐��ɃX�^�[�g�ʒu���������W���`�F�b�N�|�C���g�ɂ���
			float posZ = startPosZ_ + (goalPosZ_ - startPosZ_) / (float)(checkPointNum + 1) * (float)(i + 1);
			if (posZ <= Player::GetInstance()->GetPos().z)
			{
				checkPoints_[i].isSlide = true;
			}
		}

		//------------------------------------- �G�̐������� -------------------------------------//
		//�G�o�����Ⴄ��{�^��
		if (NInput::IsKeyDown(DIK_C))
		{
			EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(10, 0, 8));
		}
	}

	//�S�[�����ĂȂ��Ƃ�
	if (isGoal_ == false)
	{
		//���𒴂�����X�^�[�g��������true��
		if (goalPosZ_ <= Player::GetInstance()->GetPos().z)
		{
			isGoal_ = true;
			lines_[(uint32_t)LineType::Goal].isSlide = true;
		}
	}

	//�S�[�������Ȃ�
	if (isGoal_)
	{
		//��ʍ��O�܂łԂ���΂�
		lines_[(uint32_t)LineType::Goal].slidePos =
			NEasing::InQuad(0.0f,
				-fieldObj_->scale_.x,
				lines_[(uint32_t)LineType::Goal].slideTimer.GetTimeRate());

		//�X���C�h����������
		if (lines_[(uint32_t)LineType::Goal].slideTimer.GetEnd())
		{
		}
	}

	//�X���C�h���Ă����Ȃ�
	for (uint32_t i = 0; i < (uint32_t)LineType::MaxSize; i++)
	{
		if (lines_[i].isSlide)
		{
			//�X�^�[�g���X���C�h�^�C�}�[�J�n
			if (lines_[i].slideTimer.GetStarted() == false)
			{
				lines_[i].slideTimer.Start();
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(10, 0, 8));
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(-10, 0, 5));

				NAudioManager::Play("startSE");
				lines_[i].isSlide = false;	//�X���C�h�����Ⴞ�߂ɂ���
			}
		}
		//�^�C�}�[�X�V
		lines_[i].slideTimer.Update();
	}
	//�X���C�h���Ă����Ȃ�
	for (uint32_t i = 0; i < checkPointNum; i++)
	{
		if (checkPoints_[i].isSlide)
		{
			//�X�^�[�g���X���C�h�^�C�}�[�J�n
			if (checkPoints_[i].slideTimer.GetStarted() == false)
			{
				checkPoints_[i].slideTimer.Start();
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(10, 0, 8));
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(-10, 0, 5));

				NAudioManager::Play("startSE");
				checkPoints_[i].isSlide = false;	//�X���C�h�����Ⴞ�߂ɂ���
			}
		}
		//�^�C�}�[�X�V
		checkPoints_[i].slideTimer.Update();
	}

	//�I�u�W�F�N�g�̍X�V
	fieldObj_->SetDivide(tileDivide_);
	fieldObj_->SetActivityArea(activityAreaX_);
	fieldObj_->Update();

	for (uint32_t i = 0; i < (uint32_t)LineType::MaxSize; i++)
	{
		lines_[i].line->Update();
		lines_[i].text->Update();
	}

	for (uint32_t i = 0; i < checkPointNum; i++)
	{
		checkPoints_[i].line->Update();
		checkPoints_[i].text->Update();
	}

#ifdef _DEBUG
	ImGui::Begin("FieldParameter");
	//1F~180F�܂ł̊ԂɂƂǂ߂�
	ImGui::SliderFloat("StartPosZ", &startPosZ_, 1.0f, 180.0f);
	ImGui::SliderFloat("GoalPosZ", &goalPosZ_, 1.0f, 180.0f);
	ImGui::SliderFloat("Divide", &tileDivide_, 0.0f, 10.0f);
	ImGui::SliderFloat("ActivityArea", &activityAreaX_, 1.0f, 100.0f);
	ImGui::End();
#endif //DEBUG
}

void Field::Draw()
{
	//�������^�C�����O����
	NTile::CommonBeginDraw();
	fieldObj_->Draw();
	NObj3d::CommonBeginDraw();

	NObj3d::SetBlendMode(BlendMode::Alpha);
	for (uint32_t i = 0; i < (uint32_t)LineType::MaxSize; i++)
	{
		lines_[i].line->Draw();
		lines_[i].text->Draw();
	}

	for (uint32_t i = 0; i < checkPointNum; i++)
	{
		checkPoints_[i].line->Draw();
		checkPoints_[i].text->Draw();
	}
	NObj3d::SetBlendMode(BlendMode::None);
}