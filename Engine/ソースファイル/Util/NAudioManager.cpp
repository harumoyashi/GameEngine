#include "NAudioManager.h"
#include "NUtil.h"

std::unordered_map<SoundHandle, uint32_t> NAudioManager::sSoundMap;

void NAudioManager::AllLoad()
{
    LoadSound("title_BGM.mp3", "titleBGM");     //タイトルのBGM
    LoadSound("play_BGM.mp3", "playBGM");       //プレイ中のBGM
    LoadSound("clear_BGM.mp3", "clearBGM");     //クリアリザルトのBGM
    LoadSound("faild_BGM.mp3", "faildBGM");     //失敗リザルトのBGM

    LoadSound("shot_SE.mp3", "shotSE");                 //弾撃った時
    LoadSound("start_SE.mp3", "startSE");               //スタートの文字掃ける音
    LoadSound("vanish_SE.mp3", "vanishSE");             //敵が弾ける音
    LoadSound("explosion_SE.mp3", "explosionSE");       //プレイヤーが弾ける音
    LoadSound("sceneChange_SE.mp3", "sceneChangeSE");   //シーン遷移音
    LoadSound("dead_SE.mp3", "deadSE");                 //プレイヤー死亡音
}

uint32_t NAudioManager::GetSound(const std::string& soundHandle)
{
    return sSoundMap[soundHandle];
}

void NAudioManager::LoadSound(const std::string& filename, const std::string& soundHandle)
{
    //拡張子を抽出してそれに応じた方で読み込み
    std::string ext = NUtil::GetExtension(filename);
    if (ext == "wav")
    {
        uint32_t sound = NAudio::GetInstance()->LoadWave(filename);
        sSoundMap.insert(std::make_pair(soundHandle, sound));
    }
    else if (ext == "mp3")
    {
        uint32_t sound = NAudio::GetInstance()->LoadMP3(filename);
        sSoundMap.insert(std::make_pair(soundHandle, sound));
    }
}

void NAudioManager::Play(const std::string& soundHandle, bool isRoop, const float volume, const int roopNum)
{
    uint32_t a = sSoundMap[soundHandle];
    NAudio::GetInstance()->PlayWave(sSoundMap[soundHandle],isRoop,volume,roopNum);
}

void NAudioManager::Destroy(const std::string& soundHandle)
{
    NAudio::GetInstance()->DestroyWave(sSoundMap[soundHandle]);
}

void NAudioManager::Stop(const std::string& soundHandle)
{
    NAudio::GetInstance()->StopWave(sSoundMap[soundHandle]);
}

void NAudioManager::Start(const std::string& soundHandle)
{
    NAudio::GetInstance()->StartWave(sSoundMap[soundHandle]);
}

bool NAudioManager::GetIsPlaying(const std::string& soundHandle)
{
    return NAudio::GetInstance()->IsPlaying(sSoundMap[soundHandle]);
}
