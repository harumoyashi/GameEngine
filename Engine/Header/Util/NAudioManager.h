#pragma once
#include "NAudio.h"
#include <unordered_map>

// 音管理 //

typedef std::string SoundHandle;
struct Sound
{
	uint32_t handle;	//音声ハンドル
	bool isBGM;			//BGMかフラグ
	float volume = 1.f;	//音量
};

class NAudioManager final
{
private:
	std::unordered_map<SoundHandle, Sound> sSoundMap;	//サウンド群

	float masterVolume_;	//マスター音量保存用
	float bgmVolume_;		//BGM音量保存用
	float seVolume_;		//SE音量保存用

private:
	//全ての音量設定
	void AllSetVolume();

public:
	//シングルトンインスタンス取得
	static NAudioManager* GetInstance();

	//全部の音ファイル読み込み
	void AllLoad();

	//全体の音量調整
	void SetMasterVolume(float masterVolume);
	//BGMの音量調整
	void SetBGMVolume(float bgmVolume);
	//SEの音量調整
	void SetSEVolume(float seVolume);

	//全体の音量取得
	float GetMasterVolume() { return masterVolume_; }
	//BGMの音量取得
	float GetBGMVolume() { return bgmVolume_; }
	//SEの音量取得
	float GetSEVolume() { return seVolume_; }

	//音量設定読み込み
	void LoadVolume();
	//音量設定保存
	void SaveVolume();

	//サウンドデータの取得
	uint32_t GetSound(const std::string& soundHandle);
	//wavかmp3音声読み込み
	//"filename" = ファイル名
	void LoadSound(const std::string& filename, const std::string& soundHandle, bool isBGM);
	//音声再生
	//"soundHandle" サウンドデータハンドル
	//"isRoop" ループ再生フラグ
	//"volume" ボリューム
	//"roopNum" ループする回数。0だとループはせず一度だけ流れる
	//0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする
	void Play(const std::string& soundHandle, bool isRoop = false, const float volume = 1.0f, const int roopNum = 0);
	//音声削除
	//"voiceHandle" 音声ハンドル
	void Destroy(const std::string& soundHandle);
	//音声一時停止
	//"voiceHandle" 音声ハンドル
	void Stop(const std::string& soundHandle);
	//一時停止した音声を再生
	//"voiceHandle" 音声ハンドル
	void Start(const std::string& soundHandle);

	//音声再生中かどうか
	//"soundHandle" 音声ハンドル
	bool GetIsPlaying(const std::string& soundHandle);

	//音量設定
	//"soundHandle" 音声ハンドル
	//"volume" ボリューム
	//0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする
	void SetVolume(const std::string& soundHandle, const float volume);
};

