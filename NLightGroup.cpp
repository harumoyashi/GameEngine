#include "NLightGroup.h"
#include "NObj3d.h"

NDirectionalLight NLightGroup::sDirLights = {};
NPointLight NLightGroup::sPointLights = {};
NSpotLight NLightGroup::sSpotLights = {};
NCircleShadow NLightGroup::sCircleShadows = {};

void NLightGroup::Init(const bool isDirLights, const bool isPointLights,
	const bool isSpotLights, const bool isCircleShadows)
{
	sDirLights.Init();
	sPointLights.Init();
	sSpotLights.Init();
	sCircleShadows.Init();
	if (!isDirLights)
	{
		sDirLights.SetActive(false);
	}

	if (!isPointLights)
	{
		sPointLights.SetActive(false);
	}

	if (!isSpotLights)
	{
		sSpotLights.SetActive(false);
	}

	if (!isCircleShadows)
	{
		sCircleShadows.SetActive(false);
	}
}

void NLightGroup::Update()
{
	sDirLights.Update();
	sPointLights.Update();
	sSpotLights.Update();
	sCircleShadows.Update();
}

void NLightGroup::Draw()
{
	sDirLights.Draw(4);
	sPointLights.Draw(5);
	sSpotLights.Draw(6);
	sCircleShadows.Draw(7);
}