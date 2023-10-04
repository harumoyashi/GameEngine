#pragma once
#include "NAudio.h"
#include <unordered_map>

typedef std::string SoundHandle;
struct Sound
{
	uint32_t handle;	//�����n���h��
	bool isBGM;			//BGM���t���O
	float volume = 1.f;	//����
};

class NAudioManager final
{
private:
	static std::unordered_map<SoundHandle, Sound> sSoundMap;	//�T�E���h�Q

	static float masterVolume_;	//�}�X�^�[���ʕۑ��p
	static float bgmVolume_;	//BGM���ʕۑ��p
	static float seVolume_;		//SE���ʕۑ��p

private:
	//�S�Ẳ��ʐݒ�
	void AllSetVolume();

public:
	//�C���X�^���X�擾
	static NAudioManager* GetInstance();

	//ImGui
	void ImGuiDraw();

	//�S���̉��t�@�C���ǂݍ���
	void AllLoad();
	//�S�̂̉��ʒ���
	void SetMasterVolume(float masterVolume);
	//BGM�̉��ʒ���
	void SetBGMVolume(float bgmVolume);
	//SE�̉��ʒ���
	void SetSEVolume(float seVolume);
	//���ʐݒ�ǂݍ���
	void LoadVolume();
	//���ʐݒ�ۑ�
	void SaveVolume();

	//�T�E���h�f�[�^�̎擾
	uint32_t GetSound(const std::string& soundHandle);
	//wav��mp3�����ǂݍ���
	//"filename" = �t�@�C����
	void LoadSound(const std::string& filename, const std::string& soundHandle,bool isBGM);
	//�����Đ�
	//"soundHandle" �T�E���h�f�[�^�n���h��
	//"isRoop" ���[�v�Đ��t���O
	//"volume" �{�����[��
	//"roopNum" ���[�v����񐔁B0���ƃ��[�v�͂�����x���������
	//0�Ŗ����A1���f�t�H���g���ʁB���܂�傫����������Ɖ����ꂷ��
	void Play(const std::string& soundHandle, bool isRoop = false, const float volume = 1.0f, const int roopNum = 0);
	//�����폜
	//"voiceHandle" �����n���h��
	void Destroy(const std::string& soundHandle);
	//�����ꎞ��~
	//"voiceHandle" �����n���h��
	void Stop(const std::string& soundHandle);
	//�ꎞ��~�����������Đ�
	//"voiceHandle" �����n���h��
	void Start(const std::string& soundHandle);

	//�����Đ������ǂ���
	//"soundHandle" �����n���h��
	bool GetIsPlaying(const std::string& soundHandle);

	//���ʐݒ�
	//"soundHandle" �����n���h��
	//"volume" �{�����[��
	//0�Ŗ����A1���f�t�H���g���ʁB���܂�傫����������Ɖ����ꂷ��
	void SetVolume(const std::string& soundHandle, const float volume);
};

