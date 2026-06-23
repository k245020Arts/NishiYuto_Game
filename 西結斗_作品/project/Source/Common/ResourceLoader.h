#pragma once
#include "../../Library/GameObject.h"
#include "ID/ID.h"
#include "ID/SoundID.h"
#include "ID/EffectID.h"
#include "ID/FontID.h"
/// <summary>
/// ロードを一括で管理する名前空間
/// map上で管理されてそこからGetHandleをすることによって画像データを得ることが出来る
/// </summary>

namespace ResourceLoad {

	//ロードを管理しているクラス。
	//初期化処理
	void Init();
	/// <summary>
	/// モデルのロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="id">設定するID</param>
	/// <returns>モデルデータ</returns>
	int LoadModel(std::string path, ID::IDType id);
	/// <summary>
	/// モデルのロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="id">設定するID</param>
	/// <param name="_common">コモンデータに入れるかどうかtrueなら入れる</param>
	/// <returns>モデルデータ</returns>
	int LoadModel(std::string path, ID::IDType id, bool _common);
	/// <summary>
	/// サウンドのロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="exten">拡張子</param>
	/// <param name="id">設定するID</param>
	/// <returns>サウンドデータ</returns>
	int LoadSound(std::string path, std::string exten, Sound_ID::SOUND_ID id);
	/// <summary>
	/// サウンドのロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="exten">拡張子</param>
	/// <param name="id">設定するID</param>
	/// <param name="_common">コモンデータに入れるかどうかtrueなら入れる</param>
	/// <returns>サウンドデータ</returns>
	int LoadSound(std::string path, std::string exten, Sound_ID::SOUND_ID id, bool _common);
	/// <summary>
	/// エフェクトのロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="id">設定するID</param>
	/// <param name="size">エフェクトの大きさ</param>
	/// <returns>エフェクトデータ</returns>
	int LoadEffect(std::string path, std::string _exten,Effect_ID::EFFECT_ID id, float size);
	/// <summary>
	/// エフェクトのロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="id">設定するID</param>
	/// <param name="size">エフェクトの大きさ</param>
	/// <param name="_common">コモンデータに入れるかどうかtrueなら入れる</param>
	/// <returns>エフェクトデータ</returns>
	int LoadEffect(std::string path, std::string _exten, Effect_ID::EFFECT_ID id, float size, bool _common);
	/// <summary>
	/// 画像のロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="id">設定するID</param>
	/// <returns>画像データ</returns>
	int LoadImageGraph(std::string path, ID::IDType id);
	/// <summary>
	/// 画像のロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="id">設定するID</param>
	/// <param name="_common">コモンデータに入れるかどうかtrueなら入れる</param>
	/// <returns>画像データ</returns>
	int LoadImageGraph(std::string path, ID::IDType id, bool _common);
	/// <summary>
	/// アニメーションデータのロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="id">設定するID</param>
	/// <returns>アニメーションデータ</returns>
	int LoadAnim(std::string path, ID::IDType id);
	/// <summary>
	/// アニメーションデータのロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="id">設定するID</param>
	/// <param name="_common">コモンデータに入れるかどうかtrueなら入れる</param>
	/// <returns>アニメーションデータ</returns>
	int LoadAnim(std::string path, ID::IDType id, bool _common);

	/// <summary>
	/// サウンドのロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="exten">拡張子</param>
	/// <param name="id">設定するID</param>
	/// <returns>サウンドデータ</returns>
	int LoadFont(std::string path, std::string exten, Font_ID::Font_ID id,int _edge);
	/// <summary>
	/// サウンドのロードをする
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="exten">拡張子</param>
	/// <param name="id">設定するID</param>
	/// <param name="_common">コモンデータに入れるかどうかtrueなら入れる</param>
	/// <returns>サウンドデータ</returns>
	int LoadFont(std::string path, std::string exten, Font_ID::Font_ID id, int _edge, bool _common);

	/// <summary>
	/// 指定したデータの削除
	/// </summary>
	/// <param name="id">指定したID</param>
	/// <param name="id">commoonフォルダを検索するかどうか</param>
	void DeleteData(ID::IDType id,bool _common);
	void DeleteEffectData(Effect_ID::EFFECT_ID id,bool _common);

	/// <summary>
	/// すべてのデータを削除
	/// </summary>
	void AllDelete();
	/// <summary>
	/// 指定したIDのデータを取得
	/// </summary>
	/// <param name="id">指定したID</param>
	/// <returns>データ</returns>
	int GetHandle(ID::IDType id);
	/// <summary>
	/// 指定したIDのデータを取得
	/// </summary>
	/// <param name="id">指定したID</param>
	/// <returns>データ</returns>
	int GetSoundHandle(Sound_ID::SOUND_ID id);
	/// <summary>
	/// 指定したIDのデータを取得
	/// </summary>
	/// <param name="id">指定したID</param>
	/// <returns>データ</returns>
	int GetEffectHandle(Effect_ID::EFFECT_ID id);
	/// <summary>
	/// 指定したIDのデータを取得
	/// </summary>
	/// <param name="id">指定したID</param>
	/// <returns>データ</returns>
	int GetFontHandle(Font_ID::Font_ID id);
	/// <summary>
	/// 非同期読み込みを開始するかどうか
	/// </summary>
	/// <param name="_async">非同期同期の始まりをtrueにする</param>
	void SetAsync(bool _async);
	/// <summary>
	/// 非同期読み込み中か判定
	/// </summary>
	/// <returns>読み込み中ならtrue</returns>
	bool IsLoading();

	/// <summary>
	/// commonデータ以外のデータを全削除
	/// </summary>
	void FileLoadClear();
	//int GetCounter();
	//パスの定数
	//パスを毎回書くのは手間なので定数にして行う
	const std::string MODEL_PATH = "data/model/";
	const std::string ANIM_PATH = "data/model/animation/";
	const std::string IMAGE_PATH = "data/image/";
	const std::string SOUND_PATH = "data/sound/";
	const std::string EFFECT_PATH = "data/effect/";
	const std::string FONT_PATH = "data/font/";
}