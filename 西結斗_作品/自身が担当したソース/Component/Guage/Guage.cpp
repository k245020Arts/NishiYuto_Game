#include "Guage.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include "../../Camera/CameraManager.h"

Guage::Guage()
{
	edge				= nullptr;
	guage				= nullptr;
	barValueMax			= 0.0f;
	barValue			= nullptr;
	debugId				= 19;
	tag					= Function::GetClassNameC<Guage>();
	displayHp			= 0.0f;
	changeColorCounter	= 0.0f;
	screenMode			= false;
	addMode				= false;
	damageGuage			= nullptr;
    displayHpMain = 0.0f;
    displayHpDamage = 0.0f;

    damageDelayTimer = 0.0f;
    damageDelay = 1.2f;
    prevHp = 0.0f;
}

Guage::~Guage()
{
	edge		= nullptr;
	guage		= nullptr;
	barValue	= nullptr;
}

void Guage::Update()
{
    if (screenMode) {
        VECTOR3 worldPos = VECTOR3(
            obj->GetParent()->GetTransform()->position.x,
            obj->GetParent()->GetTransform()->position.y,
            obj->GetParent()->GetTransform()->position.z
        );

        VECTOR headPos = worldPos + plus;
        VECTOR3 screenPos = ConvWorldPosToScreenPos(headPos);

        obj->GetTransform()->position = screenPos;

        if (screenPos.z > 1.0f) {
            edge->SetDraw(false);
            guage->SetDraw(false);
        }
        else {
            edge->SetDraw(true);
            guage->SetDraw(true);
        }
        edge->SetPosition(obj->GetTransform()->position);
    }
    
    if (guage != nullptr) {

        float dt = Time::DeltaTimeRate();

        float speedMain = 10.0f; 
        float speedDamage = 5.0f; 

        
        if (*barValue < prevHp) {
            damageDelayTimer = 0.0f;
        }
        prevHp = *barValue;

        displayHpMain += (*barValue - displayHpMain) * speedMain * dt;
        displayHpMain = std::clamp(displayHpMain, 0.0f, barValueMax);

        
        damageDelayTimer += dt;

        if (damageDelayTimer >= damageDelay) {
            displayHpDamage += (displayHpMain - displayHpDamage) * speedDamage * dt;
        }

        // ”O‚Ì‚½‚ßƒNƒ‰ƒ“ƒv
        displayHpDamage = std::clamp(displayHpDamage, 0.0f, barValueMax);

        float amountMain = displayHpMain / barValueMax;
        guage->SetDrawImageSize(VECTOR2I(static_cast<int>(amountMain * guage->GetImageSize().x),guage->GetImageSize().y ));

        if (damageGuage != nullptr) {
            float amountDamage = displayHpDamage / barValueMax;
            damageGuage->SetDrawImageSize(VECTOR2I(static_cast<int>(amountDamage * damageGuage->GetImageSize().x),damageGuage->GetImageSize().y));
        }
    }
}

void Guage::Draw()
{
}

void Guage::EdgeDrawReady(int _image, MeshRenderer2D::GraphMode _mode, Transform _transfrom)
{
	edge = obj->Component()->AddComponent<MeshRenderer2D>();
	edge->TextureHandle(_image, _mode);
	edge->SetTransform(_transfrom);
}

void Guage::DamageGuageDrawReady(int _image, MeshRenderer2D::GraphMode _mode,Transform _transform)
{
	damageGuage = obj->Component()->AddComponent<MeshRenderer2D>();
	damageGuage->TextureHandle(_image, _mode);
	Transform transfrom = _transform;
	damageGuage->SetTransform(transfrom);
}

void Guage::WorldToScreenMode(bool _mode, VECTOR3 _plusPos)
{
	screenMode	= _mode;
	plus		= _plusPos;
}
