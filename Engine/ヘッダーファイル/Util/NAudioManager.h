#pragma once
#include "NAudio.h"
#include <unordered_map>

typedef std::string SoundHandle;

class NAudioManager final
{
private:
	static std::unordered_map<SoundHandle, uint32_t> sSoundMap;	//�T�E���h�Q 

public:
	//�S���̉��t�@�C���ǂݍ���
	static void AllLoad();
	//�T�E���h�f�[�^�̎擾
	static uint32_t GetSound(const std::string& soundHandle);
	//WAV�����ǂݍ���
	//"filename" = WAV�t�@�C����
	static void LoadSound(const std::string& filename, const std::string& soundHandle);
	//�����Đ�
	//"soundHandle" �T�E���h�f�[�^�n���h��
	//"isRoop" ���[�v�Đ��t���O
	//"volume" �{�����[��
	//"roopNum" ���[�v����񐔁B0���ƃ��[�v�͂�����x���������
	//0�Ŗ����A1���f�t�H���g���ʁB���܂�傫����������Ɖ����ꂷ��
	static void Play(const std::string& soundHandle, bool isRoop = false, const float volume = 1.0f, const int roopNum = 0);
	//�����폜
	//"voiceHandle" �Đ��n���h��
	static void Destroy(const std::string& soundHandle);
	//�����ꎞ��~
	//"voiceHandle" �Đ��n���h��
	static void Stop(const std::string& soundHandle);
	//�ꎞ��~�����������Đ�
	//"voiceHandle" �Đ��n���h��
	static void Start(const std::string& soundHandle);
	//�����Đ������ǂ���
	//"soundHandle" �����n���h��
	static bool GetIsPlaying(const std::string& soundHandle);
};

