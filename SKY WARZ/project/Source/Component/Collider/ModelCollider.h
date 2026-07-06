#pragma once
#include "colliderBase.h"
#include "../Transform/Transform.h"

class ModelCollider : public ColliderBase
{
public:
	ModelCollider();
	~ModelCollider();

	void Update()override;
	void Draw()override;

	/// <summary>
	/// メッシュ当たり判定の初期化
	/// </summary>
	/// <param name="_info">当たり判定の情報</param>
	/// <param name="_transform">Transformの取得</param>
	/// <param name="_modelHandle">当たり判定に使用するモデルデーター</param>
	void ModelColliderSet(const CollsionInfo& _info, const Transform& _transform , int _modelHandle, std::function<void(const CollsionEventData&)> _funk);
	void ModelColliderSet(const CollsionInfo& _info, const Transform& _transform , int _modelHandle, std::function<void(const CollsionEventData&)> _funk,std::string _tag);

	/// <summary>
	/// 現在使われているモデルデータ
	/// </summary>
	/// <returns>当たり判定に使用するモデルデーター</returns>
	int GetModel() { return modelHandle; }

private:
	int modelHandle; //当たり判定に使うモデル
};