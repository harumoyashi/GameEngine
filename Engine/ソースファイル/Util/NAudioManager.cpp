#include "NAudioManager.h"
#include "NUtil.h"

std::unordered_map<SoundHandle, Sound> NAudioManager::sSoundMap;

void NAudioManager::AllLoad()
{
	LoadSound("title_BGM.mp3", "titleBGM", true);     //タイトルのBGM
	LoadSound("play_BGM.mp3", "playBGM", true);       //プレイ中のBGM
	LoadSound("clear_BGM.mp3", "clearBGM", true);     //クリアリザルトのBGM
	LoadSound("faild_BGM.mp3", "faildBGM", true);     //失敗リザルトのBGM

	LoadSound("shot_SE.mp3", "shotSE", false);                 //弾撃った時
	LoadSound("start_SE.mp3", "startSE", false);               //スタートの文字掃ける音
	LoadSound("vanish_SE.mp3", "vanishSE", false);             //敵が弾ける音
	LoadSound("explosion_SE.mp3", "explosionSE", false);       //プレイヤーが弾ける音
	LoadSound("sceneChange_SE.mp3", "sceneChangeSE", false);   //シーン遷移音
	LoadSound("dead_SE.mp3", "deadSE", false);                 //プレイヤー死亡音
	LoadSound("item_get_SE.mp3", "itemGetSE", false);          //アイテム獲得音
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
