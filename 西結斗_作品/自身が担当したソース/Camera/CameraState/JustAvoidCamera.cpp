#include "JustAvoidCamera.h"
#include "../../Common/Easing.h"
#include "../../Component/Transform/Transform.h"
#include "../cameraInformation.h"
#include "../../Component/Shaker/Shaker.h"
#include "cameraStateManager.h"
#include "../camera.h"

JustAvoidCamera::JustAvoidCamera()
{
    Start();
    string          = Function::GetClassNameC<JustAvoidCamera>();
    zoomDistance    = VZero;
   // id = ID::P_ANIM_JUST_AVOID;
}

JustAvoidCamera::~JustAvoidCamera()
{
}

void JustAvoidCamera::Update()
{
    Camera* c = GetBase<Camera>();
    //ズーム処理
    if (timer >= 0.0f) {
        float t = 1.0f - timer / MAX_TIMER;
        //ジャスト回避の時にカメラを近づける
        VECTOR3 easedT   = Easing::EaseOut(c->defalutDistance, zoomDistance, t);
        c->currentDistance = easedT;
        timer           -= Time::DeltaTimeRate();
    }
    else {
        c->currentDistance = zoomDistance;
    }
  
    c->cameraComponent.camera->Follow();

    changeTimer -= Time::DeltaTimeRate();

    if (changeTimer <= 0.0f) {
        //ターゲットのオブジェクトが生きていたら追いかけObject
        if (c->cameraComponent.target.obj != nullptr) {
            c->cameraComponent.state->ChangeState(StateID::FOLLOW_CAMERA_S);
        }
        else {
            c->cameraComponent.state->ChangeState(StateID::FREE_CAMERA_S);
        }
       
    }

    c->target = c->target;

    c->cameraComponent.camera->CameraRotationSet();

}

void JustAvoidCamera::Draw()
{
}

void JustAvoidCamera::Start()
{
    timer           = MAX_TIMER;
    changeTimer     = CHANGE_STATE_TIMER;

    defalutDistance = VECTOR3(0.0f, 0.0f, -1500.0f);
    zoomDistance    = VECTOR3(0.0f,0.0f, -1000.0f);
    currentDistance = VECTOR3(0.0f, 0.0f, -1500.0f);
    speed           = 0.1f;
}

void JustAvoidCamera::Finish()
{
}