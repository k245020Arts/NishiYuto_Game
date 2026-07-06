#pragma once
#include "../../../EnemyState/Attack/EnemyAttackBase.h"

inline void to_json(JSON& j, const EnemyAttackBase::RayColliderInfo& p)
{
	j = JSON{
		{"rayStartPos", p.rayStartPos},
		{"rayFinishPos", p.rayFinishPos}
	};
}

inline void from_json(const JSON& j, EnemyAttackBase::RayColliderInfo& p)
{
	if (j.contains("rayStartPos")) j.at("rayStartPos").get_to(p.rayStartPos);
	if (j.contains("rayFinishPos")) j.at("rayFinishPos").get_to(p.rayFinishPos);
}

inline void to_json(JSON& j, const EnemyAttackBase::DountColliderInfo& p)
{
	j = JSON{
		{"inRadius", p.inRadius},
		{"outRadius", p.outRadius},
	};
}

inline void from_json(const JSON& j, EnemyAttackBase::DountColliderInfo& p)
{
	if (j.contains("inRadius")) j.at("inRadius").get_to(p.inRadius);
	if (j.contains("outRadius")) j.at("outRadius").get_to(p.outRadius);
}

inline void to_json(JSON& j, const EnemyAttackBase::SoundEffectEvent& p)
{
	j = JSON{
		{"name", p.name},
		{"soundStartFrame", p.soundStartFrame},
		{"soundStopFrame", p.soundStopFrame},
		{"loop", p.loop},
		{"soundStopFrameAfetrSoundPlay", p.soundStopFrameAfetrSoundPlay},
		{"loopAnim", p.loopAnim}
	};
}

inline void from_json(const JSON& j, EnemyAttackBase::SoundEffectEvent& p)
{
	if (j.contains("name")) j.at("name").get_to(p.name);
	if (j.contains("soundStartFrame")) j.at("soundStartFrame").get_to(p.soundStartFrame);
	if (j.contains("soundStopFrame")) j.at("soundStopFrame").get_to(p.soundStopFrame);
	if (j.contains("loop")) j.at("loop").get_to(p.loop);
	if (j.contains("soundStopFrameAfetrSoundPlay"))
		j.at("soundStopFrameAfetrSoundPlay").get_to(p.soundStopFrameAfetrSoundPlay);
	if (j.contains("loopAnim")) j.at("loopAnim").get_to(p.loopAnim);
}

inline void to_json(JSON& j, const EnemyAttackBase::ThrowObjectAttackData& p)
{
	j = JSON{
		{"throwObjectID", p.throwObjectID},
		{"baseGravity", p.baseGravity},
		{"baseFirction", p.baseFirction}
	};

	// pushColl
	if (p.pushCollCan)
	{
		j["pushCollCan"] = p.pushCollCan;
		j["pushCollTransform"] = p.pushCollTransform;
	}

	// rand
	if (p.randCan)
	{
		j["randCan"] = p.randCan;
		j["randCollInfo"] = p.randCollInfo;

	}

	j["randTime"] = p.randTime;

	// playerHit
	if (p.playerHit)
	{
		j["playerHit"] = p.playerHit;
		j["playerHitNoReaction"] = p.playerHitNoReaction;
		j["playerHitCollRadius"] = p.playerHitCollRadius;
		j["playerHitJustAvoidCollRadius"] = p.playerHitJustAvoidCollRadius;
		if (p.capsule) {
			j["Capsule"] = p.capsule;
			j["CapsuleStartPos"] = p.capselStartPos;
			j["CapsuleEndPos"] = p.capselEndPos;
			j["capsuleEndPosExtend"] = p.capsuleEndPosExtend;
			j["capsuleEndPosExtendOffset"] = p.capsuleEndPosExtendOffset;
			j["capsuleEndPosExtendFromThrowStart"] = p.capsuleEndPosExtendFromThrowStart;
			j["capsuleEndPosExtendStartAnimFrame"] = p.capsuleEndPosExtendStartAnimFrame;
		}
	}

	if (p.alotCollsionHit) {
		j["alotCollsionHit"] = p.alotCollsionHit;
		j["alotHitColliderCreateCounter"] = p.alotHitColliderCreateCounter;
	}

	// playerGroundHit
	if (p.playerGroundHit)
	{
		j["playerGroundHit"] = p.playerGroundHit;
		j["playerGroundCollRadius"] = p.playerGroundCollRadius;
		j["playerGroundOneHit"] = p.playerGroundOneHit;
		j["playerGroundNoDamageReactionHit"] = p.playerGroundNoDamageReactionHit;
	}

	// bossHit
	if (p.bossHit)
	{
		j["bossHit"] = p.bossHit;
		j["bossHitCollRadius"] = p.bossHitCollRadius;
	}

	// 飛び道具
	if (p.playerAttackFlying)
	{
		j["playerAttackFlying"] = p.playerAttackFlying;
		j["playerAttackFlyingCollRadius"] = p.playerAttackFlyingCollRadius;
		j["flyingSpeed"] = p.flyingSpeed;
		j["flyingHeight"] = p.flyingHeight;

	}

	// 突進ヒット
	if (p.bossRushHit)
	{
		j["bossRushHit"] = p.bossRushHit;
		j["bossRushHitCollRadius"] = p.bossRushHitCollRadius;
	}

	// 爆発
	if (p.blastCan)
	{
		j["blastCan"] = p.blastCan;
		j["blastColliderInfo"] = p.blastColliderInfo;
		j["blastJustAvoidColliderInfo"] = p.blastJustAvoidColliderInfo;
		j["blastBlinkMaxCounter"] = p.blastBlinkMaxCounter;
		j["randomBlast"] = p.randomBlast;
		j["randomBlastRate"] = p.randomBlastRate;
		j["maxRadius"] = p.maxRadius;
		j["waveSpeed"] = p.waveSpeed;
	}

	if (p.randomHeight) {
		j["randomHeight"] = p.randomHeight;
		j["minHeight"] = p.minHeight;
		j["maxHeight"] = p.maxHeight;
	}

	if (p.randomSpeed) {
		j["randomSpeed"] = p.randomSpeed;
		j["minSpeed"] = p.minSpeed;
		j["maxSpeed"] = p.maxSpeed;
	}

	// 予測円
	if (p.predictionCicleCan)
	{
		j["predictionCicleCan"] = p.predictionCicleCan;
		j["predictionCicleColliderInfo"] = p.predictionCicleColliderInfo;
	}

	// 投擲
	if (p.armThrow)
	{
		j["armThrow"] = p.armThrow;
		j["armSwordHand"] = p.armSwordHand;
		j["armFrameNum"] = p.armFrameNum;
		j["throwStartTime"] = p.throwStartTime;
		j["throwObjectApperaTime"] = p.throwObjectApperaTime;
		j["armAddPos"] = p.armAddPos;
		j["throwToPlayer"] = p.throwToPlayer;
		j["thorwToFront"] = p.thorwToFront;
		j["throwSpeed"] = p.throwSpeed;
		j["upSpeed"] = p.upSpeed;
		j["throwFirstSpeed"] = p.throwFirstSpeed;
		j["diffusionAngle"] = p.diffusionAngle;
		j["throwToFall"] = p.throwToFall;
		j["throwHeight"] = p.throwHeight;
		j["throwFallGravity"] = p.throwFallGravity;
		j["throwToFallToPlayer"] = p.throwToFallToPlayer;
		j["freeDir"] = p.freeDir;
		j["thorwStartPos"] = p.thorwStartPos;
		j["thorwVelocity"] = p.thorwVelocity;
	}

	j["throwToFall"] = p.throwToFall;
	if (p.throwToFall)
	{
		j["throwHeight"] = p.throwHeight;
		j["throwFallGravity"] = p.throwFallGravity;
		j["thorwStartPos"] = p.thorwStartPos;
		j["thorwVelocity"] = p.thorwVelocity;
	}

	// --- プレイヤー追尾落下 ---
	j["throwToFallToPlayer"] = p.throwToFallToPlayer;

	// --- 自由方向系 ---
	j["freeDir"] = p.freeDir;
	if (p.freeDir)
	{
		j["thorwStartPos"] = p.thorwStartPos;
		j["thorwVelocity"] = p.thorwVelocity;
	}

	// その他
	if (p.groundDelete)
	{
		j["groundDelete"] = p.groundDelete;
	}

	if (p.playerAttackObjectDrop)
	{
		j["playerAttackObjectDrop"] = p.playerAttackObjectDrop;
	}

	if (p.attackFinishDelete)
	{
		j["attackFinishDelete"] = p.attackFinishDelete;
	}
}

inline void from_json(const JSON& j, EnemyAttackBase::ThrowObjectAttackData& p)
{
	// 基本
	if (j.contains("throwObjectID")) j.at("throwObjectID").get_to(p.throwObjectID);
	if (j.contains("baseGravity")) j.at("baseGravity").get_to(p.baseGravity);
	if (j.contains("baseFirction")) j.at("baseFirction").get_to(p.baseFirction);

	// pushColl
	if (j.contains("pushCollCan"))
	{
		j.at("pushCollCan").get_to(p.pushCollCan);

		if (p.pushCollCan && j.contains("pushCollTransform"))
		{
			j.at("pushCollTransform").get_to(p.pushCollTransform);
		}
	}
	else p.pushCollCan = false;

	// rand
	if (j.contains("randCan"))
	{
		j.at("randCan").get_to(p.randCan);

		if (p.randCan)
		{
			if (j.contains("randCollInfo")) j.at("randCollInfo").get_to(p.randCollInfo);

		}
	}
	else p.randCan = false;

	if (j.contains("randTime")) j.at("randTime").get_to(p.randTime);

	// playerHit
	if (j.contains("playerHit"))
	{
		j.at("playerHit").get_to(p.playerHit);

		if (p.playerHit)
		{
			if (j.contains("playerHitNoReaction")) j.at("playerHitNoReaction").get_to(p.playerHitNoReaction);
			if (j.contains("playerHitCollRadius")) j.at("playerHitCollRadius").get_to(p.playerHitCollRadius);
			if (j.contains("playerHitJustAvoidCollRadius")) j.at("playerHitJustAvoidCollRadius").get_to(p.playerHitJustAvoidCollRadius);
			if (j.contains("Capsule")) j.at("Capsule").get_to(p.capsule);
			if (p.capsule) {
				if (j.contains("CapsuleStartPos")) j.at("CapsuleStartPos").get_to(p.capselStartPos);
				if (j.contains("CapsuleEndPos")) j.at("CapsuleEndPos").get_to(p.capselEndPos);
				if (j.contains("capsuleEndPosExtend")) j.at("capsuleEndPosExtend").get_to(p.capsuleEndPosExtend);
				if (j.contains("capsuleEndPosExtendOffset")) j.at("capsuleEndPosExtendOffset").get_to(p.capsuleEndPosExtendOffset);
				if (j.contains("capsuleEndPosExtendFromThrowStart")) j.at("capsuleEndPosExtendFromThrowStart").get_to(p.capsuleEndPosExtendFromThrowStart);
				if (j.contains("capsuleEndPosExtendStartAnimFrame")) j.at("capsuleEndPosExtendStartAnimFrame").get_to(p.capsuleEndPosExtendStartAnimFrame);
			}
		}
	}
	else p.playerHit = false;

	if (j.contains("alotCollsionHit"))
	{
		j.at("alotCollsionHit").get_to(p.alotCollsionHit);
		if (p.alotCollsionHit) {
			if (j.contains("alotHitColliderCreateCounter")) j.at("alotHitColliderCreateCounter").get_to(p.alotHitColliderCreateCounter);
		}
	}
	else p.alotCollsionHit = false;

	// playerGroundHit
	if (j.contains("playerGroundHit"))
	{
		j.at("playerGroundHit").get_to(p.playerGroundHit);

		if (p.playerGroundHit)
		{
			if (j.contains("playerGroundCollRadius")) j.at("playerGroundCollRadius").get_to(p.playerGroundCollRadius);
			if (j.contains("playerGroundOneHit")) j.at("playerGroundOneHit").get_to(p.playerGroundOneHit);
			if (j.contains("playerGroundNoDamageReactionHit")) j.at("playerGroundNoDamageReactionHit").get_to(p.playerGroundNoDamageReactionHit);

		}
	}
	else p.playerGroundHit = false;

	// bossHit
	if (j.contains("bossHit"))
	{
		j.at("bossHit").get_to(p.bossHit);

		if (p.bossHit)
		{
			if (j.contains("bossHitCollRadius")) j.at("bossHitCollRadius").get_to(p.bossHitCollRadius);
		}
	}
	else p.bossHit = false;

	// playerAttackFlying
	if (j.contains("playerAttackFlying"))
	{
		j.at("playerAttackFlying").get_to(p.playerAttackFlying);

		if (p.playerAttackFlying)
		{
			if (j.contains("playerAttackFlyingCollRadius")) j.at("playerAttackFlyingCollRadius").get_to(p.playerAttackFlyingCollRadius);
			if (j.contains("flyingSpeed")) j.at("flyingSpeed").get_to(p.flyingSpeed);
			if (j.contains("flyingHeight")) j.at("flyingHeight").get_to(p.flyingHeight);

		}
	}
	else p.playerAttackFlying = false;

	// bossRushHit
	if (j.contains("bossRushHit"))
	{
		j.at("bossRushHit").get_to(p.bossRushHit);

		if (p.bossRushHit)
		{
			if (j.contains("bossRushHitCollRadius")) j.at("bossRushHitCollRadius").get_to(p.bossRushHitCollRadius);
		}
	}
	else p.bossRushHit = false;

	// blast
	if (j.contains("blastCan"))
	{
		j.at("blastCan").get_to(p.blastCan);

		if (p.blastCan)
		{
			if (j.contains("blastColliderInfo")) j.at("blastColliderInfo").get_to(p.blastColliderInfo);
			if (j.contains("blastJustAvoidColliderInfo")) j.at("blastJustAvoidColliderInfo").get_to(p.blastJustAvoidColliderInfo);
			if (j.contains("blastBlinkMaxCounter")) j.at("blastBlinkMaxCounter").get_to(p.blastBlinkMaxCounter);
			if (j.contains("randomBlast")) j.at("randomBlast").get_to(p.randomBlast);
			if (j.contains("randomBlastRate")) j.at("randomBlastRate").get_to(p.randomBlastRate);
			if (j.contains("maxRadius")) j.at("maxRadius").get_to(p.maxRadius);
			if (j.contains("waveSpeed")) j.at("waveSpeed").get_to(p.waveSpeed);
		}
	}
	else p.blastCan = false;

	if (j.contains("randomHeight")) {
		j.at("randomHeight").get_to(p.randomHeight);
		if (p.randomHeight) {
			if (j.contains("minHeight")) j.at("minHeight").get_to(p.minHeight);
			if (j.contains("maxHeight")) j.at("maxHeight").get_to(p.maxHeight);
		}
	}

	if (j.contains("randomSpeed")) {
		j.at("randomSpeed").get_to(p.randomSpeed);
		if (p.randomSpeed) {
			if (j.contains("minSpeed")) j.at("minSpeed").get_to(p.minSpeed);
			if (j.contains("maxSpeed")) j.at("maxSpeed").get_to(p.maxSpeed);
		}
	}

	// predictionCicle
	if (j.contains("predictionCicleCan"))
	{
		j.at("predictionCicleCan").get_to(p.predictionCicleCan);

		if (p.predictionCicleCan)
		{
			if (j.contains("predictionCicleColliderInfo")) j.at("predictionCicleColliderInfo").get_to(p.predictionCicleColliderInfo);
		}
	}
	else p.predictionCicleCan = false;

	// armThrow
	if (j.contains("armThrow"))
	{
		j.at("armThrow").get_to(p.armThrow);

		if (p.armThrow)
		{
			if (j.contains("armFrameNum")) j.at("armFrameNum").get_to(p.armFrameNum);
			if (j.contains("throwStartTime")) j.at("throwStartTime").get_to(p.throwStartTime);
			if (j.contains("throwObjectApperaTime")) j.at("throwObjectApperaTime").get_to(p.throwObjectApperaTime);
			if (j.contains("armSwordHand")) j.at("armSwordHand").get_to(p.armSwordHand);
			if (j.contains("armAddPos")) j.at("armAddPos").get_to(p.armAddPos);
			if (j.contains("throwToPlayer")) j.at("throwToPlayer").get_to(p.throwToPlayer);
			if (j.contains("thorwToFront")) j.at("thorwToFront").get_to(p.thorwToFront);
			if (j.contains("throwSpeed")) j.at("throwSpeed").get_to(p.throwSpeed);
			if (j.contains("upSpeed")) j.at("upSpeed").get_to(p.upSpeed);
			if (j.contains("throwFirstSpeed")) j.at("throwFirstSpeed").get_to(p.throwFirstSpeed);
			if (j.contains("diffusionAngle")) j.at("diffusionAngle").get_to(p.diffusionAngle);
			if (j.contains("throwToFall")) j.at("throwToFall").get_to(p.throwToFall);
			if (j.contains("throwHeight")) j.at("throwHeight").get_to(p.throwHeight);
			if (j.contains("throwFallGravity")) j.at("throwFallGravity").get_to(p.throwFallGravity);
			if (j.contains("throwToFallToPlayer")) j.at("throwToFallToPlayer").get_to(p.throwToFallToPlayer);
			if (j.contains("freeDir")) j.at("freeDir").get_to(p.freeDir);
			if (j.contains("thorwStartPos")) j.at("thorwStartPos").get_to(p.thorwStartPos);
			if (j.contains("thorwVelocity")) j.at("thorwVelocity").get_to(p.thorwVelocity);
		}
	}
	else p.armThrow = false;

	// --- 落下関連 ---
	if (j.contains("throwToFall"))
	{
		j.at("throwToFall").get_to(p.throwToFall);

		if (p.throwToFall)
		{
			if (j.contains("throwHeight")) j.at("throwHeight").get_to(p.throwHeight);
			if (j.contains("throwFallGravity")) j.at("throwFallGravity").get_to(p.throwFallGravity);
			if (j.contains("thorwStartPos")) j.at("thorwStartPos").get_to(p.thorwStartPos);
			if (j.contains("thorwVelocity")) j.at("thorwVelocity").get_to(p.thorwVelocity);
		}
	}

	// --- プレイヤー追尾落下 ---
	if (j.contains("throwToFallToPlayer"))
	{
		j.at("throwToFallToPlayer").get_to(p.throwToFallToPlayer);
	}

	// --- 自由方向系 ---
	if (j.contains("freeDir"))
	{
		j.at("freeDir").get_to(p.freeDir);

		if (p.freeDir)
		{
			if (j.contains("thorwStartPos")) j.at("thorwStartPos").get_to(p.thorwStartPos);
			if (j.contains("thorwVelocity")) j.at("thorwVelocity").get_to(p.thorwVelocity);
		}
	}

	// groundDelete
	if (j.contains("groundDelete"))
	{
		j.at("groundDelete").get_to(p.groundDelete);
	}
	else p.groundDelete = false;

	// playerAttackObjectDrop
	if (j.contains("playerAttackObjectDrop"))
	{
		j.at("playerAttackObjectDrop").get_to(p.playerAttackObjectDrop);
	}
	else p.playerAttackObjectDrop = false;

	if (j.contains("attackFinishDelete"))
	{
		j.at("attackFinishDelete").get_to(p.attackFinishDelete);
	}
	else p.attackFinishDelete = false;
}

inline void to_json(JSON& j, const EnemyAttackBase::BossAttackParam& p)
{
	j = JSON{
		{"bossID", p.bossID},
		{"animNum", p.animNum},
		{"useFlash", p.useFlash},
		{"attackFlashStartTime", p.attackFlashStartTime},
		{"slowTime", p.slowTime},
		{"slowAmout", p.slowAmout},
		{"speedUpMotionSpeed", p.speedUpMotionSpeed},
		{"attackCollsionStartTime", p.attackCollsionStartTime},
		{"attackCollsionEndTime", p.attackCollsionEndTime},
		{"justAvoidCollsionStartTime", p.justAvoidCollsionStartTime},
		{"justAvoidCollsionEndTime", p.justAvoidCollsionEndTime},
		{"attackPositionFrameNum", p.attackPositionFrameNum},
		{"attackSoundStartTime", p.attackSoundStartTime},
		{"attackCollTransform", p.attackCollTransform},
		{"justAvoidCollTransform", p.justAvoidCollTransform},
		{"animID", ID::GetID(p.animID)},
		{"attackBeforeAnimID", ID::GetID(p.attackBeforeAnimID)},
		{"hitDamage", p.hitDamage},
		{"damagePattern", EnemyAttackBase::ToString(p.damagePattern)},
		{"voiceName", p.voiceName}
	};

	// =========================
	// 移動
	// =========================
	if (p.frontMove)
	{
		j["frontMove"] = p.frontMove;
		j["moveSpeed"] = p.moveSpeed;
	}

	// =========================
	// プレイヤー追従
	// =========================
	if (p.playerAloowMove)
	{
		j["playerAloowMove"] = p.playerAloowMove;
		j["baseSpeed"] = p.baseSpeed;
		j["baseFirstSpeed"] = p.baseFirstSpeed;
		j["playerNearStop"] = p.playerNearStop;
		j["playerNearAloowStop"] = p.playerNearAloowStop;
		j["addVelocity"] = p.addVelocity;
		j["playerBaseNear"] = p.playerBaseNear;
		j["maxMoveSpeed"] = p.maxMoveSpeed;
		j["minMoveSpeed"] = p.minMoveSpeed;
		j["moveStartTime"] = p.moveStartTime;
		j["moveFinishTime"] = p.moveFinishTime;
	}

	// =========================
	// 突進
	// =========================
	if (p.rushMove)
	{
		j["rushMove"] = p.rushMove;
		j["rushAfterAnimID"] = ID::GetID(p.rushAfterAnimID);
		j["rushAfterSpeed"] = p.rushAfterSpeed;
		j["rushSoundRightFoot"] = p.rushSoundRightFoot;
		j["rushSoundLeftFoot"] = p.rushSoundLeftFoot;
		j["rushTime"] = p.rushTime;
		j["rushColl"] = p.rushColl;
		j["addRushCollScale"] = p.addRushCollScale;
	}

	// =========================
	// 回転
	// =========================
	if (p.rotateMove)
	{
		j["rotateMove"] = p.rotateMove;
		j["angleMoveAmout"] = p.angleMoveAmout;
	}

	// =========================
	// ジャンプ
	// =========================
	if (p.jump)
	{
		j["jump"] = p.jump;
		j["addGravity"] = p.addGravity;
		j["jumpSpeed"] = p.jumpSpeed;
		j["jumpStartTime"] = p.jumpStartTime;
		j["groundEffectStartTime"] = p.groundEffectStartTime;
		j["jumpGroundEffect"] = static_cast<int>(p.jumpGroundEffect);
		j["groundShakeCamera"] = p.groundShakeCamera;
		j["groundShakeTime"] = p.groundShakeTime;
	}

	// =========================
	// 衝撃波
	// =========================
	if (p.shockWave)
	{
		j["shockWave"] = p.shockWave;
		j["shockMoveEffect"] = static_cast<int>(p.shockMoveEffect);
		j["shockWaveSpeed"] = p.shockWaveSpeed;
		j["startRange"] = p.startRange;
	}

	// =========================
	// 見る
	// =========================
	if (p.lookPlayer)
	{
		j["lookPlayer"] = p.lookPlayer;
		j["lookNum"] = p.lookNum;
		j["lookMaxCounter"] = p.lookMaxCounter;
	}

	// =========================
	// 投擲
	// =========================
	if (p.throwObject)
	{
		j["throwObject"] = p.throwObject;
		j["throwAttackData"] = p.throwAttackData;
		j["armThrow"] = p.armThrow;
		j["throwObjectNum"] = p.throwObjectNum;
		j["objectApperaPosition"] = p.objectApperaPosition;
		j["intervalTime"] = p.intervalTime;
		j["intervalTimeSub"] = p.intervalTimeSub;
		j["maxIntervalTime"] = p.maxIntervalTime;
		j["minIntervalTime"] = p.minIntervalTime;
	}

	// =========================
	// カメラ
	// =========================
	if (p.attackCameraBossLook)
	{
		j["attackCameraBossLook"] = p.attackCameraBossLook;
		j["cameraChangeSpeed"] = p.cameraChangeSpeed;
	}

	// トレイル
	if (p.useTrail)
	{
		j["useTrail"] = p.useTrail;
		j["trailRightHand"] = p.trailRightHand;
	}

	if (!p.soundEvent.empty()) {
		j["soundEvent"] = p.soundEvent;
	}

}

inline void from_json(const JSON& j, EnemyAttackBase::BossAttackParam& p)
{
	// 基本
	if (j.contains("bossID")) j.at("bossID").get_to(p.bossID);
	if (j.contains("attackID")) j.at("attackID").get_to(p.attackID);
	if (j.contains("animNum")) j.at("animNum").get_to(p.animNum);
	if (j.contains("useFlash")) j.at("useFlash").get_to(p.useFlash);
	if (j.contains("attackFlashStartTime")) j.at("attackFlashStartTime").get_to(p.attackFlashStartTime);
	if (j.contains("slowTime")) j.at("slowTime").get_to(p.slowTime);
	if (j.contains("slowAmout")) j.at("slowAmout").get_to(p.slowAmout);
	if (j.contains("speedUpMotionSpeed")) j.at("speedUpMotionSpeed").get_to(p.speedUpMotionSpeed);
	if (j.contains("attackCollsionStartTime")) j.at("attackCollsionStartTime").get_to(p.attackCollsionStartTime);
	if (j.contains("attackCollsionEndTime")) j.at("attackCollsionEndTime").get_to(p.attackCollsionEndTime);
	if (j.contains("justAvoidCollsionStartTime")) j.at("justAvoidCollsionStartTime").get_to(p.justAvoidCollsionStartTime);
	if (j.contains("justAvoidCollsionEndTime")) j.at("justAvoidCollsionEndTime").get_to(p.justAvoidCollsionEndTime);
	if (j.contains("attackPositionFrameNum")) j.at("attackPositionFrameNum").get_to(p.attackPositionFrameNum);
	if (j.contains("attackSoundStartTime")) j.at("attackSoundStartTime").get_to(p.attackSoundStartTime);
	if (j.contains("attackCollTransform")) j.at("attackCollTransform").get_to(p.attackCollTransform);
	if (j.contains("justAvoidCollTransform")) j.at("justAvoidCollTransform").get_to(p.justAvoidCollTransform);

	if (j.contains("animID"))
	{
		std::string s;
		j.at("animID").get_to(s);
		p.animID = ID::StringToID(s);
		p.animFileName = s;
	}

	if (j.contains("attackBeforeAnimID"))
	{
		std::string s;
		j.at("attackBeforeAnimID").get_to(s);
		p.attackBeforeAnimID = ID::StringToID(s);
	}

	if (j.contains("hitDamage")) j.at("hitDamage").get_to(p.hitDamage);

	if (j.contains("damagePattern"))
	{
		std::string s;
		j.at("damagePattern").get_to(s);
		p.damagePattern = EnemyAttackBase::FromString(s);
	}

	if (j.contains("voiceName")) j.at("voiceName").get_to(p.voiceName);

	// =========================
	// 移動
	// =========================
	if (j.contains("frontMove"))
	{
		j.at("frontMove").get_to(p.frontMove);
		j.at("moveSpeed").get_to(p.moveSpeed);
	}

	// =========================
	// プレイヤー追従
	// =========================
	if (j.contains("playerAloowMove"))
	{
		j.at("playerAloowMove").get_to(p.playerAloowMove);

		if (p.playerAloowMove)
		{
			if (j.contains("baseSpeed")) j.at("baseSpeed").get_to(p.baseSpeed);
			if (j.contains("baseFirstSpeed")) j.at("baseFirstSpeed").get_to(p.baseFirstSpeed);
			if (j.contains("playerNearStop")) j.at("playerNearStop").get_to(p.playerNearStop);
			if (j.contains("playerNearAloowStop")) j.at("playerNearAloowStop").get_to(p.playerNearAloowStop);
			if (j.contains("addVelocity")) j.at("addVelocity").get_to(p.addVelocity);
			if (j.contains("playerBaseNear")) j.at("playerBaseNear").get_to(p.playerBaseNear);
			if (j.contains("maxMoveSpeed")) j.at("maxMoveSpeed").get_to(p.maxMoveSpeed);
			if (j.contains("minMoveSpeed")) j.at("minMoveSpeed").get_to(p.minMoveSpeed);
			if (j.contains("moveStartTime")) j.at("moveStartTime").get_to(p.moveStartTime);
			if (j.contains("moveFinishTime")) j.at("moveFinishTime").get_to(p.moveFinishTime);
		}
	}
	else p.playerAloowMove = false;

	// =========================
	// 突進
	// =========================
	if (j.contains("rushMove"))
	{
		j.at("rushMove").get_to(p.rushMove);

		if (p.rushMove)
		{
			std::string s;
			if (j.contains("rushAfterAnimID"))
			{
				j.at("rushAfterAnimID").get_to(s);
				p.rushAfterAnimID = ID::StringToID(s);
			}

			if (j.contains("rushAfterSpeed")) j.at("rushAfterSpeed").get_to(p.rushAfterSpeed);
			if (j.contains("rushSoundRightFoot")) j.at("rushSoundRightFoot").get_to(p.rushSoundRightFoot);
			if (j.contains("rushSoundLeftFoot")) j.at("rushSoundLeftFoot").get_to(p.rushSoundLeftFoot);
			if (j.contains("rushTime")) j.at("rushTime").get_to(p.rushTime);
			if (j.contains("rushColl")) j.at("rushColl").get_to(p.rushColl);
			if (j.contains("addRushCollScale")) j.at("addRushCollScale").get_to(p.addRushCollScale);
		}
	}
	else p.rushMove = false;

	// =========================
	// 回転
	// =========================
	if (j.contains("rotateMove")) j.at("rotateMove").get_to(p.rotateMove);
	if (j.contains("angleMoveAmout")) j.at("angleMoveAmout").get_to(p.angleMoveAmout);

	// =========================
	// ジャンプ
	// =========================
	if (j.contains("jump"))
	{
		j.at("jump").get_to(p.jump);

		if (p.jump)
		{
			if (j.contains("addGravity")) j.at("addGravity").get_to(p.addGravity);
			if (j.contains("jumpSpeed")) j.at("jumpSpeed").get_to(p.jumpSpeed);
			if (j.contains("jumpStartTime")) j.at("jumpStartTime").get_to(p.jumpStartTime);
			if (j.contains("groundEffectStartTime")) j.at("groundEffectStartTime").get_to(p.groundEffectStartTime);

			if (j.contains("jumpGroundEffect"))
			{
				int v;
				j.at("jumpGroundEffect").get_to(v);
				p.jumpGroundEffect = static_cast<Effect_ID::EFFECT_ID>(v);
			}

			if (j.contains("groundShakeCamera")) j.at("groundShakeCamera").get_to(p.groundShakeCamera);
			if (j.contains("groundShakeTime")) j.at("groundShakeTime").get_to(p.groundShakeTime);
		}
	}
	else p.jump = false;

	// =========================
	// 衝撃波
	// =========================
	if (j.contains("shockWave"))
	{
		j.at("shockWave").get_to(p.shockWave);

		if (p.shockWave)
		{
			int v;
			if (j.contains("shockMoveEffect"))
			{
				j.at("shockMoveEffect").get_to(v);
				p.shockMoveEffect = static_cast<Effect_ID::EFFECT_ID>(v);
			}

			if (j.contains("shockWaveSpeed")) j.at("shockWaveSpeed").get_to(p.shockWaveSpeed);
			if (j.contains("startRange")) j.at("startRange").get_to(p.startRange);
		}
	}
	else p.shockWave = false;

	// =========================
	// 見る
	// =========================
	if (j.contains("lookPlayer"))
	{
		j.at("lookPlayer").get_to(p.lookPlayer);

		if (p.lookPlayer)
		{
			if (j.contains("lookNum")) j.at("lookNum").get_to(p.lookNum);
			if (j.contains("lookMaxCounter")) j.at("lookMaxCounter").get_to(p.lookMaxCounter);
		}
	}
	else p.lookPlayer = false;

	// =========================
	// 投擲
	// =========================
	if (j.contains("throwObject"))
	{
		j.at("throwObject").get_to(p.throwObject);

		if (p.throwObject)
		{
			if (j.contains("throwAttackData")) j.at("throwAttackData").get_to(p.throwAttackData);
			if (j.contains("armThrow")) j.at("armThrow").get_to(p.armThrow);
			if (j.contains("throwObjectNum")) j.at("throwObjectNum").get_to(p.throwObjectNum);
			if (j.contains("objectApperaPosition")) j.at("objectApperaPosition").get_to(p.objectApperaPosition);
			if (j.contains("intervalTime")) j.at("intervalTime").get_to(p.intervalTime);
			if (j.contains("intervalTimeSub")) j.at("intervalTimeSub").get_to(p.intervalTimeSub);
			if (j.contains("maxIntervalTime")) j.at("maxIntervalTime").get_to(p.maxIntervalTime);
			if (j.contains("minIntervalTime")) j.at("minIntervalTime").get_to(p.minIntervalTime);
		}
	}
	else p.throwObject = false;

	// =========================
	// カメラ
	// =========================
	if (j.contains("attackCameraBossLook"))
		j.at("attackCameraBossLook").get_to(p.attackCameraBossLook);

	if (j.contains("cameraChangeSpeed"))
		j.at("cameraChangeSpeed").get_to(p.cameraChangeSpeed);

	// =========================
	// トレイル
	// =========================
	if (j.contains("useTrail"))
		j.at("useTrail").get_to(p.useTrail);

	if (j.contains("trailRightHand"))
		j.at("trailRightHand").get_to(p.trailRightHand);

	if(j.contains("soundEvent"))
		if (j.contains("soundEvent")) j.at("soundEvent").get_to(p.soundEvent);
}
