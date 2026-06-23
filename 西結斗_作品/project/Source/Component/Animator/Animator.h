#pragma once
#include <map>
#include "../../../Library/myDxLib.h"
#include <string>
#include "../Component.h"
#include "../../Common/ID/ID.h"

/// <summary>
/// アニメーションを再生する
/// </summary>
class Animator : public Component
{
public:
	/// <summary>
	/// アニメーションクラスを作る
	/// </summary>
	/// <param name="model">アニメーションを付けるモデル</param>
	Animator();
	~Animator();

	void BaseModelSet(int _model, int _root);
	void BaseModelSet(int _model, std::string _rootName);

	/// <summary>
	/// アニメーションの再生をする
	/// </summary>
	void Update();

	/// <summary>
	/// アニメーションファイルを追加する
	/// </summary>
	/// <param name="id">アニメーション番号</param>
	/// <param name="filename">アニメーションファイル</param>
	/// <param name="loop">ループする場合はtrue</param>
	/// <param name="speed">再生倍率（基本は1.0）</param>
	void AddFile(ID::IDType id, std::string filename, bool loop,float speed = 1.0f,float _eventStart = -1.0f,float _eventFinish = -1.0f);

	/// <summary>
	/// 最大タイムをデフォルトタイムから変えたいときに設定する。
	/// </summary>
	/// <param name="id"></param>
	/// <param name="_maxTime"></param>
	void SetMaxFrame(ID::IDType id,float _maxTime);

	/// <summary>
	/// アニメーションを再生する
	/// </summary>
	/// <param name="id">再生するアニメーションの番号</param>
	/// <param name="margeTime">前のアニメーションとのブレンド時間</param>
	void Play(ID::IDType id, float margeTime = 0.2f);

	/// <summary>
	/// アニメーションの再生速度を変更する
	/// </summary>
	/// <param name="speed">再生速度（倍率）</param>
	void SetPlaySpeed(float speed);

	/// <summary>
	/// 今のアニメーションの再生速度を返す
	/// </summary>
	/// <returns>再生速度（倍率）</returns>
	float GetPlaySpeed();

	/// <summary>
	/// アニメーションが終ったか調べる
	/// ループしないアニメーションが、最後のフレームまで再生したものを、終りとする
	/// </summary>
	/// <returns>終っていればtrue</returns>
	bool IsFinish();

	/// <summary>
	/// 現在再生しているアニメーション番号
	/// </summary>
	/// <returns>アニメーション番号</returns>
	std::string GetCurrentID();

	/// <summary>
	/// 現在の再生フレーム
	/// </summary>
	/// <returns>再生フレーム</returns>
	float GetCurrentFrame()const;
	/// <summary>
	/// １フレーム前の再生フレーム
	/// </summary>
	/// <returns>再生フレーム</returns>
	float GetCurrentBeforeFrame()const;

	/// <summary>
	/// 現在再生中アニメーションの最大フレームを返す
	/// </summary>
	/// <returns>最大フレーム</returns>
	float GetMaxFrame();

	/// ボーン番号を引数に入れるとそのローカルポジションが次のアップデートから得られるようになる。
	void SetBoneFrame(int _index);

	/// アニメーションでもし移動する場合があった時にそのボーンの位置にポジションを合わせなければいけないので
	/// その時にこの関数を使う。
	/// これの返り値の座標引く動かしたい座標をすればその位置が帰ってくる。
	VECTOR3 BoneMovePositionAdd();
	VECTOR3 BoneMovePositionAdd(VECTOR3 _position);
	VECTOR3 BoneNowPosition();

	/// <summary>
	/// アニメーションイベントを生成しているかどうか
	/// </summary>
	/// <returns>アニメーションイベントが発動しているならtrue</returns>
	bool AnimEventCan() { return animEventCan; }
	/// <summary>
	/// アニメーションイベントを発動をキャンセルする
	/// </summary>
	void AnimEventReset() { animEventCan = false; }
	/// <summary>
	/// アニメーションイベントの開始時間取得
	/// </summary>
	/// <param name="id">取得したいID</param>
	/// <returns>アニメーションイベントの開始時間</returns>
	float EventStartTime(ID::IDType id) { return fileInfos[ID::GetID(id)].eventStartTime; }
	float EventStartTime(std::string id) { return fileInfos[id].eventStartTime; }
	/// <summary>
	/// アニメーションイベントの終了時間取得
	/// </summary>
	/// <param name="id">取得したいID</param>
	/// <returns>アニメーションイベントの終了時間</returns>
	float EventFinishTime(ID::IDType id) { return fileInfos[ID::GetID(id)].eventFinishTime; }
	float EventFinishTime(std::string id) { return fileInfos[id].eventFinishTime; }

	/// <summary>
	/// アニメーションのフレームをセットする
	/// </summary>
	/// <param name="_frame"></param>
	void SetFrame(float _frame);

	/// <summary>
	/// 基準となるモデルを取得
	/// </summary>
	/// <returns>基準となるモデル</returns>
	int GetBaseModel() const ;
	/// <summary>
	/// 今のアタッチしているアニメーションの取得
	/// </summary>
	/// <returns>アタッチしてるアニメーション</returns>
	int GetCurrentAttachID() const ;
	/// <summary>
	/// アニメーションブレンドが終了しているかどうか
	/// </summary>
	/// <returns>終了してたらtrue</returns>
	bool BlendFinish() const ;

	void ImguiDraw()override;

	struct AnimFileInfo {
		int hModel; //アニメーションモデル
		bool loop; //ループ再生をするか
		float maxFrame; //最大フレーム
		float playSpeed; //プレイスピード
		float eventStartTime; //アニメーションイベントのスタートタイム
		float eventFinishTime; //アニメーションイベントの終了タイム
		std::string fileName; //アニメーションモデルのファイルネーム
		std::string id; //アニメーションID

		AnimFileInfo() : hModel(-1), loop(false), maxFrame(1.0f), playSpeed(1.0f), eventFinishTime(-1.0f), eventStartTime(-1.0f), fileName(""), id("") {}
	};

	/// <summary>
	/// アニメーションのすべてのデータをSaveする
	/// </summary>
	void AnimDataSaveAll();
	/// <summary>
	/// アニメーションの一部データをSaveする
	/// </summary>
	/// <param name="_fileName">アニメーションjsonファイル名</param>
	void AnimDataReSave(std::string _fileName);
	/// <summary>
	/// アニメーションのデータをロードするときに使用
	/// </summary>
	/// <param name="_charaID">キャラクターのID</param>
	/// <param name="_typeID">キャラクターの中の種類のID</param>
	void AnimDataLoad(const std::string& _charaID,const std::string _typeID);
	//void AnimDataLoad(const std::string& _path);
	
	/// <summary>
	/// アニメーションデータの取得
	/// </summary>
	/// <param name="_fileName">指定したファイルネーム</param>
	/// <returns></returns>
	AnimFileInfo GetSelectFileInfo(std::string _fileName);
	/// <summary>
	/// アニメーションデータのセット
	/// </summary>
	/// <param name="_animFileInfo"></param>
	void SetSelectFileInfo(AnimFileInfo _animFileInfo);

private:
	int baseModel;
	int rootNum;

	std::map<std::string, AnimFileInfo> fileInfos;

	struct PlayInfo {
		std::string fileID; //ID
		int attachID; //アタッチID
		float frame; //現在のフレーム
		float beforeFrame; //１フレーム前のフレーム
		int boneIndex; //フレームのインデックス

		PlayInfo() : fileID("NONE"), attachID(-1), frame(0.0f),beforeFrame(0.0f), boneIndex(-1){}
	};
	PlayInfo current; // 今のアニメ
	PlayInfo before; // 前のアニメ

	float blendTime; // ブレンドのタイマー
	float blendTimeMax; // ブレンドのタイマー最大値

	bool finished;
	float playSpeed;

	VECTOR3 currentPosition;
	VECTOR3 nowPosition;
	VECTOR3 subPosition;

	bool animEventCan;

	
};

inline void to_json(nlohmann::json& j, const Animator::AnimFileInfo& f)
{
	j = {
		{"loop", f.loop},
		{"maxFrame", f.maxFrame},
		{"playSpeed", f.playSpeed},
		{"eventStartTime", f.eventStartTime},
		{"eventFinishTime", f.eventFinishTime},
		{"fileName", f.fileName},
		{"id", f.id}
	};
}

inline void from_json(const nlohmann::json& j, Animator::AnimFileInfo& f)
{
	f.loop = j.value("loop", false);
	f.maxFrame = j.value("maxFrame", 1.0f);
	f.playSpeed = j.value("playSpeed", 1.0f);
	f.eventStartTime = j.value("eventStartTime", -1.0f);
	f.eventFinishTime = j.value("eventFinishTime", -1.0f);
	f.fileName = j.value("fileName", "");
	f.id = j.value("id", "");

	// 実行時だけ
	f.hModel = -1;
}