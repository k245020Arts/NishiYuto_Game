#pragma once
#include "../CharaBase/CharaBase.h"
#include "../Player/playerInformation.h"
#include "../Component/Transform/Transform.h"
#include "../Common/ID/ID.h"
#include "../Common/ID/EffectID.h"
#include "../Common/ID/SoundID.h"

class TitleCamera;
class TitleScene;

class TitlePlayer: public CharaBase
{
public:

	friend class TitleControl;
	friend class TitlePlayerIdol;
	friend class TitlePlayerMove;

	TitlePlayer();
	~TitlePlayer();


	void Update()override;
	void Draw()override;
	void Start(Object3D* _obj);

	/// <summary>
	/// 移動の関数
	/// </summary>
	/// <param name="_speed"></param>
	/// <param name="_speedMax"></param>
	void Move(float _speed, float _speedMax);

	/// <summary>
	/// プレイヤーの回転を滑らかに行うために行っている関数
	/// 上は自由指定ver、下はデフォルト値ver
	/// </summary>
	/// <param name="_angle"></param>
	/// <param name="_speed"></param>
	void RotationChange(const VECTOR3& _angle, float _speed);
	void RotationChange(const VECTOR3& _angle);

private:
	PlayerInformation::CharaComponent playerCom;
	Transform* playerTransform;
	TitleCamera* camera;
	TitleScene* title;
	VECTOR3 walkAngle;
};
