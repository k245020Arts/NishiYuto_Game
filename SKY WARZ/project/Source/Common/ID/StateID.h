#pragma once
#include <string>

namespace StateID{

    enum State_ID
    {
        PLAYER_WAIT_S,
        PLAYER_WALK_S,
        PLAYER_AVOID_S,
        PLAYER_JUST_AVOID_S,
        PLAYER_ATTACK1_S,
        PLAYER_JUST_AVOID_ATTACK1_S,
        PLAYER_ATTACK2_S,
        PLAYER_ATTACK3_S,
        PLAYER_ATTACK5_S,
        PLAYER_ATTACK4_S,
        PLAYER_DAMAGE_S,
        PLAYER_BLOW_AWAY_S,
        PLAYER_DIE_S,
        PLAYER_TURN_S,
        PLAYER_SPECIAL_ATTACK_S,
        PLAYER_WIN_STATE_S,
        PLAYER_BEFORE_S,
        PLAYER_BOSS_APPEAR_S,
        PLAYER_HEAVY_CHARGE_S,
        PLAYER_HEAVY_ATTACK_S,
        PLAYER_LOSE_S,

        BOSS_IDOL_S,
        BOSS_RUN_S,
        BOSS_COOL_TIME_S,
        ATTACK_SORTING_S,
        BOSS_WALK,
        BOSS_NORMAL_ATTACK1_S,
        BOSS_NORMAL_ATTACK2_S,
        BOSS_NORMAL_ATTACK3_S,
        BOSS_NORMAL_ATTACK4_S,
        BOSS_NORMAL_ATTACK5_S,
        BOSS_NORMAL_ATTACK6_S,
        BOSS_NORMAL_ATTACK7_S,
        BOSS_SPECIAL_ATTACK1_S,
        BOSS_SPECIAL_SMALL_ATTACK1_S,
        BOSS_SPECIAL_ATTACK2_S,
        BOSS_DIE_S,
        B_ROAR_S,
        B_THREAT_S,
        BOSS_DAMAGE_S,
        BOSS_APPEAR_S,
        BOSS_LOSE_S,
        BOSS_HALF_ATTACK_S,
        BOSS_FEAR_S,
        BOSS_BACKSTEP_S,
        BOSS_WIN_S,
        FOLLOW_CAMERA_S,
        JUST_AVOID_CAMERA_S,
        JUST_AVOID_ATTACK_CAMERA_S,
        JUST_AVOID_ATTACK_HIT_CAMERA_S,
        FREE_CAMERA_S,
        PLAYER_SPECIAL_ATTACK_CAMERA_S,
        CUT_SCENE_CAMERA_S,
        PLAYER_HEAVY_CHARGE_CAMERA_S,
        R_ENEMY_CAMERA_S,
        R_ENEMY_FINISH_CAMERA_S,

        T_ENEMY_IDOL_S,
        T_ENEMY_RUN_S,
        T_ENEMY_ATTACK_S,
        T_ENEMY_DEAD,
        COOPERATEATTACK1,
        COOPERATEATTACK2,
        T_ENEMY_STANDBY,
        T_ENEMY_DAMAGE,
        T_ENEMY_WAITSEE,
        T_ENEMY_STAYSKY,
        T_ENEMY_EVADE,

        STATE_MAX,
    };

    void DeleteStateID();

	std::string GetID(State_ID _type);
	void SetID(std::string _state, State_ID _type);

    State_ID StringToID(std::string _str);
}