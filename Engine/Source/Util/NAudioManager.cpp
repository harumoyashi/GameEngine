#include "NAudioManager.h"
#include "NUtil.h"

#include <fstream>
#include <string>
#include <sstream>

#include <functional>
#include "NImGuiManager.h"
#include "imgui.h"

void NAudioManager::AllSetVolume()
{
	for (auto& sound : sSoundMap)
	{
		if (sSoundMap[sound.first].isBGM)
		{
			//保存してた個別の音量 * 全体の音量 * BGMの音量
			NAudio::GetInstance()->SetVolume(
				sSoundMap[sound.first].handle,
				sSoundMap[sound.first].volume * masterVolume_ * bgmVolume_
			);
		}
		else
		{
			//保存してた個別の音量 * 全体の音量 * SEの音量
			NAudio::GetInstance()->SetVolume(
				sSoundMap[sound.first].handle,
				sSoundMap[sound.first].volume * masterVolume_ * seVolume_
			);
		}
	}
}

void NAudioManager::AllLoad()
{
	//BGM読み込み
	LoadSound("title_BGM.mp3", "titleBGM", true);     //タイトルのBGM
	LoadSound("play_BGM.mp3", "playBGM", true);       //プレイ中のBGM
	LoadSound("clear_BGM.mp3", "clearBGM", true);     //クリアリザルトのBGM
	LoadSound("faild_BGM.mp3", "faildBGM", true);     //失敗リザルトのBGM

	//SE読み込み
	LoadSound("shot_SE.mp3", "shotSE", false);                 //弾撃った時
	LoadSound("start_SE.mp3", "startSE", false);               //スタートの文字掃ける音
	LoadSound("vanish_SE.mp3", "vanishSE", false);             //敵が弾ける音
	LoadSound("explosion_SE.mp3", "explosionSE", false);       //プレイヤーが弾ける音
	LoadSound("sceneChange_SE.mp3", "sceneChangeSE", false);   //シーン遷移音
	LoadSound("dead_SE.mp3", "deadSE", false);                 //プレイヤー死亡音
	LoadSound("item_get_SE.mp3", "itemGetSE", false);          //アイテム獲得音

	LoadVolume();	//保存してた音量データ読み込んで
	AllSetVolume();	//音量を調節する
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
	//ファイル入力処理
	std::ifstream readingFile;

	readingFile.open("./Resources/Data/volume.txt");
	//ファイルオープン失敗をチェック
	if (readingFile.fail())
	{
		assert(0);
	}

	std::string line;

	int i = 0;
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
	//ファイル出力処理
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
	//拡張子を抽出してそれに応じた方で読み込み
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
	SetVolume(soundHandle, volume);
	if (sSoundMap[soundHandle].isBGM)
	{
		NAudio::GetInstance()->PlayWave(
			sSoundMap[soundHandle].handle, isRoop,
			sSoundMap[soundHandle].volume * masterVolume_ * bgmVolume_,
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
	sSoundMap[soundHandle].volume = volume;	//音量保存

	if (sSoundMap[soundHandle].isBGM)
	{
		//指定された個別の音量 * 全体の音量 * BGMの音量
		NAudio::GetInstance()->SetVolume(sSoundMap[soundHandle].handle, volume * masterVolume_ * bgmVolume_);
	}
	else
	{
		//指定された個別の音量 * 全体の音量 * SEの音量
		NAudio::GetInstance()->SetVolume(sSoundMap[soundHandle].handle, volume * masterVolume_ * seVolume_);
	}
}
