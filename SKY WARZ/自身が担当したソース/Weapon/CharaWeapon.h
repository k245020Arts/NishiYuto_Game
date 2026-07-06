#pragma once
#include "../Component/Component.h"
#include "../Component/Animator/Animator.h"
#include "../Common/ID/ID.h"

class CharaWeapon : public Component
{
public:
	CharaWeapon();
	~CharaWeapon();

	void Update()override;
	void Draw() override;
	/// <summary>
	/// 剣を持たせる
	/// </summary>
	/// <param name="_obj">剣を持たせるオブジェクト</param>
	/// <param name="_frame">どのフレームの位置に持たせるか</param>
	/// <param name="_baseModelName">剣を持たせるオブジェクトのモデルデータ</param>
	/// <param name="_weaponHandle">剣のハンドル</param>
	void ObjectPointer(BaseObject* _obj,int _frame, ID::IDType _baseModelName,int _weaponHandle);
	/// <summary>
	/// この関数でfalseにすると剣をもたない、ボスとかは剣を持っていないのでfalseにしている
	/// </summary>
	/// <param name="_parent">tureにすると剣を持つように</param>
	void SetParent(bool _parent);
	void ImguiDraw()override;
	/// <summary>
	/// 剣の軌跡を生成
	/// </summary>
	/// <param name="_nPos">剣の末端</param>
	/// <param name="_fPos">剣先</param>
	/// <param name="_r">赤成分</param>
	/// <param name="_g">緑成分</param>
	/// <param name="_b">青成分</param>
	/// <param name="_a">半透明成分</param>
	/// <param name="index">剣を持っている手のindex</param>
	/// <param name="_time">何秒表示するか</param>
	void CreateSwordEffect(const VECTOR3& _nPos, const VECTOR3& _fPos, float _r, float _g, float _b, float _a, int index, float _time);

	/// <summary>
	/// 現在のキャラのオブジェクトのポインタの取得
	/// </summary>
	/// <returns>現在のキャラのオブジェクト</returns>
	BaseObject* GetChara() { return chara; }
	/// <summary>
	/// 現在のキャラの剣のモデル
	/// </summary>
	/// <returns></returns>
	int WeaponModel() { return weaponHandle; }
	/// <summary>
	/// 現在のキャラのオブジェクトのモデルデータの取得
	/// </summary>
	/// <returns></returns>
	int CharaModel() { return modelHandle; }
	/// <summary>
	/// 剣の行列を取得
	/// </summary>
	/// <returns></returns>
	MATRIX GetWeaponMatrix() { return matrix; }
	/// <summary>
	/// 剣の軌跡のイメージをセット
	/// </summary>
	/// <param name="_model"></param>
	void SetImage(int _model);

	MATRIX* GetMatrixPtr() { return &matrix; }

private:
	BaseObject* chara;
	Animator* anim;
	int frame;
	int modelHandle;
	int weaponHandle;

	bool weapon;
	MATRIX matrix;
	bool parent;
	Transform* transform;
};
