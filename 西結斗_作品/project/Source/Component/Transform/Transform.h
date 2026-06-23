#pragma once
#include "../../../Library/myDxLib.h"
#include "../Component.h"

class Transform : public Component
{
public:
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;
	VECTOR3 Forward() { return VECTOR3(0, 0, 1) * rotMatrix; }
	Transform() {
		position = VZero;
		rotation = VZero;
		scale = VECTOR3(1, 1, 1);
		parent = nullptr;
		rotMatrix = MGetIdent();
		positionMatrix = MGetIdent();
		scaleMatrix = MGetIdent();
		debugId = 11;
		tag = Function::GetClassNameC<Transform>();
	}
	Transform(const VECTOR3& _pos, const VECTOR3& _rotate, const VECTOR3& _scale) {
		position = _pos;
		rotation = _rotate;
		scale = _scale;
		parent = nullptr;
		rotMatrix = MGetIdent();
		positionMatrix = MGetIdent();
		scaleMatrix = MGetIdent();
		debugId = 11;
		tag = Function::GetClassNameC<Transform>();
	}
	~Transform(){
		if (parent != nullptr) {
			parent->RemoveChild(this);
		}
		for (auto itr = child.begin(); itr != child.end(); itr++) {
			(*itr)->RemoveParent(this);
		}
	}

	MATRIX GetPositionMatrix()const ;
	MATRIX GetRotationMatrix() ;
	MATRIX GetScaleMatrix()const ;

	MATRIX GetMatrix() ;

	//子が親を追加したいとき
	void SetParent(Transform* _pare);
	//親が子を追加したいとき
	void AddChild(Transform* _child) { 
		if (_child->parent) {
			_child->parent->RemoveChild(_child); // 元の親から外す
		}
		_child->parent = this;
		child.emplace_back(_child);
	}
	//子のポジションのワールド座標を取り出す関数。
	Transform WorldTransform();
	/// <summary>
	/// ワールドTransformをセットする
	/// </summary>
	/// <param name="_transform">ワールドTransform</param>
	void SetWorld(const Transform& _transform);
	void ImguiDraw()override;
	/// <summary>
	/// 子供を取り除く
	/// </summary>
	/// <param name="_child"></param>
	void RemoveChild(Transform* _child);
	/// <summary>
	/// ローテーションマトリックスをセットする
	/// </summary>
	/// <param name="_matrix">回転行列</param>
	void SetRotationMatrix(const MATRIX& _matrix) { rotMatrix = _matrix; }

	void RemoveParent(Transform* transform);

private:

	MATRIX rotMatrix;
	MATRIX positionMatrix;
	MATRIX scaleMatrix;

	Transform* parent;
	std::list<Transform*> child;

	void Child(Transform* _child){ child.emplace_back(_child); }
};

inline void from_json(const nlohmann::json& j, Transform& t)
{
	j.at("position").get_to(t.position);
	j.at("rotation").get_to(t.rotation);
	j.at("scale").get_to(t.scale);
}

inline void to_json(nlohmann::json& j, const Transform& t)
{
	j = {
		{"position", t.position},
		{"rotation", t.rotation},
		{"scale", t.scale}
	};
}