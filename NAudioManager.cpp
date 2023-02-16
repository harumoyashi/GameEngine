#include "NAudioManager.h"

std::map<SoundHandle, uint32_t> NAudioManager::soundMap;

void NAudioManager::AllLoad()
{
    LoadSound("fanfare.wav", "WinSE");
    LoadSound("fever.wav", "Fever");
}

uint32_t NAudioManager::GetSound(const std::string& soundHandle)
{
    return soundMap[soundHandle];
}

void NAudioManager::LoadSound(const std::string& filename, const std::string soundHandle)
{
    uint32_t sound = NAudio::GetInstance()->LoadWave(filename);
    soundMap.insert(std::make_pair(soundHandle, sound));
}

void NAudioManager::Play(const std::string& soundHandle, const bool& isRoop, const float& volume, const int& roopNum)
{
    uint32_t a = soundMap[soundHandle];
    NAudio::GetInstance()->PlayWave(soundMap[soundHandle],isRoop,volume,roopNum);
}

void NAudioManager::Destroy(const std::string& soundHandle)
{
    NAudio::GetInstance()->DestroyWave(soundMap[soundHandle]);
}

void NAudioManager::Stop(const std::string& soundHandle)
{
    NAudio::GetInstance()->StopWave(soundMap[soundHandle]);
}

void NAudioManager::Start(const std::string& soundHandle)
{
    NAudio::GetInstance()->StartWave(soundMap[soundHandle]);
}

bool NAudioManager::GetIsPlaying(const std::string& soundHandle)
{
    return NAudio::GetInstance()->IsPlaying(soundMap[soundHandle]);
}
