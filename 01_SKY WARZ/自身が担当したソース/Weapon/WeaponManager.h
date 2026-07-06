#pragma once
#include "../../Library/GameObject.h"

class Object3D;
class CharaWeapon;

class WeaponManager : public GameObject
{
public:
	WeaponManager();
	~WeaponManager();
	void Update()override;
	void Draw()override;
	/// <summary>
	/// Player‚Ì•Ší‚Ì¶¬
	/// </summary>
	void CreatePlayerWeapon();
	/// <summary>
	/// “G‚Ì•Ší‚Ì¶¬
	/// </summary>
	void CreateEnemyWeapon();

	/// <summary>
	/// Œ•‚Ì‹OÕ‚Ì¶¬
	/// </summary>
	/// <param name="_nearPos">Œ•‚Ì‹OÕ‚Ì––’[</param>
	/// <param name="_farPos">Œ•‚Ì‹OÕ‚Ìæ’[</param>
	/// <param name="_rgb">‹OÕ‚ÌF</param>
	/// <param name="_boneIndex">œ</param>
	/// <param name="_time">ŠÔ</param>
	void CreateTrailPlayer(VECTOR3 _nPos, VECTOR3 _fPos, float _r, float _g, float _b, float _a, int index, float _time);
	/// <summary>
	/// Œ•‚Ì‹OÕ‚Ì¶¬
	/// </summary>
	/// <param name="_nearPos">Œ•‚Ì‹OÕ‚Ì––’[</param>
	/// <param name="_farPos">Œ•‚Ì‹OÕ‚Ìæ’[</param>
	/// <param name="_rgb">‹OÕ‚ÌF</param>
	/// <param name="_boneIndex">œ</param>
	/// <param name="_time">ŠÔ</param>
	void CreateTrailEnemy(VECTOR3 _nPos, VECTOR3 _fPos, float _r, float _g, float _b, float _a, int index, float _time);

private:
	Object3D* pObj;
	Object3D* eObj;

	CharaWeapon* pWeapon;
	CharaWeapon* eWeapon;
};
