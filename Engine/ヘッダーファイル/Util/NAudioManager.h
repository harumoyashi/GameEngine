#pragma once
#include "NAudio.h"
#include <unordered_map>

typedef std::string SoundHandle;
struct Sound
{
	uint32_t handle;	//音声ハンドル
	bool isBGM;			//BGMかフラグ
};

class NAudioManager final
{
private:
	static std::unordered_map<SoundHandle, Sound> sSoundMap;	//サウンド群

	static float masterVolume_;	//マスター音量保存用
	static float bgmVolume_;	//BGM音量保存用
	static float seVolume_;		//SE音量保存用

public:
	//全部の音ファイル読み込み
	static void AllLoad();
	//全部の音ファイルの音量調整
	static void SetMasterVolume(float masterVolume);
	//サウンドデータの取得
	static uint32_t GetSound(const std::string& soundHandle);
	//WAV音声読み込み
	//"filename" = WAVファイル名
	static void LoadSound(const std::string& filename, const std::string& soundHandle,bool isBGM);
	//音声再生
	//"soundHandle" サウンドデータハンドル
	//"isRoop" ループ再生フラグ
	//"volume" ボリューム
	//"roopNum" ループする回数。0だとループはせず一度だけ流れる
	//0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする
	static void Play(const std::string& soundHandle, bool isRoop = false, const float volume = 1.0f, const int roopNum = 0);
	//音声削除
	//"voiceHandle" 音声ハンドル
	static void Destroy(const std::string& soundHandle);
	//音声一時停止
	//"voiceHandle" 音声ハンドル
	static void Stop(const std::string& soundHandle);
	//一時停止した音声を再生
	//"voiceHandle" 音声ハンドル
	static void Start(const std::string& soundHandle);
	//音声再生中かどうか
	//"soundHandle" 音声ハンドル
	static bool GetIsPlaying(const std::string& soundHandle);
	//音量設定
	//"soundHandle" 音声ハンドル
	//"volume" ボリューム
	//0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする
	static void SetVolume(const std::string& soundHandle, const float volume);
};

