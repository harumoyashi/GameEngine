#include "NAudioManager.h"
#include "NUtil.h"

std::map<SoundHandle, uint32_t> NAudioManager::sSoundMap;

void NAudioManager::AllLoad()
{
    LoadSound("fanfare.wav", "WinSE");
    LoadSound("fever.wav", "Fever");
    LoadSound("retrogamecenter3.mp3", "RetroBGM");
}

uint32_t NAudioManager::GetSound(const std::string& soundHandle)
{
    return sSoundMap[soundHandle];
}

void NAudioManager::LoadSound(const std::string& filename, const std::string& soundHandle)
{
    //Šg’£Žq‚ð’Šo‚µ‚Ä‚»‚ê‚É‰ž‚¶‚½•û‚Å“Ç‚Ýž‚Ý
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
