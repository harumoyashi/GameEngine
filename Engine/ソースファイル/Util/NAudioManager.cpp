#include "NAudioManager.h"
#include "NUtil.h"

#include <fstream>
#include <string>
#include <sstream>

#include <functional>
#include "NImGuiManager.h"
#include "imgui.h"

std::unordered_map<SoundHandle, Sound> NAudioManager::sSoundMap;
float NAudioManager::masterVolume_ = 1.f;
float NAudioManager::bgmVolume_ = 1.f;
float NAudioManager::seVolume_ = 1.f;

void NAudioManager::AllSetVolume()
{
	for (auto& sound : sSoundMap)
	{
		if (sSoundMap[sound.first].isBGM)
		{
			//�ۑ����Ă��ʂ̉��� * �S�̂̉��� * BGM�̉���
			NAudio::GetInstance()->SetVolume(
				sSoundMap[sound.first].handle,
				sSoundMap[sound.first].volume * masterVolume_ * bgmVolume_
			);
		}
		else
		{
			//�ۑ����Ă��ʂ̉��� * �S�̂̉��� * SE�̉���
			NAudio::GetInstance()->SetVolume(
				sSoundMap[sound.first].handle,
				sSoundMap[sound.first].volume * masterVolume_ * seVolume_
			);
		}
	}
}

void NAudioManager::ImGuiDraw()
{
	ImGui::Begin("AudioVolume");
	ImGui::SliderFloat("MasterVolume",&masterVolume_,0.f,1.f);
	ImGui::SliderFloat("bgmVolume",&bgmVolume_,0.f,1.f);
	ImGui::SliderFloat("seVolume",&seVolume_,0.f,1.f);
	AllSetVolume();
	ImGui::End();
}

void NAudioManager::AllLoad()
{
	//BGM�ǂݍ���
	LoadSound("title_BGM.mp3", "titleBGM", true);     //�^�C�g����BGM
	LoadSound("play_BGM.mp3", "playBGM", true);       //�v���C����BGM
	LoadSound("clear_BGM.mp3", "clearBGM", true);     //�N���A���U���g��BGM
	LoadSound("faild_BGM.mp3", "faildBGM", true);     //���s���U���g��BGM

	//SE�ǂݍ���
	LoadSound("shot_SE.mp3", "shotSE", false);                 //�e��������
	LoadSound("start_SE.mp3", "startSE", false);               //�X�^�[�g�̕����|���鉹
	LoadSound("vanish_SE.mp3", "vanishSE", false);             //�G���e���鉹
	LoadSound("explosion_SE.mp3", "explosionSE", false);       //�v���C���[���e���鉹
	LoadSound("sceneChange_SE.mp3", "sceneChangeSE", false);   //�V�[���J�ډ�
	LoadSound("dead_SE.mp3", "deadSE", false);                 //�v���C���[���S��
	LoadSound("item_get_SE.mp3", "itemGetSE", false);          //�A�C�e���l����

	LoadVolume();	//�ۑ����Ă����ʃf�[�^�ǂݍ����
	AllSetVolume();	//���ʂ𒲐߂���
}

void NAudioManager::SetMasterVolume(float masterVolume)
{
	masterVolume_ = masterVolume;

	AllSetVolume();
}

void NAudioManager::SetBGMVolume(float bgmVolume)
{
	bgmVolume_ = bgmVolume;

	AllSetVolume();
}

void NAudioManager::SetSEVolume(float seVolume)
{
	seVolume_ = seVolume;

	AllSetVolume();
}

void NAudioManager::LoadVolume()
{
	//�t�@�C�����͏���
	std::ifstream readingFile;

	readingFile.open("./Resources/Data/volume.txt");
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (readingFile.fail())
	{
		assert(0);
	}

	std::string line;

	int i = 0;
	bool ispalletNum = false;
	while (getline(readingFile, line))
	{
		switch (i)
		{
		case 0:
			masterVolume_ = NUtil::StringToFloat(line);
			break;

		case 1:
			bgmVolume_ = NUtil::StringToFloat(line);
			break;

		case 2:
			seVolume_ = NUtil::StringToFloat(line);
			break;

		default:
			break;
		}

		i++;
	}
}

void NAudioManager::SaveVolume()
{
	//�t�@�C���o�͏���
	std::ofstream writingFile;

	std::string filename = "";
	filename = "./Resources/Data/volume.txt";

	writingFile.open(filename, std::ios::out);

	writingFile << masterVolume_;
	writingFile << "," << std::endl;

	writingFile << bgmVolume_;
	writingFile << "," << std::endl;

	writingFile << seVolume_;
	writingFile << "," << std::endl;

	writingFile.close();
}

uint32_t NAudioManager::GetSound(const std::string& soundHandle)
{
	return sSoundMap[soundHandle].handle;
}

NAudioManager* NAudioManager::GetInstance()
{
	static NAudioManager instance;
	return &instance;
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
	if (sSoundMap[soundHandle].isBGM)
	{
		NAudio::GetInstance()->PlayWave(
			sSoundMap[soundHandle].handle, isRoop,
			volume * masterVolume_ * bgmVolume_,
			roopNum
		);
	}
	else
	{
		NAudio::GetInstance()->PlayWave(
			sSoundMap[soundHandle].handle,
			isRoop, volume * masterVolume_ * seVolume_,
			roopNum
		);
	}
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
	sSoundMap[soundHandle].volume = volume;	//���ʕۑ�

	if (sSoundMap[soundHandle].isBGM)
	{
		//�w�肳�ꂽ�ʂ̉��� * �S�̂̉��� * BGM�̉���
		NAudio::GetInstance()->SetVolume(sSoundMap[soundHandle].handle, volume * masterVolume_ * bgmVolume_);
	}
	else
	{
		//�w�肳�ꂽ�ʂ̉��� * �S�̂̉��� * SE�̉���
		NAudio::GetInstance()->SetVolume(sSoundMap[soundHandle].handle, volume * masterVolume_ * seVolume_);
	}
}
