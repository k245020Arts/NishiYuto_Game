#pragma once
#include "../Component/Component.h"

class Transform;
class EffectManager;

class Wall : public Component
{
public:
	Wall();
	~Wall();

	void Update()override;
	void Draw()override;
	/// <summary>
	///壁に使われるモデルのセット
	/// </summary>
	/// <param name="_modelHandle"></param>
	void ModelSet(int _modelHandle);

	void ImguiDraw()override;

private:
	int model;
	Transform* pTransform;
	Transform* transform;
	//EffectManager* effect;
	int count;
};