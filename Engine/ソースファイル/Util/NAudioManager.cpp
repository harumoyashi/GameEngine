#include "NAudioManager.h"
#include "NUtil.h"

std::unordered_map<SoundHandle, Sound> NAudioManager::sSoundMap;

void NAudioManager::AllLoad()
{
	LoadSound("title_BGM.mp3", "titleBGM", true);     //�^�C�g����BGM
	LoadSound("play_BGM.mp3", "playBGM", true);       //�v���C����BGM
	LoadSound("clear_BGM.mp3", "clearBGM", true);     //�N���A���U���g��BGM
	LoadSound("faild_BGM.mp3", "faildBGM", true);     //���s���U���g��BGM

	LoadSound("shot_SE.mp3", "shotSE", false);                 //�e��������
	LoadSound("start_SE.mp3", "startSE", false);               //�X�^�[�g�̕����|���鉹
	LoadSound("vanish_SE.mp3", "vanishSE", false);             //�G���e���鉹
	LoadSound("explosion_SE.mp3", "explosionSE", false);       //�v���C���[���e���鉹
	LoadSound("sceneChange_SE.mp3", "sceneChangeSE", false);   //�V�[���J�ډ�
	LoadSound("dead_SE.mp3", "deadSE", false);                 //�v���C���[���S��
	LoadSound("item_get_SE.mp3", "itemGetSE", false);          //�A�C�e���l����
}

void NAudioManager::SetMasterVolume(float masterVolume)
{
	for (auto& sound : sSoundMap)
	{
		NAudio::GetInstance()->SetVolume(sSoundMap[sound.first].handle, masterVolume);
	}
}

uint32_t NAudioManager::GetSound(const std::string& soundHandle)
{
	return sSoundMap[soundHandle].handle;
}

void NAudioManager::LoadSound(const std::string& filename, const std::string& soundHandle, bool isBGM)
{
	//�g���q�𒊏o���Ă���ɉ��������œǂݍ���
	std::string ext = NUtil::GetExtension(filename);
	if (ext == "wav")
	{
		uint32_t handle = NAudio::GetInstance()->LoadWave(filename);
		Sound sound = { handle,isBGM };
		sSoundMap.insert(std::make_pair(soundHandle, sound));
	}
	else if (ext == "mp3")
	{
		uint32_t handle = NAudio::GetInstance()->LoadMP3(filename);
		Sound sound = { handle,isBGM };
		sSoundMap.insert(std::make_pair(soundHandle, sound));
	}
}

void NAudioManager::Play(const std::string& soundHandle, bool isRoop, const float volume, const int roopNum)
{
	NAudio::GetInstance()->PlayWave(sSoundMap[soundHandle].handle, isRoop, volume, roopNum);
}

void NAudioManager::Destroy(const std::string& soundHandle)
{
	NAudio::GetInstance()->DestroyWave(sSoundMap[soundHandle].handle);
}

void NAudioManager::Stop(const std::string& soundHandle)
{
	NAudio::GetInstance()->StopWave(sSoundMap[soundHandle].handle);
}

void NAudioManager::Start(const std::string& soundHandle)
{
	NAudio::GetInstance()->StartWave(sSoundMap[soundHandle].handle);
}

bool NAudioManager::GetIsPlaying(const std::string& soundHandle)
{
	return NAudio::GetInstance()->IsPlaying(sSoundMap[soundHandle].handle);
}

void NAudioManager::SetVolume(const std::string& soundHandle, const float volume)
{
	NAudio::GetInstance()->SetVolume(sSoundMap[soundHandle].handle, volume);
}
