#include "CollsionInformation.h"
#include <array>
#include "../../Common/Function.h"
#include "../Transform/Transform.h"
#include "../Physics/Physics.h"
#include <algorithm>


namespace {
    std::array<bool, CollsionInformation::COLLSION_TAG_MAX> pairTag;

    void SetPair(CollsionInformation::Tag tag1, CollsionInformation::Tag tag2) {
        pairTag[Function::EnumTag(tag1, tag2, CollsionInformation::TAG_MAX)] = true;
    }

}

void CollsionInformation::InitSetPair()
{
    //SetPair(PLAYER, ENEMY);
    //SetPair(PLAYER, BOSS);
    SetPair(PLAYER, BOSS_PUSH);
    SetPair(FLOOR, SHADOW);
    SetPair(PLAYER, B_E_ATTACK);
    SetPair(PLAYER, E_ATTACK);
    SetPair(PLAYER, BOSS_ROCK_ATTACK);
    SetPair(PLAYER, BOSS_ROCK);
    //SetPair(BOSS, BOSS_ROCK);
    SetPair(PLAYER, ROCK_BLAST_DAMAGE);
    //SetPair(BOSS, ROCK_BLAST_DAMAGE);
    SetPair(BOSS, BOSS_ROCK_DAMAGE);
    SetPair(BOSS_ROCK_RUSH, BOSS_RUSH);
    SetPair(PLAYER, WALL);
    //SetPair(CAMERA, FLOOR);
    SetPair(ENEMY, P_ATTACK);
    SetPair(ENEMY, WALL);
    SetPair(PLAYER, B_ATTACK);
    SetPair(BOSS, P_ATTACK);
    SetPair(BOSS_ROCK_PLAYER_ATTACK, P_ATTACK);
    SetPair(BOSS, WALL);
    SetPair(FLOOR, P_FLOOR);
    SetPair(FLOOR, E_FLOOR);
    SetPair(FLOOR, C_FLOOR);
    SetPair(FLOOR, B_FLOOR);
    SetPair(FLOOR, BOSS_ROCK_F);
    SetPair(P_SPECIAL_ATTACK, BOSS);
    SetPair(P_SPECIAL_ATTACK, ENEMY);
    SetPair(BOSS_ROCK_UI, FLOOR_AABB);
    SetPair(PLAYER, JUST_AVOID);
    SetPair(PLAYER, JUST_AVOID_ENEMY);
    SetPair(PLAYER, JUST_AVOID_BOSS);
    SetPair(PLAYER, THROW_OBJECT_GROUND);
    SetPair(PLAYER, THROW_OBJECT_GROUND_ONE_HIT);
    SetPair(PLAYER, THROW_OBJECT_GROUND_NO_DAMAGE_REACTION);
}

bool CollsionInformation::IsCollPair(Tag tag1, Tag tag2)
{
    return  pairTag[Function::EnumTag(tag1, tag2, CollsionInformation::TAG_MAX)];
}

Pushback::Pushback()
{
    Clear();
}

Pushback::~Pushback()
{
    Clear();
}

void Pushback::Clear() {
    pushes.clear();
}

void Pushback::AddPush(const VECTOR3& _normal, float  _penetration, CollsionInformation::Shape _shape, const VECTOR3& _targetPos)
{
    if (_penetration > 0.0f) {
        PushInfo i(_normal, _penetration, _shape, _targetPos);

        pushes.emplace_back(i);
    }
}

VECTOR3 Pushback::ResultPushback(float  _maxLength, VECTOR3 _pos) {
    if (pushes.empty()) {
        return VZero;
    }
 
    VECTOR3 totalPush = VECTOR3(0, 0, 0);

    for (const auto& p : pushes) {
        VECTOR3 diff = _pos - p.targetPos;
        float penetration = VDot(p.normal,diff);
        VECTOR3 newPush = p.normal * p.penetration;

		totalPush += newPush;
		VECTOR3 pushVecNorm = totalPush.Normalize();
        //押し返す量を取得
		float pushIn = VDot(pushVecNorm, newPush);
        //丸の形状ならyの押し返しを消す
        if (p.shape == CollsionInformation::SPHERE) {
            pushVecNorm = VECTOR3(1,0,1) * pushVecNorm;
            newPush = VECTOR3(newPush.x, 0.0f, newPush.z);
        }
		if (pushIn < totalPush.Size()) {
			totalPush += newPush - pushIn * pushVecNorm;
		}
		else {
			totalPush = newPush;
		}
    }

    //最大の値を越したらそれ以上は押し返さないようにする
    if (totalPush.Size() > _maxLength) {
        totalPush = totalPush.Normalize() * _maxLength;
    }

    return totalPush;

}

void Pushback::Apply(Transform* _transform, Physics* _physics, bool  _affectVelocity, float  _maxLength) {
    VECTOR3 pos = _transform->position;
    VECTOR3 push = ResultPushback(_maxLength,pos);
    if (push.Size() < 0.001f) { // 微小なら無視
        return;
    }

    // 位置補正
    _transform->position += push;

    if (_affectVelocity && _physics) {
        VECTOR3 vel = _physics->GetVelocity();
        VECTOR3 pushNorm = push.Normalize();

        // めり込み方向の速度だけ打ち消す
        float dot = VDot(vel, pushNorm);
        if (dot < 0.0f) {
            vel -= pushNorm * dot;
            _physics->SetVelocity(vel);
        }
    }
}

bool Pushback::IsGrounded(float minYNormal){
    for (const auto& push : pushes) {
        // 法線のY成分が一定以上なら地面とみなす
        if (push.normal.y >= minYNormal) {
            return true;
        }
    }
    return false;
}