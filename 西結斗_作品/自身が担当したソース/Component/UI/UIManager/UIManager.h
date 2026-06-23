#pragma once
#include "../../../../Library/GameObject.h"

class BaseObject;

class UIManager : public GameObject
{
public:
	UIManager();
	~UIManager();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// ボタンUIをセットする
	/// </summary>
	void ButtonUISet();
	/// <summary>
	/// UIのリストに追加
	/// </summary>
	/// <param name="_base"></param>
	void UIPush(BaseObject* _base);
	/// <summary>
	/// UIのリストから削除
	/// </summary>
	/// <param name="_base"></param>
	void UIDelete(BaseObject* _base);
	/// <summary>
	/// UIすべてをdrawするかしないかを判定
	/// </summary>
	/// <param name="_draw">trueなら描画する</param>
	void SetUIDraw(bool _draw) { draw = _draw; }

private:
	std::list<BaseObject*> uiObjects;
	bool draw;
};
