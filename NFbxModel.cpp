#include <assimp/scene.h>
#include "NFbxModel.h"
#include "NAssimpLoader.h"


FbxModel::FbxModel()
{
	format = ModelFormat::Fbx;
}

void FbxModel::PlayAnimation()
{
	// �A�j���[�V�����Đ����Ȃ�
	if (animation.isPlay == false)
	{
		//�A�j���[�V�����^�C�}�[�������I��0�ɂ��čĐ�����Ȃ��悤��
		ParseNodeHeirarchy(0.f, 0, NMatrix4::Identity(), scene->mRootNode);
		return;
	}

	// �ő厞�Ԃƌ��݂̎��Ԏ擾
	float maxTime = (float)scene->mAnimations[animation.index]->mDuration;
	float nowTime = animation.timer.GetTimeRate() * maxTime;

	//�擾�������݂̃A�j���[�V�����^�C�}�[���g���ăA�j���[�V����������
	ParseNodeHeirarchy(nowTime, animation.index, NMatrix4::Identity(), scene->mRootNode);

	//�A�j���[�V�����^�C�}�[�񂵑����Ƃ�
	animation.timer.Update(true);
}

aiNodeAnim* FbxModel::FindNodeAnimation(const std::string& nodeName, aiAnimation* animation)
{
	return nullptr;
}

uint32_t FbxModel::FindScaleIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return 0;
}

uint32_t FbxModel::FindRotIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return 0;
}

uint32_t FbxModel::FindPosIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return 0;
}

NVec3 FbxModel::CalcCurrentScale(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return NVec3();
}

NVec3 FbxModel::CalcCurrentRot(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return NVec3();
}

NVec3 FbxModel::CalcCurrentPos(const aiNodeAnim* nodeAnim, const float currentTime)
{
	return NVec3();
}

void FbxModel::ParseNodeHeirarchy(const float currentTime, const uint32_t index, const NMatrix4& parentMat, const aiNode* rootNode)
{
	//aiMatrix4x4 aiMat = rootNode->mTransformation;

	//NMatrix4 currentPoseMat = NAssimpLoader::ConvertNMatrix4FromAssimpMat(aiMat).Transpose();

	//// �m�[�h�A�j���[�V�������擾����
	//std::string nodeName = rootNode->mName.C_Str();
	//aiAnimation* animation = scene->mAnimations[index];  // �K�؂ȃA�j���[�V������I������K�v������܂�

	//const aiNodeAnim* nodeAnim = FindNodeAnim(nodeName, animation);

	//// �m�[�h�A�j���[�V����������ꍇ�A�m�[�h�̕ϊ��s���⊮����
	//if (nodeAnim)
	//{
	//	// �X�P�[�����O��⊮
	//	Vec3 scale = CalcCurrentScale(nodeAnim, currentTime);

	//	// ��]��⊮
	//	Quaternion rot = CalcCurrentRot(nodeAnim, currentTime);

	//	// ���W��⊮
	//	Vec3 pos = CalcCurrentPos(nodeAnim, currentTime);

	//	// �s��̍���
	//	currentPoseMat = CalculateWorldMat(pos, scale, rot);
	//}

	//NMatrix4 globalTransformMat = currentPoseMat * parentMat;

	//for (uint32_t i = 0; i < bones.size(); i++)
	//{
	//	if (bones[i].name == nodeName)
	//	{
	//		NMatrix4 initalMat = bones[i].offsetMat;
	//		NMatrix4 invWorldMat = AssimpLoader::ConvertNMatrix4FromAssimpMat(scene->mRootNode->mTransformation);
	//		bones[i].currentMat = initalMat * globalTransformMat * invWorldMat;
	//	}
	//}

	//for (uint32_t i = 0; i < rootNode->mNumChildren; i++)
	//{
	//	ParseNodeHeirarchy(currentTime, index, globalTransformMat, rootNode->mChildren[i]);
	//}
}
