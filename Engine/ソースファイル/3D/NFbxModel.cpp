#include <assimp/scene.h>
#include "NFbxModel.h"
#include "NAssimpLoader.h"
#include "NQuaternion.h"
#include "NMathUtil.h"


FbxModel::FbxModel()
{
	format = ModelFormat::Fbx;
}

void FbxModel::PlayAnimation()
{
	// アニメーション再生しない
	if (animation.isPlay == false)
	{
		//アニメーションタイマーを強制的に0にして再生されないように
		ParseNodeHeirarchy(0.f, 0, NMatrix4::Identity(), scene->mRootNode);
		return;
	}

	// 最大時間と現在の時間取得
	float maxTime = (float)scene->mAnimations[animation.index]->mDuration;
	float nowTime = animation.timer.GetTimeRate() * maxTime;

	//取得した現在のアニメーションタイマーを使ってアニメーションさせる
	ParseNodeHeirarchy(nowTime, animation.index, NMatrix4::Identity(), scene->mRootNode);

	//アニメーションタイマー回し続けとく
	animation.timer.Update(true);
}

aiNodeAnim* FbxModel::FindNodeAnimation(const std::string& nodeName, aiAnimation* animation)
{
	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* nodeAnim = animation->mChannels[i];
		if (nodeAnim->mNodeName.data == nodeName)
		{
			return nodeAnim;
		}
	}
	return nullptr;
}

uint32_t FbxModel::FindScaleIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	for (uint32_t i = 0; i < nodeAnim->mNumScalingKeys - 1; i++)
	{
		if (currentTime < nodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

uint32_t FbxModel::FindRotIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	for (uint32_t i = 0; i < nodeAnim->mNumRotationKeys - 1; i++)
	{
		if (currentTime >= nodeAnim->mRotationKeys[i].mTime &&
			currentTime < nodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

uint32_t FbxModel::FindPosIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	for (uint32_t i = 0; i < nodeAnim->mNumPositionKeys - 1; i++)
	{
		if (currentTime >= nodeAnim->mRotationKeys[i].mTime &&
			currentTime < nodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

NVec3 FbxModel::CalcCurrentScale(const aiNodeAnim* nodeAnim, const float currentTime)
{
	NVec3 result;
	if (nodeAnim->mNumScalingKeys == 1)
	{
		result.x = (float)nodeAnim->mScalingKeys[0].mValue.x;
		result.y = (float)nodeAnim->mScalingKeys[0].mValue.y;
		result.z = (float)nodeAnim->mScalingKeys[0].mValue.z;

		return result;
	}
	else if (nodeAnim->mNumScalingKeys > 1)
	{
		uint32_t scaleIndex = FindScaleIndex(nodeAnim, currentTime);
		uint32_t scaleNextIndex = scaleIndex + 1;

		aiVectorKey curKey = nodeAnim->mScalingKeys[scaleIndex];
		aiVectorKey nextKey = nodeAnim->mScalingKeys[scaleNextIndex];

		float deltaTime = (float)(nextKey.mTime - curKey.mTime);
		float factor = (float)(currentTime - curKey.mTime) / deltaTime;

		NVec3 start =
		{
			nodeAnim->mScalingKeys[scaleIndex].mValue.x,
			nodeAnim->mScalingKeys[scaleIndex].mValue.y,
			nodeAnim->mScalingKeys[scaleIndex].mValue.z,
		};

		NVec3 end =
		{
			nodeAnim->mScalingKeys[scaleNextIndex].mValue.x,
			nodeAnim->mScalingKeys[scaleNextIndex].mValue.y,
			nodeAnim->mScalingKeys[scaleNextIndex].mValue.z,
		};

		result = NVec3::Lerp(start, end, factor);
		return result;
	}

	return NVec3::zero;
}

NQuaternion FbxModel::CalcCurrentRot(const aiNodeAnim* nodeAnim, const float currentTime)
{
	NQuaternion result;
	if (nodeAnim->mNumRotationKeys == 1)
	{
		result.x = (float)nodeAnim->mRotationKeys[0].mValue.x;
		result.y = (float)nodeAnim->mRotationKeys[0].mValue.y;
		result.z = (float)nodeAnim->mRotationKeys[0].mValue.z;
		result.w = (float)nodeAnim->mRotationKeys[0].mValue.w;

		return result;
	}
	else if (nodeAnim->mNumRotationKeys > 1)
	{
		uint32_t rotIndex = FindRotIndex(nodeAnim, currentTime);
		uint32_t rotNextIndex = rotIndex + 1;

		aiQuatKey curKey = nodeAnim->mRotationKeys[rotIndex];
		aiQuatKey nextKey = nodeAnim->mRotationKeys[rotNextIndex];

		float deltaTime = (float)(nextKey.mTime - curKey.mTime);
		float factor = (float)(currentTime - curKey.mTime) / deltaTime;

		NQuaternion start =
		{
			nodeAnim->mRotationKeys[rotIndex].mValue.x,
			nodeAnim->mRotationKeys[rotIndex].mValue.y,
			nodeAnim->mRotationKeys[rotIndex].mValue.z,
			nodeAnim->mRotationKeys[rotIndex].mValue.w,
		};

		NQuaternion end =
		{
			nodeAnim->mRotationKeys[rotNextIndex].mValue.x,
			nodeAnim->mRotationKeys[rotNextIndex].mValue.y,
			nodeAnim->mRotationKeys[rotNextIndex].mValue.z,
			nodeAnim->mRotationKeys[rotNextIndex].mValue.w,
		};

		result = NQuaternion::Lerp(start, end, factor);
		return result;
	}

	return NQuaternion{};
}

NVec3 FbxModel::CalcCurrentPos(const aiNodeAnim* nodeAnim, const float currentTime)
{
	NVec3 result;
	if (nodeAnim->mNumPositionKeys == 1)
	{
		result.x = (float)nodeAnim->mPositionKeys[0].mValue.x;
		result.y = (float)nodeAnim->mPositionKeys[0].mValue.y;
		result.z = (float)nodeAnim->mPositionKeys[0].mValue.z;

		return result;
	}
	else if (nodeAnim->mNumPositionKeys > 1)
	{
		uint32_t posIndex = FindPosIndex(nodeAnim, currentTime);
		uint32_t posNextIndex = posIndex + 1;

		aiVectorKey curKey = nodeAnim->mPositionKeys[posIndex];
		aiVectorKey nextKey = nodeAnim->mPositionKeys[posNextIndex];

		float deltaTime = (float)(nextKey.mTime - curKey.mTime);
		float factor = (float)(currentTime - curKey.mTime) / deltaTime;

		NVec3 start =
		{
			nodeAnim->mPositionKeys[posIndex].mValue.x,
			nodeAnim->mPositionKeys[posIndex].mValue.y,
			nodeAnim->mPositionKeys[posIndex].mValue.z,
		};

		NVec3 end =
		{
			nodeAnim->mPositionKeys[posNextIndex].mValue.x,
			nodeAnim->mPositionKeys[posNextIndex].mValue.y,
			nodeAnim->mPositionKeys[posNextIndex].mValue.z,
		};

		result = NVec3::Lerp(start, end, factor);
		return result;
	}

	return NVec3::zero;
}

void FbxModel::ParseNodeHeirarchy(const float currentTime, const uint32_t index, const NMatrix4& parentMat, const aiNode* rootNode)
{
	aiMatrix4x4 aiMat = rootNode->mTransformation;

	NMatrix4 currentPoseMat = NAssimpLoader::AssimpMatToMat4(aiMat).Transpose();

	// ノードアニメーションを取得する
	std::string nodeName = rootNode->mName.C_Str();
	aiAnimation* animation = scene->mAnimations[index];  // 適切なアニメーションを選択する必要があります

	const aiNodeAnim* nodeAnim = FindNodeAnimation(nodeName, animation);

	// ノードアニメーションがある場合、ノードの変換行列を補完する
	if (nodeAnim)
	{
		// スケーリングを補完
		NVec3 scale = CalcCurrentScale(nodeAnim, currentTime);

		// 回転を補完
		NQuaternion rot = CalcCurrentRot(nodeAnim, currentTime);

		// 座標を補完
		NVec3 pos = CalcCurrentPos(nodeAnim, currentTime);

		// 行列の合成
		currentPoseMat = MathUtil::CalculateWorldMat(pos, scale, rot);
	}

	NMatrix4 globalTransformMat = currentPoseMat * parentMat;

	for (uint32_t i = 0; i < bones.size(); i++)
	{
		if (bones[i].name == nodeName)
		{
			NMatrix4 initalMat = bones[i].offsetMat;
			NMatrix4 invWorldMat = NAssimpLoader::AssimpMatToMat4(scene->mRootNode->mTransformation);
			bones[i].currentMat = initalMat * globalTransformMat * invWorldMat;
		}
	}

	for (uint32_t i = 0; i < rootNode->mNumChildren; i++)
	{
		ParseNodeHeirarchy(currentTime, index, globalTransformMat, rootNode->mChildren[i]);
	}
}
