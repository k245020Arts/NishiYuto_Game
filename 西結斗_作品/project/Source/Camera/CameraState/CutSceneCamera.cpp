#include "CutSceneCamera.h"
#include "../../Common/Easing.h"
#include "../../Component/Transform/Transform.h"
#include "../cameraInformation.h"
#include "../../Component/Shaker/Shaker.h"
#include "cameraStateManager.h"
#include "../camera.h"
#include "../CameraEditorGui.h"
#include "../CutSceneBox.h"
#include "../../Component/UI/UIManager/UIManager.h"

CutSceneCamera::CutSceneCamera()
{
    
    time = 0.0f;
    first = false;

    position = CutSceneObj();
    target = CutSceneObj();

    firstPos = VZero;
    firstTarget = VZero;

    beforePosName = "";
    beforeTargetName = "";

    playerFirstCaptured = false;
    enemyFirstCaptured = false;
    playerFirstTargetCaptured = false;
    enemyFirstTargetCaptured = false;

    playerFirstPos = VZero;
    playerFirstRot = MGetIdent();
    enemyFirstPos = VZero;
    enemyFirstRot = MGetIdent();

    playerLastPos = VZero;
    playerLastRot = MGetIdent();
    enemyLastPos = VZero;
    enemyLastRot = MGetIdent();

   

    playerFirstTargetPos = VZero;
    playerFirstTargetRot = MGetIdent();
    enemyFirstTargetPos = VZero;
    enemyFirstTargetRot = MGetIdent();
    playerLastTargetPos = VZero;
    playerLastTargetRot = MGetIdent();
    enemyLastTargetPos = VZero;
    enemyLastTargetRot = MGetIdent();

}

CutSceneCamera::~CutSceneCamera()
{
}


void CutSceneCamera::Update()
{
    Camera* camera = GetBase<Camera>();
    if (camera->cutSceneData.empty()) return;

    CutSceneSpece::CutScene& cut = camera->cutSceneData[camera->cutSceneIndex];

    // 対象切り替え判定 
    bool isTransition = (cut.followPosName != beforePosName);
    bool isTargetTransition = (cut.followPosTarget != beforeTargetName);

    //線形補完
    time -= Time::DeltaTimeRate();
    float t = std::clamp(time / cut.duration, 0.0f, 1.0f);

    Transform* posBase = PlayerEnemyWorldToPos(cut.followPosName);

    if (posBase != nullptr)
    {
        // 最初の位置の更新（前のターゲットと違う場合のみ）
        if (cut.followPosName != beforePosName)
        {
            if (cut.followPosName == PLAYER_POS_NAME || cut.followPosName == PLAYER_FIRST_POS_NAME)
            {
                playerFirstPos = posBase->position;
                playerFirstRot = posBase->GetRotationMatrix();
            }
            else if (cut.followPosName == ENEMY_POS_NAME || cut.followPosName == ENEMY_FIRST_POS_NAME)
            {
                enemyFirstPos = posBase->position;
                enemyFirstRot = posBase->GetRotationMatrix();
            }

            beforePosName = cut.followPosName; // 直前オブジェクト更新
        }

        //最後の位置はは常に更新
        if (cut.followPosName == PLAYER_POS_NAME || cut.followPosName == PLAYER_FIRST_POS_NAME)
        {
            playerLastPos = posBase->position;
            playerLastRot = posBase->GetRotationMatrix();
        }
        else if (cut.followPosName == ENEMY_POS_NAME || cut.followPosName == ENEMY_FIRST_POS_NAME)
        {
            enemyLastPos = posBase->position;
            enemyLastRot = posBase->GetRotationMatrix();
        }
    }

    // 基準決定
    VECTOR3 basePos = VZero;
    MATRIX  baseRot = MGetIdent();

    if (cut.followPosName == PLAYER_FIRST_POS_NAME)
    {
        basePos = playerFirstPos;
        baseRot = playerFirstRot;
    }
    else if (cut.followPosName == ENEMY_FIRST_POS_NAME)
    {
        basePos = enemyFirstPos;
        baseRot = enemyFirstRot;
    }
    else if (cut.followPosName == PLAYER_POS_NAME)
    {
        basePos = playerLastPos;
        baseRot = playerLastRot;
    }
    else if (cut.followPosName == ENEMY_POS_NAME)
    {
        basePos = enemyLastPos;
        baseRot = enemyLastRot;
    }

    VECTOR3 endPos = basePos + cut.camera.endPos * baseRot;

    VECTOR3 movePos = VZero;

   

    if (!isTransition && first)
    {
        // 追従開始時に一度だけ取る
        firstPos = camera->cameraComponent.cameraTransform->position - basePos;
        
    }

    if (isTransition)
    {
        //対象となる敵のポジションも含み補完
        switch (cut.ease)
        {
        case CutSceneSpece::EaseType::Linear:
            movePos = Easing::Lerp(endPos, firstPos, t);
            break;
        case CutSceneSpece::EaseType::In:
            movePos = Easing::EaseIn(endPos, firstPos, t);
            break;
        case CutSceneSpece::EaseType::Out:
            movePos = Easing::EaseOut(endPos, firstPos, t);
            break;
        case CutSceneSpece::EaseType::InOut:
            movePos = Easing::EaseInOut(endPos, firstPos, t);
            break;
        }
    }
    else
    {
        // 追従の時はoffsetのみ補間 
        VECTOR3 currentPos =
            camera->cameraComponent.cameraTransform->position;

        VECTOR3 currentOffset = currentPos - basePos;
        VECTOR3 targetOffset = cut.camera.endPos * baseRot;

        VECTOR3 offset = VZero;

        switch (cut.ease)
        {
        case CutSceneSpece::EaseType::Linear:
            offset = Easing::Lerp(targetOffset, firstPos, t);
            break;
        case CutSceneSpece::EaseType::In:
            offset = Easing::EaseIn(targetOffset, firstPos, t);
            break;
        case CutSceneSpece::EaseType::Out:
            offset = Easing::EaseOut(targetOffset, firstPos, t);
            break;
        case CutSceneSpece::EaseType::InOut:
            offset = Easing::EaseInOut(targetOffset, firstTarget, t);
            break;
        }

        movePos = basePos + offset;
    }

    camera->cameraComponent.cameraTransform->position = movePos;
    camera->cameraComponent.cameraTransform->position += camera->cameraComponent.shaker->GetShakePower();
   
    // ターゲット
    Transform* targetBase = PlayerEnemyWorldToPos(cut.followPosTarget);

    if (targetBase != nullptr)
    {
        // 最初の位置の更新（前のターゲットと違う場合のみ）
        if (cut.followPosTarget != beforeTargetName)
        {
            if (cut.followPosTarget == PLAYER_POS_NAME || cut.followPosTarget == PLAYER_FIRST_POS_NAME)
            {
                playerFirstTargetPos = targetBase->position;
                playerFirstTargetRot = targetBase->GetRotationMatrix();
            }
            else if (cut.followPosTarget == ENEMY_POS_NAME || cut.followPosTarget == ENEMY_FIRST_POS_NAME)
            {
                enemyFirstTargetPos = targetBase->position;
                enemyFirstTargetRot = targetBase->GetRotationMatrix();
            }

            beforeTargetName = cut.followPosTarget; // 直前ターゲット更新
        }

        //最後の位置はは常に更新
        if (cut.followPosTarget == PLAYER_POS_NAME || cut.followPosTarget == PLAYER_FIRST_POS_NAME)
        {
            playerLastTargetPos = targetBase->position;
            playerLastTargetRot = targetBase->GetRotationMatrix();
        }
        else if (cut.followPosTarget == ENEMY_POS_NAME || cut.followPosTarget == ENEMY_FIRST_POS_NAME)
        {
            enemyLastTargetPos = targetBase->position;
            enemyLastTargetRot = targetBase->GetRotationMatrix();
        }
    }

    // 基準決定
    VECTOR3 targetBasePos = VZero;
    MATRIX  targetBaseRot = MGetIdent();

    if (cut.followPosTarget == PLAYER_FIRST_POS_NAME)
    {
        targetBasePos = playerFirstTargetPos;
        targetBaseRot = playerFirstTargetRot;
    }
    else if (cut.followPosTarget == ENEMY_FIRST_POS_NAME)
    {
        targetBasePos = enemyFirstTargetPos;
        targetBaseRot = enemyFirstTargetRot;
    }
    else if (cut.followPosTarget == PLAYER_POS_NAME)
    {
        targetBasePos = playerLastTargetPos;
        targetBaseRot = playerLastTargetRot;
    }
    else if (cut.followPosTarget == ENEMY_POS_NAME)
    {
        targetBasePos = enemyLastTargetPos;
        targetBaseRot = enemyLastTargetRot;
    }

    VECTOR3 endTarget = targetBasePos + cut.camera.target * targetBaseRot;
    VECTOR3 moveTarget = VZero;

  

    if (!isTransition && first)
    {
        // 追従開始時に一度だけ取る
        firstTarget = camera->target - targetBasePos;
        first = false;
    }

    if (isTargetTransition)
    {
        //対象となる敵のポジションも含み補完
        switch (cut.ease)
        {
        case CutSceneSpece::EaseType::Linear:
            moveTarget = Easing::Lerp(endTarget, firstTarget, t);
            break;
        case CutSceneSpece::EaseType::In:
            moveTarget = Easing::EaseIn(endTarget, firstTarget, t);
            break;
        case CutSceneSpece::EaseType::Out:
            moveTarget = Easing::EaseOut(endTarget, firstTarget, t);
            break;
        case CutSceneSpece::EaseType::InOut:
            moveTarget = Easing::EaseInOut(endTarget, firstTarget, t);
            break;
        }
    }
    else
    {
        // 追従の時はoffsetのみ補間 
        VECTOR3 currentTarget = camera->target;

        VECTOR3 currentOffset = currentTarget - targetBasePos;
        VECTOR3 targetOffset = cut.camera.target * targetBaseRot;

        VECTOR3 offset = VZero;

        switch (cut.ease)
        {
        case CutSceneSpece::EaseType::Linear:
            offset = Easing::Lerp(targetOffset, firstTarget, t);
            break;
        case CutSceneSpece::EaseType::In:
            offset = Easing::EaseIn(targetOffset, firstTarget, t);
            break;
        case CutSceneSpece::EaseType::Out:
            offset = Easing::EaseOut(targetOffset, firstTarget, t);
            break;
        case CutSceneSpece::EaseType::InOut:
            offset = Easing::EaseInOut(targetOffset, firstTarget, t);
            break;
        }

        moveTarget = targetBasePos + offset;
    }

    camera->target = moveTarget;
    camera->CameraRotationSet();

    //VECTOR3 pos = camera->cameraComponent.cameraTransform->position;
    //VECTOR3 tgt = camera->target;

    //VECTOR3 forward = VNorm(tgt - pos);
    //VECTOR3 up = VGet(0, 1, 0);

    //// forward と up が平行に近い場合の保険
    //if (fabsf(VDot(forward, up)) > 0.99f)
    //{
    //    up = VGet(0, 0, 1);
    //}

    //VECTOR3 right = VNorm(VCross(up, forward));
    //up = VCross(forward, right);

    //MATRIX rot = MGetIdent();

    //rot.m[0][0] = right.x;
    //rot.m[0][1] = right.y;
    //rot.m[0][2] = right.z;

    //rot.m[1][0] = up.x;
    //rot.m[1][1] = up.y;
    //rot.m[1][2] = up.z;

    //rot.m[2][0] = forward.x;
    //rot.m[2][1] = forward.y;
    //rot.m[2][2] = forward.z;

    //camera->cameraComponent.cameraTransform->SetRotationMatrix(rot);

    if (t <= 0.0f)
    {
        camera->cutSceneIndex++;

        //すべてのカットが終わったらStateの切り替え
        if (camera->cutSceneIndex >= camera->cutSceneData.size())
        {
            camera->cameraComponent.state->ChangeState(StateID::FREE_CAMERA_S);
            camera->cutSceneIndex = -1;
            return;
        }

        time = camera->cutSceneData[camera->cutSceneIndex].duration;
        first = true;

        firstPos = camera->cameraComponent.cameraTransform->position;
        firstTarget = camera->target;
    }
}

void CutSceneCamera::Draw()
{
}


void CutSceneCamera::Start()
{
    Camera* camera = GetBase<Camera>();
    if (camera->cutSceneData.empty()) { //カットシーンのデータがなければスルー
        return;
    }
    //0カット目から流す
    camera->cutSceneIndex = 0;
    time = camera->cutSceneData[0].duration;
    first = true;
    camera->rockOn = false;

    position = CutSceneObj();
    target = CutSceneObj();

    playerFirstCaptured = false;
    enemyFirstCaptured = false;
    playerFirstTargetCaptured = false;
    enemyFirstTargetCaptured = false;

    camera->isCutScene = true;

    beforePosName = "";
    beforeTargetName = "";

    Transform* startTransform = PlayerEnemyWorldToPos(camera->cutSceneData[camera->cutSceneIndex].firstPosBaseName);
    //最初に指定するTransformがあったら
    if (startTransform != nullptr) {
        firstPos = camera->cutSceneData[camera->cutSceneIndex].camera.startPos * startTransform->GetRotationMatrix() + startTransform->position;;
    }
   else {
        Transform* st = PlayerEnemyWorldToPos(camera->cutSceneData[camera->cutSceneIndex].followPosName);
       if (st != nullptr) {
            firstPos = camera->cutSceneData[camera->cutSceneIndex].camera.startPos * st->GetRotationMatrix() + st->position;;
       }
       else {
           firstPos = camera->cutSceneData[camera->cutSceneIndex].camera.startPos;
       }
              
    }
    firstTarget = camera->target;
}

void CutSceneCamera::Finish()
{
    Camera* camera = GetBase<Camera>();
    camera->isCutScene = false;
    camera->cutSceneIndex = -1;
    //カットシーン用のboxを描画していたらそれをしまう
    if (camera->cutSceneBoxDraw) {
        camera->cutSceneBox->FinishBox(1.0f, 0x00000, Easing::EaseInOut<int>);
        camera->uiManager->SetUIDraw(true);
    }
    
}

Transform* CutSceneCamera::PlayerEnemyWorldToPos(std::string _name)
{
    Camera* camera = GetBase<Camera>();
    
    //プレイヤー関連の名前がついていたらプレイヤーのTransformを取得
    if (_name == PLAYER_POS_NAME || _name == PLAYER_FIRST_POS_NAME)
        return camera->cameraComponent.player.transform;

    //敵関連の名前がついていたら敵のTransformを取得
    if (_name == ENEMY_POS_NAME || _name == ENEMY_FIRST_POS_NAME)
        return camera->cameraComponent.target.transform;

    return nullptr;
}

void CutSceneCamera::StateImguiDraw()
{
    Camera* camera = GetBase<Camera>();
    ImGui::Text("CutScene Index : %d", camera->cutSceneIndex);
}
//#include "CutSceneCamera.h"
//#include "../../Common/Easing.h"
//#include "../../Component/Transform/Transform.h"
//#include "../cameraInformation.h"
//#include "../../Component/Shaker/Shaker.h"
//#include "cameraStateManager.h"
//#include "../camera.h"
//#include "../CameraEditorGui.h"
//
//CutSceneCamera::CutSceneCamera()
//{
//    cutSceneIndex = 0;
//    time = 0.0f;
//    first = false;
//
//    position = CutSceneObj();
//    target = CutSceneObj();
//
//    firstPos = VZero;
//    firstTarget = VZero;
//
//    beforePosName = "";
//    beforeTargetName = "";
//}
//
//CutSceneCamera::~CutSceneCamera()
//{
//}
//
//
//void CutSceneCamera::Update()
//{
//    Camera* camera = GetBase<Camera>();
//
//    if (camera->cutSceneData.empty()) return;
//
//    CutSceneSpece::CutScene& cut = camera->cutSceneData[cutSceneIndex];
//
//    // 時間補間
//    time -= Time::DeltaTimeRate();
//    float t = std::clamp(time / cut.duration, 0.0f, 1.0f);
//
//    Transform* posBase = PlayerEnemyWorldToPos(cut.followPosName);
//
//    if (posBase != nullptr)
//    {
//        CutSceneInfoSet(position, posBase, beforePosName);
//    }
//
//    VECTOR3 endPos = GetEndTarget(position, cut.camera.endPos);
//
//    VECTOR3 movePos = VZero;
//    switch (cut.ease)
//    {
//    case CutSceneSpece::EaseType::Linear:
//        movePos = Easing::Lerp(endPos, firstPos, t);
//        break;
//
//    case CutSceneSpece::EaseType::In:
//        movePos = Easing::EaseIn(endPos, firstPos, t);
//        break;
//
//    case CutSceneSpece::EaseType::Out:
//        movePos = Easing::EaseOut(endPos, firstPos, t);
//        break;
//
//    case CutSceneSpece::EaseType::InOut:
//        movePos = Easing::EaseInOut(endPos, firstPos, t);
//        break;
//    }
//
//    camera->cameraComponent.cameraTransform->position = movePos;
//
//
//    // ターゲット
//    Transform* targetBase = PlayerEnemyWorldToPos(cut.followPosTarget);
//
//    if (targetBase != nullptr)
//    {
//        CutSceneInfoSet(target, targetBase, beforeTargetName);
//    }
//
//    VECTOR3 endTarget = GetEndTarget(target, cut.camera.target);
//
//    VECTOR3 moveTarget = VZero;
//    switch (cut.ease)
//    {
//    case CutSceneSpece::EaseType::Linear:
//        moveTarget = Easing::Lerp(endTarget, firstTarget, t);
//        break;
//
//    case CutSceneSpece::EaseType::In:
//        moveTarget = Easing::EaseIn(endTarget, firstTarget, t);
//        break;
//
//    case CutSceneSpece::EaseType::Out:
//        moveTarget = Easing::EaseOut(endTarget, firstTarget, t);
//        break;
//
//    case CutSceneSpece::EaseType::InOut:
//        moveTarget = Easing::EaseInOut(endTarget, firstTarget, t);
//        break;
//    }
//
//    camera->target = moveTarget;
//    camera->CameraRotationSet();
//
//    if (t <= 0.0f)
//    {
//        cutSceneIndex++;
//
//        if (cutSceneIndex >= camera->cutSceneData.size())
//        {
//            camera->cameraComponent.state->ChangeState(StateID::FREE_CAMERA_S);
//            cutSceneIndex = 0;
//            return;
//        }
//
//        time = camera->cutSceneData[cutSceneIndex].duration;
//        first = true;
//
//        firstPos = camera->cameraComponent.cameraTransform->position;
//        firstTarget = camera->target;
//    }
//}
//
//void CutSceneCamera::Draw()
//{
//}
//
//
//void CutSceneCamera::Start()
//{
//    Camera* camera = GetBase<Camera>();
//    if (camera->cutSceneData.empty()) return;
//
//    cutSceneIndex = 0;
//    time = camera->cutSceneData[0].duration;
//    first = true;
//
//    position.player.firstSet = false;
//    position.enemy.firstSet = false;
//    target.player.firstSet = false;
//    target.enemy.firstSet = false;
//
//    camera->isCutScene = true;
//
//    beforePosName = "";
//    beforeTargetName = "";
//
//    Transform* startTransform = PlayerEnemyWorldToPos(camera->cutSceneData[cutSceneIndex].firstPosBaseName);
//    if (startTransform != nullptr) {
//        firstPos = camera->cutSceneData[cutSceneIndex].camera.startPos * startTransform->GetRotationMatrix() + startTransform->position;;
//    }
//    else {
//        Transform* st = PlayerEnemyWorldToPos(camera->cutSceneData[cutSceneIndex].followPosName);
//        if (st != nullptr) {
//            firstPos = camera->cutSceneData[cutSceneIndex].camera.startPos * st->GetRotationMatrix() + st->position;;
//        }
//        else {
//            firstPos = camera->cutSceneData[cutSceneIndex].camera.startPos;
//        }
//
//    }
//    firstTarget = camera->target;
//}
//
//void CutSceneCamera::Finish()
//{
//    Camera* camera = GetBase<Camera>();
//    camera->isCutScene = false;
//}
//
//Transform* CutSceneCamera::PlayerEnemyWorldToPos(std::string _name)
//{
//    Camera* camera = GetBase<Camera>();
//
//    if (_name == PLAYER_POS_NAME || _name == PLAYER_FIRST_POS_NAME)
//        return camera->cameraComponent.player.transform;
//
//    if (_name == ENEMY_POS_NAME || _name == ENEMY_FIRST_POS_NAME)
//        return camera->cameraComponent.target.transform;
//
//    return nullptr;
//}
//
//VECTOR3 CutSceneCamera::CutSceneInfoSet(CutSceneObj& _info, Transform* _baseTransform, std::string& _name)
//{
//    Camera* camera = GetBase<Camera>();
//
//    CutSceneSpece::CutScene& cut = camera->cutSceneData[cutSceneIndex];
//    // 最初の位置の更新（前のターゲットと違う場合のみ）
//    if (cut.followPosName != beforePosName)
//    {
//        if (cut.followPosName == PLAYER_POS_NAME || cut.followPosName == PLAYER_FIRST_POS_NAME)
//        {
//            _info.player.firstPos = _baseTransform->position;
//            _info.player.FirstRot = _baseTransform->GetRotationMatrix();
//        }
//        else if (cut.followPosName == ENEMY_POS_NAME || cut.followPosName == ENEMY_FIRST_POS_NAME)
//        {
//            _info.enemy.firstPos = _baseTransform->position;
//            _info.enemy.FirstRot = _baseTransform->GetRotationMatrix();
//        }
//
//        _name = cut.followPosName; // 直前オブジェクト更新
//    }
//
//    //最後の位置はは常に更新
//    if (cut.followPosName == PLAYER_POS_NAME || cut.followPosName == PLAYER_FIRST_POS_NAME)
//    {
//        _info.player.lastPos = _baseTransform->position;
//        _info.player.lastRot = _baseTransform->GetRotationMatrix();
//    }
//    else if (cut.followPosName == ENEMY_POS_NAME || cut.followPosName == ENEMY_FIRST_POS_NAME)
//    {
//        _info.enemy.lastPos = _baseTransform->position;
//        _info.enemy.lastRot = _baseTransform->GetRotationMatrix();
//    }
//    return VECTOR3();
//}
//
//VECTOR3 CutSceneCamera::GetEndTarget(const CutSceneObj& _info, const VECTOR3& _offset)
//{
//    Camera* camera = GetBase<Camera>();
//
//    CutSceneSpece::CutScene& cut = camera->cutSceneData[cutSceneIndex];
//    // 基準決定
//    VECTOR3 targetBasePos = VZero;
//    MATRIX  targetBaseRot = MGetIdent();
//
//    if (cut.followPosTarget == PLAYER_FIRST_POS_NAME)
//    {
//        targetBasePos = _info.player.firstPos;
//        targetBaseRot = _info.player.FirstRot;
//    }
//    else if (cut.followPosTarget == ENEMY_FIRST_POS_NAME)
//    {
//        targetBasePos = _info.enemy.firstPos;
//        targetBaseRot = _info.enemy.FirstRot;
//    }
//    else if (cut.followPosTarget == PLAYER_POS_NAME)
//    {
//        targetBasePos = _info.player.lastPos;
//        targetBaseRot = _info.player.lastRot;
//    }
//    else if (cut.followPosTarget == ENEMY_POS_NAME)
//    {
//        targetBasePos = _info.enemy.lastPos;
//        targetBaseRot = _info.enemy.lastRot;
//    }
//
//    VECTOR3 endTarget = targetBasePos + _offset * targetBaseRot;
//    return endTarget;
//}
//
//void CutSceneCamera::StateImguiDraw()
//{
//    ImGui::Text("CutScene Index : %d", cutSceneIndex);
//}
