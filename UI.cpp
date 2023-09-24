#include "UI.h"

UI::UI()
{
}

UI* UI::GetInstance()
{
	static UI instance;
	return &instance;
}

void UI::Init()
{
	//ì«Ç›çûÇ›Å`
	uiSprite_[(uint32_t)UIType::Abutton].CreateClipSprite("Abutton", { 0.f,0 }, { 192.f,192.f });
	uiSprite_[(uint32_t)UIType::AbuttonPush].CreateClipSprite("Abutton", { 192.f,0 }, { 192.f,192.f });
	uiSprite_[(uint32_t)UIType::Lstick].CreateSprite("stick");
	uiSprite_[(uint32_t)UIType::Shaft].CreateSprite("shaft");
	uiSprite_[(uint32_t)UIType::Line].CreateSprite("lineUI");
	uiSprite_[(uint32_t)UIType::Side].CreateSprite("sideUI");
	uiSprite_[(uint32_t)UIType::Wide].CreateSprite("wideUI");
	uiSprite_[(uint32_t)UIType::Clear].CreateSprite("clear");
	uiSprite_[(uint32_t)UIType::Faild].CreateSprite("faild");
}

void UI::Update()
{
	for (uint32_t i = 0; i < (uint32_t)UIType::Max; i++)
	{
		uiSprite_[i].Update();
	}
}

void UI::Draw(UIType uiType)
{
	//éwíËÇ≥ÇÍÇΩUIÇï`âÊ
	uiSprite_[(uint32_t)uiType].Draw();
}

void UI::SetPos(UIType uiType, const NVec2& pos)
{
	uiSprite_[(uint32_t)uiType].SetPos(pos.x, pos.y);
}

void UI::SetSize(UIType uiType, const NVec2& size)
{
	uiSprite_[(uint32_t)uiType].SetSize(size.x,size.y);
}

void UI::SetAncorPoint(UIType uiType, const NVec2& ancorPoint)
{
	uiSprite_[(uint32_t)uiType].SetAncor(ancorPoint);
}

void UI::SetColor(UIType uiType, const NColor& color)
{
	uiSprite_[(uint32_t)uiType].color_ = color;
}

void UI::SetInvisible(UIType uiType, bool isInvisible)
{
	uiSprite_[(uint32_t)uiType].isInvisible_ = isInvisible;
}