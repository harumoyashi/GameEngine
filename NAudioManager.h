#pragma once
#include "NAudio.h"
#include <unordered_map>

typedef std::string SoundHandle;

class NAudioManager final
{
private:
	static std::unordered_map<SoundHandle, uint32_t> sSoundMap;	//サウンド群 

public:
	//全部の音ファイル読み込み
	static void AllLoad();
	//サウンドデータの取得
	static uint32_t GetSound(const std::string& soundHandle);
	//WAV音声読み込み
	//"filename" = WAVファイル名
	static void LoadSound(const std::string& filename, const std::string& soundHandle);
	//音声再生
	//"soundHandle" サウンドデータハンドル
	//"isRoop" ループ再生フラグ
	//"volume" ボリューム
	//"roopNum" ループする回数。0だとループはせず一度だけ流れる
	//0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする
	static void Play(const std::string& soundHandle, bool isRoop = false, const float volume = 1.0f, const int roopNum = 0);
	//音声削除
	//"voiceHandle" 再生ハンドル
	static void Destroy(const std::string& soundHandle);
	//音声一時停止
	//"voiceHandle" 再生ハンドル
	static void Stop(const std::string& soundHandle);
	//一時停止した音声を再生
	//"voiceHandle" 再生ハンドル
	static void Start(const std::string& soundHandle);
	//音声再生中かどうか
	//"soundHandle" 音声ハンドル
	static bool GetIsPlaying(const std::string& soundHandle);
};

