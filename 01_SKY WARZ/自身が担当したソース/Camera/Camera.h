#pragma once
#include "../Component/Component.h"
#include "../Component/Shaker/Shaker.h"
#include "cameraInformation.h"
#include "../Common/ID/StateID.h"
#include "../Common/JsonReader.h"
#include "../Camera/CameraBase.h"
#include "../Component/Collider/CollsionInformation.h"

static const std::string PLAYER_POS_NAME		= "Player";
static const std::string PLAYER_FIRST_POS_NAME	= "First_Player";
static const std::string ENEMY_POS_NAME			= "Enemy";
static const std::string ENEMY_FIRST_POS_NAME	= "First_Enemy";
static const std::string WORLD_POS_NAME			= "World";

namespace CutSceneSpece {

	enum class EaseType {
		Linear,
		In,
		Out,
		InOut
	};

	enum CutStopChara
	{
		NONE = -1,
		PLAYER = 1 << 0,
		ALL_ENEMY = 1 << 1,
		ENEMY = 1 << 2,
		BOSS = 1 << 3,
	};

	struct CutSceneCameraPos {
		CutSceneCameraPos(){
			startPos = VZero;
			endPos = VZero;
			target = endPos;
		}
		VECTOR3 startPos;
		VECTOR3 endPos;
		VECTOR3 target;
	};

	struct CutScene {
		CutScene() {
			duration = 0.0f;
			ease = EaseType::Linear;
			camera = CutSceneCameraPos();
			followPosName.clear();
			followPosTarget.clear();;
			firstPosBaseName.clear();
		}
		float duration;
		EaseType ease;
		CutSceneCameraPos camera;
		std::string followPosName;
		std::string followPosTarget;
		std::string firstPosBaseName;
	};

	// CutSceneCameraからJSONに
	inline void to_json(nlohmann::json& j, const CutSceneCameraPos& c) {
		j = {
			{"startPos", c.startPos},
			{"endPos",   c.endPos},
			{"target",   c.target}
		};
	}

	inline void from_json(const nlohmann::json& j, CutSceneCameraPos& c) {
		j.at("startPos").get_to(c.startPos);
		j.at("endPos").get_to(c.endPos);
		j.at("target").get_to(c.target);
	}

	// EaseType を文字列に変換
	inline std::string EaseTypeToString(EaseType e) {
		switch (e) {
		case EaseType::Linear: return "Linear";
		case EaseType::In:     return "In";
		case EaseType::Out:    return "Out";
		case EaseType::InOut:  return "InOut";
		}
		return "Linear";
	}

	inline EaseType StringToEaseType(const std::string& s) {
		if (s == "Linear") return EaseType::Linear;
		if (s == "In")     return EaseType::In;
		if (s == "Out")    return EaseType::Out;
		if (s == "InOut")  return EaseType::InOut;
		return EaseType::Linear;
	}

	// CutSceneからJSON
	inline void to_json(nlohmann::json& j, const CutScene& c) {
		j = {
			{"duration", c.duration},
			{"ease", EaseTypeToString(c.ease)},
			{"camera", c.camera},
			{"followPos", c.followPosName},
			{"followTarget", c.followPosTarget},
			{"firstPosBaseName", c.firstPosBaseName},
		};
	}

	inline void from_json(const nlohmann::json& j, CutScene& c) {
		j.at("duration").get_to(c.duration);
		c.ease = StringToEaseType(j.at("ease").get<std::string>());
		j.at("camera").get_to(c.camera);
		j.at("followPos").get_to(c.followPosName);
		j.at("followTarget").get_to(c.followPosTarget);
		j.at("firstPosBaseName").get_to(c.firstPosBaseName);
	}
}


class PadInput;
class InputManager;
class CameraEditorGui;
class CutSceneBox;
class UIManager;

class Camera : public CameraBase
{
public:
	friend class FollowCamera;
	friend class JustAvoidCamera;
	friend class JustAvoidAttackCamera;
	friend class JustAvoidAttackHitCamera;
	friend class CameraStateBase;
	friend class FreeCamera;
	friend class CameraEditorGui;
	friend class PlayerSpecialAttackCamera;
	friend class CutSceneCamera;
	friend class PlayerHeavyAttackCamera;
	friend class RangedEnemyCamera;
	friend class RangedEnemyFinishCamera;
	
	Camera();
	~Camera();

	void Update()override;
	void Draw()override;

	void Start(BaseObject* _eObj);
	void ImguiDraw()override;
	/// <summary>
	/// Playerの情報をセットする関数
	/// </summary>
	/// <param name="_obj">プレイヤーのポインタ</param>
	void PlayerSet(BaseObject* _obj)override;

	/// <summary>
	/// ターゲットを設定する関数
	/// </summary>
	/// <param name="_obj"></param>
	void TargetSet(BaseObject* _obj);

	/// <summary>
	/// cameraComponentのtargetの値をtargetに代入する関数
	/// </summary>
	void TargetEnemySet();
	/// <summary>
	/// カメラのラープのセット
	/// </summary>
	/// <param name="_rape"></param>
	void CameraLeapSet(float _rape);
	/// <summary>
	/// カメラのstateをチェンジする関数
	/// </summary>
	/// <param name="_id">ステートID</param>
	void ChangeStateCamera(StateID::State_ID _id);
	/// <summary>
	/// 注視点を取得
	/// </summary>
	/// <returns>注視点</returns>
	VECTOR3 GetTarget()const {  return target; }
	/// <summary>
	/// 地面とカメラの当たり判定をするカメラの設定
	/// </summary>
	/// <param name="norm">法線</param>
	/// <param name="size">大きさ</param>
	/// <param name="groundPos">地面の座標</param>
	void CollsionPosHit(const VECTOR3& _norm, float _size, const VECTOR3& _groundPos);
	/// <summary>
	/// カメラの当たり判定で押し返しをするカメラの設定
	/// </summary>
	/// <param name="_data">当たり判定の情報</param>
	void PushCamera(const CollsionEventData& _data);
	/// <summary>
	/// 敵が攻撃した時にカメラを向く座標
	/// </summary>
	/// <param name="_targetTransform">敵の座標</param>
	/// <param name="_maxspeed">最大のスピード</param>
	void AttackEnemyFovChange(Transform* _targetTransform,float _maxspeed);

	/// <summary>
	/// targetTransformの中で視野に入っているかどうかの判定
	/// </summary>
	/// <param name="_targetTransform">ターゲットトランスフォーム</param>
	/// <param name="maxFov">最大の角度</param>
	/// <returns>視野内に入っているかどうか</returns>
	bool IsFovIn(const Transform& _targetTransform,float maxFov);
	/// <summary>
	/// カメラの回転させる関数
	/// </summary>
	/// <param name="_targetTransform">ターゲットトランスフォーム</param>
	/// <param name="_speed">スピード</param>
	void RotationChange(const Transform& _targetTransform, float _speed);
	/// <summary>
	/// カメラの回転をしているかどうかの判定
	/// </summary>
	/// <returns>カメラ回転をしていたらtrue</returns>
	bool CameraRotationMove();
	/// <summary>
	/// cameraEditorModeのWindowを出す
	/// </summary>
	void CameraEditor();

	/// <summary>
	/// CutSceneを開始するための関数
	/// ストップするため
	/// </summary>
	/// <param name="_name">ファイル名</param>
	/// <param name="_cutScene">黒いボックスを出すかどうか</param>
	void CutSceneChangeState(std::string _name,bool _cutScene);
	/// <summary>
	/// CutSceneを開始するための関数
	/// ストップするため
	/// </summary>
	/// <param name="_name">ファイル名</param>
	/// <param name="_cutScene">黒いボックスを出すかどうか</param>
	/// <param name="_space">cutScene中に止めるべき対象、ビット演算にて判定、CutStopChara型のenumを使用</param>
	void CutSceneChangeState(std::string _name, bool _cutScene, int _stop);
	
	/// <summary>
	/// カットシーン中かどうか
	/// </summary>
	/// <returns>カットシーン中ならtrue</returns>
	bool IsCutScene()const { return isCutScene; }
	/// <summary>
	/// sleepModeの時にその対象をsleepさせるか
	/// </summary>
	/// <param name="_stop">ストップさせる対象、ビット演算にて判定、CutStopChara型のenumを使用</param>
	/// <param name="_sleep">スリープモードにするときはtrue、解除するときはfalse</param>
	void SleepTargetSet(int _stop,bool _sleep);
	/// <summary>
	/// カットシーンで現在のcut数を取る
	/// </summary>
	/// <returns></returns>
	int GetCutNum() const { return cutSceneIndex; }
	/// <summary>
	/// カメラの視野角shakeのスタート
	/// </summary>
	/// <param name="_power">振動の強さ</param>
	/// <param name="_time">時間</param>
	void CameraPerspectiveShakeStart(float _power,float _time);
	/// <summary>
	/// この関数を呼ぶとshakeの終了
	/// </summary>
	void CameraPerspectiveShakeFinish();

private:

	float timeTest;
	int cutSceneIndex;
	bool normalCamera;
	int targetChangeButton;
	bool rockOn;
	float beforePos;
	float counter;
	bool hit;
	float rokPos;
	float moveAmout;

	//当たり判定で判定に使用する変数
	VECTOR3 hitNormal;
	float  hitDist;
	VECTOR3 hitPos;
	//------------------------------
	
	EnemyAttackChangeCameraDirection direction; //敵が攻撃をしてきたときに回転する方向の指定
	Transform* targetEnemyTransform;
	
	float moveTimer;
	const float MOVE_TIMER_MAX = 0.4f;
	float angleMaxSpeed;

	CameraEditorGui* editor; //カットシーンのWindowのクラス
	std::vector<CutSceneSpece::CutScene> cutSceneData; //カットシーンのデータが記入
	bool isCutScene;
	
	int cutStopChara; //カットシーンの時に対象のオブジェクトを止めるかどうかの判定をビット演算にて行う
	CutSceneBox* cutSceneBox; //カットシーンの時に上下に黒いボックスを付けるかどうかのクラス
	bool cutSceneBoxDraw;
	UIManager* uiManager;

	//視野角のShakeのために必要な変数
	float shakePower;
	float shakeTime;
	float keepFov; //視野角のShakeのために行う
};