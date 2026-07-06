#include "TextRenderer.h"
#include "../Transform/Transform.h"
#include "../Shaker/Shaker.h"
#include "../ComponentManager.h"
#include "UIManager/UIManager.h"

TextRenderer::TextRenderer()
{
	//position = VECTOR2F(0.0f,0.0f);
	fontHandle = -1;
	text = "";
	rgb = Color::Rgb();
	//scale = VECTOR2F(1.0f,1.0f);
	debugId = 36;
	tag = Function::GetClassNameC<TextRenderer>();
}

TextRenderer::~TextRenderer()
{

}

void TextRenderer::Update()
{

}

void TextRenderer::Draw()
{
	VECTOR3 position = obj->GetTransform()->position;
	//描画専用のポジジョンを作る
	if (shaker != nullptr) {
		//ヒットストップなどでシェイクしているときは描画用の座標だけを動かしている
		position += shaker->GetShakePower();
	}
	VECTOR3 scale = obj->GetTransform()->scale;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)rgb.a);
    DrawExtendFormatStringToHandle((int)position.x,(int)position.y,(double)scale.x,(double)scale.y,GetColor((int)rgb.r,(int)rgb.g,(int)rgb.b),fontHandle,"%s",text.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
}


void TextRenderer::TextSetting(std::string _text, std::string _fileName, std::string _exten, const Color::Rgb& _rgb,int _size, Font_ID::Font_ID _id)
{
	text = _text;
	//position = _pos;
	fontHandle = ResourceLoad::LoadFont(_fileName.c_str(), _exten, _id, _size);
	rgb = _rgb;
	//scale = _scale;
	shaker = obj->Component()->GetComponent<Shaker>();
}

int TextRenderer::GetTextWidth()
{
	return GetDrawExtendFormatStringWidthToHandle(obj->GetTransform()->scale.x, fontHandle, text.c_str());
}

void TextRenderer::UseDrawUI()
{
	uiManager = FindGameObject<UIManager>();
	if (uiManager == nullptr) {
		return; //UiManagerがnullptrならリターン
	}
	uiManager->UIPush(obj);//リストに登録
	obj->SetDraw(false);
}

void TextRenderer::SetScale(const VECTOR3& _scale)
{
	obj->GetTransform()->scale = _scale;
}

void TextRenderer::SetPos(const VECTOR3& _pos)
{
	obj->GetTransform()->position = _pos;
}
