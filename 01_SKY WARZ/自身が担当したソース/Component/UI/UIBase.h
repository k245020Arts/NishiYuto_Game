#pragma once
#include "../Component.h"
#include "../../Common/ResourceLoader.h"

class UIManager;

class UIBase : public Component
{
public:
	UIBase();
	virtual ~UIBase();

	void Awake()override;

private:
	UIManager* uiManager;
};