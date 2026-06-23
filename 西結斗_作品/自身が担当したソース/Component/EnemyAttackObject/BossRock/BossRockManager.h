#pragma once
#include "../../../../Library/GameObject.h"
#include "../../../Enemy/EnemyState/Attack/EnemyAttackBase.h"
#include "../../../Common/JsonReader.h"

class BossRockBase;
class Boss;
class BaseObject;

class BossRockManager : public GameObject
{
public:
	BossRockManager();
	BossRockManager(Boss* _boss);
	~BossRockManager();

	void Update()override;
	void Draw() override;
	/// <summary>
	/// 空のオブジェクトを生成
	/// </summary>
	/// <param name="_data">攻撃情報に入っているデータの配列</param>
	void CreateThrowEmptyObject(const std::vector<EnemyAttackBase::ThrowObjectAttackData>& _data);
	/// <summary>
	/// 空のオブジェクトから描画出来るようにする
	/// </summary>
	/// <param name="_rock">空のオブジェクトのポインタ</param>
	/// <param name="_index">インデックス</param>
	/// <param name="_total">出てくるトータルの数</param>
	/// <param name="_rotateAngle">回転角</param>
	void AppearThrowObject(BossRockBase* _rock, int _index, int _total, float _rotateAngle);
	/// <summary>
	/// 手で投げるとき以外に使う
	/// </summary>
	/// <param name="_data">攻撃情報に入っているデータの配列</param>
	/// <param name="_index">インデックス</param>
	/// <param name="_total">出てくるトータルの数</param>
	/// <param name="_rotateAngle">回転角</param>
	void CreateThrowObject(const std::vector<EnemyAttackBase::ThrowObjectAttackData>& _data, int _index, int _total, float _rotateAngle);
	/// <summary>
	/// 当たり判定のサイズを取得
	/// </summary>
	/// <param name="_data">投擲物のデータ</param>
	/// <returns></returns>
	VECTOR3 GetPushCollSize(const EnemyAttackBase::ThrowObjectAttackData& _data);

	/// <summary>
	/// リストに追加する関数、投擲物クラスから減速呼んでいるので特にいじる必要なし
	/// </summary>
	/// <param name="_obj"></param>
	void PushList(BossRockBase* _obj);
	/// <summary>
	/// リストから削除する関数、投擲物クラスから減速呼んでいるので特にいじる必要なし
	/// </summary>
	/// <param name="_obj"></param>
	void RemoveList(BossRockBase* _obj);

	/// <summary>
	/// 投擲物のリストのサイズを取得
	/// </summary>
	/// <returns></returns>
	size_t GetSize() { return rocks.size(); }

	/// <summary>
	/// カメラを震わせるときに使う関数
	/// </summary>
	void ShakeCamera();

	/// <summary>
	/// 投擲物を落とすかどうかの判定
	/// </summary>
	void DropRockStart();

	//指定されたポジションから一番近い岩のポジションを返す
	VECTOR3 GetRockPos(const VECTOR3& _pos);
	/// <summary>
	/// すべての投擲物の位置を返す
	/// </summary>
	/// <returns>投擲物のPosition</returns>
	std::list<VECTOR3> GetAllRockPos();

	struct BossThrowObjectData
	{
		std::string id;
		std::string modelName;
		int modelData;
		Transform modelTransform;
		Transform pushTransform;
		bool isEffect;

		BossThrowObjectData() {
			id = "";
			modelName = "";
			modelData = -1;
			modelTransform = Transform();
			pushTransform = Transform();
			isEffect = false;
		}
	};
	/// <summary>
	/// 投擲物のデータを渡す
	/// </summary>
	/// <returns>投擲物のデータの配列</returns>
	std::map<std::string, BossThrowObjectData> GetThrowObjectsData() { return throwObjectsData; }

	/// <summary>
	/// 投擲物データの追加
	/// </summary>
	/// <param name="_data">新たに入れる投擲物データ</param>
	void AddJsonData(BossThrowObjectData& _data);
	/// <summary>
	/// 今まで使われていたものを上書きするときに使う関数
	/// </summary>
	/// <param name="_data">新しいデータ</param>
	/// <param name="_oldKey">今までの名前</param>
	/// <param name="_newKey">新しい名前</param>
	void ChangeJsonData(const BossThrowObjectData& _data, const std::string& _oldKey, const std::string& _newKey);
	/// <summary>
	/// 岩が出るタイミングを管理
	/// </summary>
	/// <param name="_data">ボスの攻撃のデータ</param>
	/// <param name="_animNum">アニメーションの現在の再生時間</param>
	void RockContorler(const EnemyAttackBase::BossAttackParam& _data,float _animFrame);
	/// <summary>
	/// 攻撃が終了した時に消すオブジェクトは消す
	/// </summary>
	void AttackFinishDelete();
	/// <summary>
	/// 攻撃が終了した時にまだ空のオブジェクトなら削除
	/// </summary>
	void DeleteEmptyObject();
	/// <summary>
	/// ボスが死んだときにオブジェクトが生きているなら消す
	/// </summary>
	void BossDieDeleteObject(float _hp);

private:

	std::map<std::string,BossThrowObjectData> throwObjectsData;

	std::list<BossRockBase*> rocks;
	Boss* boss;

	int modelNum;
	int effectNum;

	bool IsFreePos(const VECTOR3& _pos, float _minDist);
	void SetRockComponent(Object3D* _base,const VECTOR3& _gravity, const VECTOR3& _fir,const EnemyAttackBase::ThrowObjectAttackData& _data);

	void LoadEffect(BossThrowObjectData& _data);
	void LoadModel(BossThrowObjectData& _data);
};

inline void to_json(JSON& j, const BossRockManager::BossThrowObjectData& data)
{
	j = JSON{
		{"id", data.id},
		{"modelName", data.modelName},
		{"modelData", data.modelData},
		{"modelTransform", data.modelTransform},
		{"pushTransform", data.pushTransform},
		{"isEffect", data.isEffect}
	};
}

// from_json
inline void from_json(const JSON& j, BossRockManager::BossThrowObjectData& data)
{
	j.at("id").get_to(data.id);
	j.at("modelName").get_to(data.modelName);
	j.at("modelData").get_to(data.modelData);
	j.at("modelTransform").get_to(data.modelTransform);
	j.at("pushTransform").get_to(data.pushTransform);
	if(j.contains("isEffect")) j.at("isEffect").get_to(data.isEffect);

}