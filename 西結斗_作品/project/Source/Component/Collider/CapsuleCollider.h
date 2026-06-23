#pragma once
#include "ColliderBase.h"
#include "../Transform/Transform.h"

class CapsuleCollider : public ColliderBase
{
public:
	CapsuleCollider();
	~CapsuleCollider();

	void Update() override;
	void Draw() override;

	void Start()override;

	/// <summary>
	/// 当たり判定の設定
	/// </summary>
	/// <param name="_info">当たり判定の情報</param>
	/// <param name="_transform">線の先端</param>
	/// <param name="_transform2">線の末端</param>
	void CapsuleSet(const CollsionInfo& _info, const Transform& _endTransform, const Transform& _startTransform,float _sphere,bool _mustMatrix,MATRIX* _matrix, std::function<void(const CollsionEventData&)> _funk);
	void CapsuleSet(const CollsionInfo& _info, const Transform& _endTransform, const Transform& _startTransform, float _sphere, bool _mustMatrix, MATRIX* _matrix, std::function<void(const CollsionEventData&)> _funk, std::string _tag);
	/// <summary>
	/// カプセルの終了のTransformを取得
	/// </summary>
	/// <returns></returns>
	Transform* CapselEndTransform() { return capselEndTransform; }
	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius; }
	/// <summary>
	/// 剣持対応
	/// </summary>
	/// <returns></returns>
	bool GetMultMatrix() { return matrixMult; }
	/// <summary>
	/// カプセルの終了の座標のセット
	/// </summary>
	/// <param name="_position">終了の座標</param>
	void SetCapsuleEndPos(const VECTOR3 _position) { baseEndTransform.position = _position; }
	/// <summary>
	/// カプセル終了の座標の加算
	/// </summary>
	/// <param name="_add"></param>
	void AddCapsuleEndPos(const VECTOR3 _add) { baseEndTransform.position += _add; }

private:
	Transform* capselEndTransform;
	float radius;
	bool matrixMult; //剣持に対応するかどうかで使う
	Transform baseStartTransform; //最初のカプセルの開始の位置の記録
	Transform baseEndTransform; //最初のカプセルの終了の位置の記録
	MATRIX* matrix; //剣持ち対応の手のマトリックス
};