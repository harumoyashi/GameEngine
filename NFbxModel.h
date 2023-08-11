#pragma once
#include "IModel.h"
#include "NTimer.h"

#include <assimp/Importer.hpp>

struct Bone
{
	std::string name;

	NMatrix4 offsetMat;
	NMatrix4 currentMat;
};

typedef struct Animation final
{
	uint32_t index = 0;
	NTimer timer;
	bool isPlay = false;
}Animation;

typedef struct Node final
{
	std::string name;
	NVec3 pos = NVec3::zero;
	NVec3 scale = NVec3::one;
	NVec3 rot = NVec3::zero;
	NMatrix4 localTransformMat;
	NMatrix4 globalTransformMat;
	Node* parent = nullptr;
}Node;

//前方宣言
struct aiNodeAnim;
struct aiAnimation;
struct aiNode;
struct aiScene;

struct FbxModel final
	: public IModel
{
	std::vector<Bone> bones;
	std::vector<Node> nodes;
	Animation animation;

	Assimp::Importer importer;
	const aiScene* scene;

	FbxModel();

	//アニメーションの再生
	void PlayAnimation();
	//
	aiNodeAnim* FindNodeAnimation(const std::string& nodeName, aiAnimation* animation);

	//
	uint32_t FindScaleIndex(const aiNodeAnim* nodeAnim, const float nowTime);
	//
	uint32_t FindRotIndex(const aiNodeAnim* nodeAnim, const float nowTime);
	//
	uint32_t FindPosIndex(const aiNodeAnim* nodeAnim, const float nowTime);

	//
	NVec3 CalcCurrentScale(const aiNodeAnim* nodeAnim, const float nowTime);
	//
	NVec3 CalcCurrentRot(const aiNodeAnim* nodeAnim, const float nowTime);
	//
	NVec3 CalcCurrentPos(const aiNodeAnim* nodeAnim, const float nowTime);
	//
	void ParseNodeHeirarchy(const float nowTime, const uint32_t index, const NMatrix4& parentMat, const aiNode* rootNode);
};