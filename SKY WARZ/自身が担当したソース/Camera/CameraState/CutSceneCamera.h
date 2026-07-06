#pragma once

#include <string>
#include "cameraStateBase.h"

class Transform;

class CutSceneCamera : public CameraStateBase
{
public:
    CutSceneCamera();
    ~CutSceneCamera();

    void Update() override;
    void Draw() override;
    void Start() override;
    void Finish() override;

    void StateImguiDraw() override;

   

private:
    Transform* PlayerEnemyWorldToPos(std::string _name);
    struct CutPositionInformation
    {
        CutPositionInformation() {
            firstPos = VZero;
            FirstRot = MGetIdent();
            lastPos = VZero;
            lastRot = MGetIdent();
            firstSet = false;
        }
        VECTOR3 firstPos;
        MATRIX FirstRot;
        VECTOR3 lastPos;
        MATRIX lastRot;
        bool firstSet;

    };

    struct CutSceneObj
    {
        CutSceneObj() {
            player = CutPositionInformation();
            enemy = CutPositionInformation();
        }
        CutPositionInformation player;
        CutPositionInformation enemy;
    };
    
    float time;
    bool  first;

    CutSceneObj position; //カメラのポジションを設定するために必要な構造体

    CutSceneObj target; // カメラの注視点を設定するために必要な構造体

    /*VECTOR3 CutSceneInfoSet(CutSceneObj& _info, Transform* _baseTransform,std::string& _name);
    VECTOR3 GetEndTarget(const CutSceneObj& _info,const VECTOR3& _offset);*/

    std::string beforePosName;
    std::string beforeTargetName;

    
    VECTOR3 firstPos;
    VECTOR3 firstTarget;

    //TODOこれを構造体にする、構造体にしてすっきりさせてはいるが挙動がおかしくなっている状態
    VECTOR3 playerFirstPos;
    MATRIX  playerFirstRot;
    VECTOR3 enemyFirstPos;
    MATRIX  enemyFirstRot;

    VECTOR3 playerLastPos;
    MATRIX  playerLastRot;
    VECTOR3 enemyLastPos;
    MATRIX  enemyLastRot;

    bool playerFirstCaptured;
    bool enemyFirstCaptured;

    VECTOR3 playerFirstTargetPos;
    MATRIX  playerFirstTargetRot;
    VECTOR3 enemyFirstTargetPos;
    MATRIX  enemyFirstTargetRot;

    VECTOR3 playerLastTargetPos;
    MATRIX  playerLastTargetRot;
    VECTOR3 enemyLastTargetPos;
    MATRIX  enemyLastTargetRot;

    bool playerFirstTargetCaptured;
    bool enemyFirstTargetCaptured;
};