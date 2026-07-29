#pragma once
#include "../../../Library/myDxLib.h"
#include <string>

class Sound
{
public:
	Sound();
	~Sound();

	void Update();
	/// <summary>
	/// 初期化するときにSoundのデータをセット
	/// </summary>
	/// <param name="_soundHandle">サウンドハンドル</param>
	/// <param name="_volume">ボリューム</param>
	void SoundData(int _soundHandle,int _volume);
	//通常再生
	void PlaySe()const;
	//bgmの再生
	void PlayBGM(bool _loop,bool firstPlay)const;

	//サウンドを止める
	void StopBGM()const;
	//サウンドを止める
	void StopSE()const;
	/// <summary>
	/// BGMのFeedIn開始
	/// </summary>
	/// <param name="time">フェードインする時間</param>
	void BGMFeedIn(float time);
	/// <summary>
	/// BGMのFeedOUT開始
	/// </summary>
	/// <param name="time">フェードアウトする時間</param>
	void BGMFeedOut(float time);

	/// <summary>
	/// 音のデータを使うかどうか
	/// </summary>
	/// <returns>音のデータを使うかどうか</returns>
	bool IsActive()const { return active; }
	/// <summary>
	/// Volumeを途中で変える時に使う
	/// </summary>
	/// <param name="_volume">音のボリューム</param>
	void ChangeVolumeSound(int _volume)const;
	/// <summary>
	/// 最初に設定したボリュームに戻す
	/// </summary>
	void BaseChangeVolumeSound()const;

	/// <summary>
	/// ボリュームを取得
	/// </summary>
	/// <returns>ボリューム</returns>
	const int GetVolumn()const { return volume; }

	/// <summary>
	/// 音の周波数を変える
	/// </summary>
	/// <param name="_ramdom">周波数がどれくらいランダムに変わるか</param>
	/// <param name="_baseFrequ">基準となる周波数</param>
	void PlayRamdomChangeFrequencySe(int _ramdom,int _baseFrequ);

private:

	int soundHandle;
	int volume;
	bool firstPlay;
	int lastPlayintBgm;

	float feedInTime;
	float feedOutTime;
	float timeMax;

	bool active;
};