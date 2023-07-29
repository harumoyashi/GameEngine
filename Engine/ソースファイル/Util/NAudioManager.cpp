#include "NAudioManager.h"
#include "NUtil.h"

std::unordered_map<SoundHandle, uint32_t> NAudioManager::sSoundMap;

void NAudioManager::AllLoad()
{
    LoadSound("title_BGM.mp3", "titleBGM");     //�^�C�g����BGM
    LoadSound("play_BGM.mp3", "playBGM");       //�v���C����BGM
    LoadSound("clear_BGM.mp3", "clearBGM");     //�N���A���U���g��BGM
    LoadSound("faild_BGM.mp3", "faildBGM");     //���s���U���g��BGM

    LoadSound("shot_SE.mp3", "shotSE");                 //�e��������
    LoadSound("start_SE.mp3", "startSE");               //�X�^�[�g�̕����|���鉹
    LoadSound("vanish_SE.mp3", "vanishSE");             //�G���e���鉹
    LoadSound("explosion_SE.mp3", "explosionSE");       //�v���C���[���e���鉹
    LoadSound("sceneChange_SE.mp3", "sceneChangeSE");   //�V�[���J�ډ�
    LoadSound("dead_SE.mp3", "deadSE");                 //�v���C���[���S��
}

uint32_t NAudioManager::GetSound(const std::string& soundHandle)
{
    return sSoundMap[soundHandle];
}

void NAudioManager::LoadSound(const std::string& filename, const std::string& soundHandle)
{
    //�g���q�𒊏o���Ă���ɉ��������œǂݍ���
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
