#pragma once
#include <string>
#include <memory>
#include "BossState/Attack/BossAttackJsonParse.h"
#include "../../Component/Animator/Animator.h"
#include "../../Component/EnemyAttackObject/BossRock/BossRockManager.h"

class AttackSorting;
struct ActionParam;

class BossAttackDataSerializer
{
public:
	BossAttackDataSerializer();
	BossAttackDataSerializer(std::shared_ptr<AttackSorting> _sort,Boss* _boss,std::string _bossName);
	~BossAttackDataSerializer();

	void SetAnim(Animator* anim);
	void SetThrowManager(BossRockManager* _data);

	void Update();

private:
	bool windowMode;
	std::string BossName;
	std::shared_ptr<AttackSorting> sorting;
	std::unordered_map<std::string, EnemyAttackBase::BossAttackParam> attackParam;
	std::vector<ActionParam> actions;
	std::vector<std::string> attackKeys;
	char newAttackID[64];
	char newAnimFile[128];

	void ActionsSave();
	void AttackSave(std::string _attackID);
	void DrawAttackParamEditor(std::string _selectID);
	void DrawTransform(const char* label, Transform& t);
	void CopyParam(std::string _selectID);
	void DrawThrowObjectEditor(std::vector<EnemyAttackBase::ThrowObjectAttackData>& list, const std::map<std::string, BossRockManager::BossThrowObjectData>& throwObjectsData);
	void DrawRayColliderInfo(const char* label, EnemyAttackBase::RayColliderInfo& r);
	void DrawDountColliderInfo(const char* label, EnemyAttackBase::DountColliderInfo& d);
	void DrawAddThrowObjects(std::map<std::string, BossRockManager::BossThrowObjectData>& throwObjectsData);
	const char* MakeLabel(const char* label, const char* category, int index);
	Animator* bossAnim;
	Animator::AnimFileInfo currentSelectAnimInfos;

	std::vector<std::string> animFileName;
	std::map<std::string, BossRockManager::BossThrowObjectData> throwObjectsData;
	BossRockManager* rockManager;
	Boss* boss;
	bool isEffect;
	std::vector<std::string> throwObjectKeys;
	bool throwKeyReset;

	void DrawSoundEventEditor(std::vector<EnemyAttackBase::SoundEffectEvent>& _eventData);

	std::vector<std::string> soundFileNames;
};