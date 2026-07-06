#include "SoundManager.h"
#include "Sound.h"
#include "../ResourceLoader.h"
#include "../Debug/Debug.h"
#include "../../Component/Object/BaseObject.h"
#include "../../Component/Transform/Transform.h"
#include "../Easing.h"
#include "../../Stage/StageSelectData.h"

SoundManager::SoundManager()
{
	Base3DSoundObj = nullptr;
}

SoundManager::~SoundManager()
{
	AllDeleteSound();
}

void SoundManager::Update()
{
	for (auto itr = feedInOutList.begin(); itr != feedInOutList.end(); ) {
		if ((*itr)->IsActive()) {
			(*itr)->Update();
			++itr;
		}
		else {
			itr = feedInOutList.erase(itr);
		}
	}
}

void SoundManager::Draw()
{
}

void SoundManager::SoundLoad(Sound_ID::SOUND_ID _type, std::string path, std::string exten, int volume)
{
	Sound* soundData = new Sound();
	soundData->SoundData(ResourceLoad::LoadSound(path, exten, _type), volume);
	sound.emplace(Sound_ID::GetSoundID(_type), soundData);
}

void SoundManager::PlaySceneLoad()
{
	//プレイシーンに使用する音のロード
	SoundLoad(Sound_ID::SOUND_ID::SWORD_HIT_SOUND,"swordHit", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_HIT_SOUND1,"swordHit000001", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_HIT_SOUND2,"swordHit000002", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_HIT_SOUND3,"swordHit000003", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_HIT_SOUND4,"swordHit000004", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_HIT_SOUND5,"swordHit000005", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_HIT_SOUND6,"swordHit000006", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_HIT_SOUND7,"swordHit000007", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_HIT_SOUND8,"swordHit0008", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_HIT_SOUND9,"swordHit0009", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_HIT_SOUND10,"swordHit00010", ".wav", 255);

	SoundLoad(Sound_ID::SOUND_ID::SWORD_WIND_SOUND1, "swordWind1", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_WIND_SOUND2, "swordWind2", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_WIND_SOUND3, "swordWind3", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_WIND_SOUND4, "swordWind4", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::SWORD_WIND_SOUND5, "swordWind5", ".wav", 255);

	SoundLoad(Sound_ID::SOUND_ID::JUST_AVOID_SOUND, "justAvoid10", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::ENEMY_ATTACK_BEFORE, "EnemyAttackBefore1", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::PLAYER_ATTACK_BEFORE, "PlayerAttackBefore", ".wav", 255);

	SoundLoad(Sound_ID::SOUND_ID::ENEMY_SWORD_WIND1, "EnemySword1", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::ENEMY_SWORD_WIND2, "EnemySword2", ".wav", 200);

	SoundLoad(Sound_ID::SOUND_ID::ENEMY_ATTACK_HIT1, "EnemyAttackHit1", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::ENEMY_ATTACK_HIT2, "EnemyAttackHit2", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::ENEMY_ATTACK_HIT3, "EnemyAttackHit3", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::ENEMY_ATTACK_HIT4, "EnemyAttackHit4", ".wav", 200);

	SoundLoad(Sound_ID::SOUND_ID::ENEMY_FALL, "EnemyFall2", ".wav", 255);

	SoundLoad(Sound_ID::SOUND_ID::V_P_ATTACK1, "P_AttackV1", ".wav", 180);
	SoundLoad(Sound_ID::SOUND_ID::V_P_ATTACK2, "P_AttackV2", ".wav", 180);
	SoundLoad(Sound_ID::SOUND_ID::V_P_ATTACK3, "P_AttackV3", ".wav", 180);
	SoundLoad(Sound_ID::SOUND_ID::V_P_ATTACK4, "P_AttackV4", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::V_P_LOSE, "P_V_AttackLose", ".wav", 255);

	SoundLoad(Sound_ID::SOUND_ID::V_P_DAMAGE1, "P_DamageV1", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::V_P_DAMAGE2, "P_DamageV2", ".wav", 255);

	SoundLoad(Sound_ID::SOUND_ID::V_P_JUST_AVOID, "P_JustAvoidV", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::V_P_AVOID, "P_AvoidV", ".wav", 255);

	SoundLoad(Sound_ID::SOUND_ID::V_E_ATTACK1, "E_AttackV1", ".wav",255);
	SoundLoad(Sound_ID::SOUND_ID::V_E_ATTACK2, "E_AttackV2", ".wav",255);
	SoundLoad(Sound_ID::SOUND_ID::V_E_ATTACK3, "E_AttackV3", ".wav",255);

	SoundLoad(Sound_ID::SOUND_ID::V_E_DAMAGE1, "E_DamageV1", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::V_E_DAMAGE2, "E_DamageV2", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::V_E_DAMAGE3, "E_DamageV3", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::V_E_DAMAGE4, "E_DamageV4", ".wav", 200);

	SoundLoad(Sound_ID::SOUND_ID::JUST_AVOID_SUCCESS, "success", ".wav", 255);

	std::string bgmName = StageSelectData::GetInstance()->GetNowStageData().bgm;
	SoundLoad(Sound_ID::SOUND_ID::PLAY_BGM, bgmName, ".wav", 150);
	SoundLoad(Sound_ID::SOUND_ID::WIN, "winSe", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::LOSE, "loseSe", ".mp3", 200);

	SoundLoad(Sound_ID::SOUND_ID::GROUND, "Ground", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::V_B_ROAR, "B_ROAR", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::PLAYER_SPECIAL_ATTACK_V, "PlayerSpecialAttackV", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::CUTIN_START, "CutInStart", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::PLAYER_SPECIAL_ATTACK_ATTACK_V1, "PlayerSpecialAttackAttackV1", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::PLAYER_SPECIAL_ATTACK_ATTACK_V2, "PlayerSpecialAttackAttackV2", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::PLAYER_SPECIAL_ATTACK_FINAL_ATTACKV, "PlayerSpecialFinalAttackV", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::PLAYER_SPECIAL_ATTACK_CAN, "PlayerSpecialAttackCan", ".wav", 255);
	
	SoundLoad(Sound_ID::SOUND_ID::PLAYER_WALK1, "p_walk1", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::BOSS_WALK, "AS_373275_ドスッ_重めの足音", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::BOSS_ROAR_VOICE, "B_ROAR_2", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::BOSS_JUMP_WIND, "BOSS_JUMP_WIND", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::BOSS_BEFORE, "BossBefore", ".wav", 100);
	SoundLoad(Sound_ID::SOUND_ID::PLAYER_CHARGE, "PlayerCharge", ".wav", 150);
	SoundLoad(Sound_ID::SOUND_ID::PLAYER_MAX_CHARGE, "PlayerMaxCharge", ".wav", 180);
	SoundLoad(Sound_ID::SOUND_ID::PLAYER_CHARGE_END, "PlayerChargeEnd", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::PLAYER_SPECIAL_ATTACK_BOM, "PlayerSpecialAttackBom", ".wav", 200);

	SoundLoad(Sound_ID::SOUND_ID::ROCK_BLAST,	"rockBlast", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::ROCK_BREAK,	"rockBreak", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::ROCK_FALL,	"rockFall", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::ROCK_GROUND,	"rockGround", ".wav", 255);

	SoundLoad(Sound_ID::SOUND_ID::BOSS_ROCK_APPEAR, "BossRockAppear", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::BOSS_ROCK_FIRING, "BossRockFiring", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::BOSS_SHOT_CHARGE, "BossShotCharge", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::BOSS_SHOT_FIRING, "BossShotFiring", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::BOSS_TREE_SWING, "BossTreeSwing", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::BOSS_JUMP, "BossJump", ".wav", 255);

	SoundLoad(Sound_ID::PUSH, "titlePush (1)", ".wav", 250);
	//SoundLoad(Sound_ID::SOUND_ID::PLAYER_WALK2, "p_walk2", ".wav", 255);
	//SoundLoad(Sound_ID::SOUND_ID::PLAYER_WALK3, "p_walk3", ".wav", 255);
	//SoundLoad(Sound_ID::SOUND_ID::PLAYER_WALK4, "p_walk4", ".wav", 255);
	//SoundLoad(Sound_ID::SOUND_ID::PLAYER_WALK5, "p_walk5", ".wav", 255);

	SoundLoad(Sound_ID::RANGED_ATK_FIN, "rockFall", ".wav", 250);
}

void SoundManager::TitleSceneLoad()
{
	//タイトルシーンに使う音のロード
	SoundLoad(Sound_ID::TITLE_BGM, "titleBGM", ".wav", 150);
	SoundLoad(Sound_ID::PUSH, "titlePush (1)", ".wav", 250);
	SoundLoad(Sound_ID::SOUND_ID::JUST_AVOID_SOUND, "justAvoid10", ".wav", 200);
	SoundLoad(Sound_ID::SOUND_ID::JUST_AVOID_SUCCESS, "success", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::V_P_JUST_AVOID, "P_JustAvoidV", ".wav", 255);
}

void SoundManager::CommonSceneLoad()
{
}

void SoundManager::PlaySe(Sound_ID::SOUND_ID _type)
{
	sound[Sound_ID::GetSoundID(_type)]->PlaySe();
}

void SoundManager::PlayBGM(Sound_ID::SOUND_ID _type, bool _loop, bool firstPlay)
{
	sound[Sound_ID::GetSoundID(_type)]->PlayBGM(_loop,firstPlay);
}

void SoundManager::StopBGM(Sound_ID::SOUND_ID _type)
{
	sound[Sound_ID::GetSoundID(_type)]->StopBGM();
}

void SoundManager::StopSE(Sound_ID::SOUND_ID _type)
{
	sound[Sound_ID::GetSoundID(_type)]->StopSE();
}

void SoundManager::AllDeleteSound()
{
	feedInOutList.clear();
	for (auto& s : sound) {
		delete s.second;
	}
	sound.clear();
	Base3DSoundObj = nullptr;
}

void SoundManager::RandamSe(std::string _name, int num)
{
	int randomNum = GetRand(num - 1) + 1;

	std::string value = std::to_string(randomNum);

	std::string cats = _name + value;

	auto itr = sound.find(cats);
	if (itr == sound.end()) {
		Debug::DebugLog("このファイルは存在しません。");
	}
	sound[cats]->PlaySe();

}

bool SoundManager::CheckSe(Sound_ID::SOUND_ID _id)
{
	return CheckSoundMem(ResourceLoad::GetSoundHandle(_id));
}

void SoundManager::FeedInStart(Sound_ID::SOUND_ID _id, float _time)
{
	sound[Sound_ID::GetSoundID(_id)]->BGMFeedIn(_time);
	SetFeedInOutList(sound[Sound_ID::GetSoundID(_id)]);
}

void SoundManager::FeedInOut(Sound_ID::SOUND_ID _id, float _time)
{
	sound[Sound_ID::GetSoundID(_id)]->BGMFeedOut(_time);
	SetFeedInOutList(sound[Sound_ID::GetSoundID(_id)]);
}

void SoundManager::SetFeedInOutList(Sound* sound)
{
	for (auto itr = feedInOutList.begin(); itr != feedInOutList.end();) {
		if ((*itr) == sound) {
			itr = feedInOutList.erase(itr);
		}
		else {
			itr++;
		}
	}
	feedInOutList.push_back(sound);
}

void SoundManager::ChangeVolumeSound(Sound_ID::SOUND_ID _id, int _change)
{
	sound[Sound_ID::GetSoundID(_id)]->ChangeVolumeSound(_change);
}

void SoundManager::BaseVolumeChange(Sound_ID::SOUND_ID _id)
{
	sound[Sound_ID::GetSoundID(_id)]->BaseChangeVolumeSound();
}

void SoundManager::Play3DSound(Sound_ID::SOUND_ID _id, BaseObject* _targetObj, float _minVolSize, float _maxVolSize)
{
	Transform*  targetTransform = _targetObj->GetTransform();
	Transform* base3DTransfom = Base3DSoundObj->GetTransform();
	VECTOR3 dist = targetTransform->position - base3DTransfom->position;
	float size = dist.Size();

	int vol = 0;
	//最低音量の距離より離れたら音量をなくす
	if (_minVolSize <= size) {
		vol = 0;
	}
	//最大音量の距離より近づいたら音量を最大にする
	else if (_maxVolSize >= size) {
		vol = sound[Sound_ID::GetSoundID(_id)]->GetVolumn();
	}
	else {
		float rate = (size - _minVolSize) / (_maxVolSize - _minVolSize);
		vol = Easing::Lerp(0, sound[Sound_ID::GetSoundID(_id)]->GetVolumn(),rate);
	}
	sound[Sound_ID::GetSoundID(_id)]->ChangeVolumeSound(vol);

}

void SoundManager::Base3DSoundObject(BaseObject* _base)
{
	Base3DSoundObj = _base;
}

void SoundManager::PlayRamdomChangeFrequencySe(Sound_ID::SOUND_ID _id, int _ramdom, int _baseFrequ)
{
	sound[Sound_ID::GetSoundID(_id)]->PlayRamdomChangeFrequencySe(_ramdom,_baseFrequ);
}