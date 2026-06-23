#pragma once
#include ".././../Library/GameObject.h"
#include <list>
#include "../Camera/CameraInformation.h"

class Object3D;
class BaseObject;
class Camera;
class EnemyBase;
class Transform;
class Boss;
class GameControler;

class EnemyManager : public GameObject
{
public:
	EnemyManager();
	~EnemyManager();
	void Update() override;
	void Draw() override;

	void DebugDrawCamera(Camera* camera);
	/// <summary>
	/// 敵の生成
	/// </summary>
	void CreateEnemy();
	/// <summary>
	/// プレイヤーのポインタのセット
	/// </summary>
	void PlayerObjPointer();
	
	/// <summary>
	/// プレイヤーを探す
	/// </summary>
	void FindPlayer();
	/// <summary>
	/// Enemyのリストを生成
	/// </summary>
	/// <param name="_enemy"> Enemyのクラスの要素</param>
	/// <param name="_obj">Enemyのオブジェクト</param>
	void AddList(EnemyBase* _enemy, BaseObject* _obj);
	/// <summary>
	/// Enemyのリストの登録解除
	/// </summary>
	/// <param name="_enemy"> Enemyのクラスの要素</param>
	/// <param name="_obj">Enemyのオブジェクト</param>
	void RemoveList(EnemyBase* _enemy, BaseObject* _obj);
	/// <summary>
	/// カメラのロックオン対象の変更を行うクラス
	/// </summary>
	/// <param name="camera">カメラのポインタ</param>
	/// <param name="_right">右側にいるか左側にいるかの判定</param>
	/// <returns>ロックチェンジが出来たかどうか</returns>
	bool ChangeCameraRockOn(Camera* camera, bool _right);
	/// <summary>
	/// カメラのロックオン対象の変更を行うクラス
	/// </summary>
	/// <param name="camera">カメラのポインタ</param>
	/// <param name="_right">右側にいるか左側にいるかの判定</param>
	/// <param name="_min">評価値の最小をとるか最大を取るか</param>
	/// <returns>ロックチェンジが出来たかどうか</returns>
	bool ChangeCameraRockOn(Camera* camera, bool _right, bool _min);
	/// <summary>
	/// カメラのロックオン対象の変更を行うクラス
	/// </summary>
	/// <param name="camera">カメラのポインタ</param>
	/// <param name="_right">右側にいるか左側にいるかの判定</param>
	/// <param name="_min">評価値の最小をとるか最大を取るか</param>
	/// <param name="_debugMode">評価値をDebug表示させるかさせないか</param>
	/// <returns></returns>
	bool ChangeCameraRockOn(Camera* camera,bool _right, bool _min,bool _debugMode);
	/// <summary>
	/// カメラロックオンのキャンセル
	/// </summary>
	/// <param name="camera">カメラのポインタ</param>
	/// <returns>キャンセル出来たらtrue</returns>
	bool TargetCancel(Camera* camera);
	/// <summary>
	/// ジャスト回避した時のターゲットの変更
	/// </summary>
	/// <param name="_obj">攻撃をした敵のオブジェクト</param>
	void JustAvoidTargetChange(BaseObject* _obj);
	/// <summary>
	/// 近くの敵のBaseObjectを取得
	/// </summary>
	/// <returns></returns>
	EnemyBase* PlayerNearEnemy();
	/// <summary>
	/// カメラの近くにいたら敵を半透明にさせる関数
	/// </summary>
	/// <param name="camPos">カメラのポジション</param>
	void NearEnemyAlpha(const VECTOR3& camPos);
	/// <summary>
	/// 最も近い敵のTransformを取得
	/// </summary>
	/// <param name="_pos">対象となる座標</param>
	/// <returns>近い敵のTransform</returns>
	Transform NearEnemyPos(const VECTOR3& _pos);
	/// <summary>
	/// 最も近い敵のTransformを取得
	/// </summary>
	/// <param name="_pos">対象となる座標</param>
	/// <param name="_angle">視野の角度</param>
	/// <returns>Transform</returns>
	Transform NearFovEnemyPos(Transform& _transform,float _angle);
	/// <summary>
	/// カメラロックオンの開始（一番近くの敵を参照）
	/// </summary>
	/// <param name="_enemy">敵のポインタ</param>
	/// <param name="_camera">カメラのポインタ</param>
	void SetCameraRockOnObject(EnemyBase* _enemy, Camera* _camera);
	/// <summary>
	/// カメラロックオンの開始（一番近くの敵を参照）
	/// </summary>
	/// <param name="_camera">カメラのポインタ</param>
	/// /// <returns>ロックオン成功したかどうか</returns>
	bool CameraRockOnStart(Camera* _camera);
	/// <summary>
	/// ボスが攻撃をしてきたときに使う
	/// </summary>
	/// <param name="camera">カメラのポインタ</param>
	/// <param name="_targetTransform">攻撃してきた敵のTransform</param>
	/// <returns>カメラの回転の方向</returns>
	EnemyAttackChangeCameraDirection BossAttackCamera(Camera* camera,const Transform& _targetTransform);
	/// <summary>
	/// Enemyすべてがカメラが入っているかどうかを調べる
	/// </summary>
	/// <returns></returns>
	bool CameraInEnemy();
	/// <summary>
	/// 敵が必殺技に当てられる状態になったらこの関数を呼ぶ
	/// </summary>
	void CanPlayerSpecialHit();
	/// <summary>
	/// すべての敵をスリープモードにする
	/// </summary>
	/// <param name="_sleep"></param>
	void SleepAllEnemy(bool _sleep);
	/// <summary>
	/// プレイヤーの視野内にどれだけ敵がいるかを判定する
	/// </summary>
	/// <param name="_pTransform">プレイヤーのTransform</param>
	/// <param name="_angle">視野角度</param>
	/// <returns>敵の人数</returns>
	int PlayerFovEnemyNum(Transform* _pTransform, float _angle);

	bool ObjectIsEnemy(BaseObject* _base);

	int GetEnemySize() { return (int)enemy.size(); }

private:
	
	std::list<BaseObject*> enemy;//敵のObjectのリスト
	Object3D* player;
	std::list<EnemyBase*> chara; //EnemyBaseコンポーネントのリスト
	BaseObject* cameraTargetObj;
	//std::list<Boss> bossList;
	GameControler* gameContorler;
	void GameSceneChangeState();

	

};