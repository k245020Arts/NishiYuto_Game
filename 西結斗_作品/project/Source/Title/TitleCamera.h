#pragma once
#include "../Camera/CameraBase.h"


//DxlibŠÖ”’u‚«êi•ª‚©‚ç‚È‚¢‚Ì‚ª‚ ‚Á‚½‚çƒŠƒ“ƒN‚É”ò‚ñ‚Å)
//https://dxlib.xsrv.jp/dxfunc.html

class TitleScene;
class TitleControl;

class TitleCamera : public CameraBase
{
public:
	TitleCamera();
	~TitleCamera();

	void Update()override;
	void Draw()override;

	void Start(BaseObject* _eObj)override;

	void PlayerSet(BaseObject* _pObj);

	void ImguiDraw()override;

private:
	TitleScene* title;
	TitleControl* titleCtrl;

	float cameraNear;
	float cameraFar;
	bool rockOn;

	int counter;

	void Update_Debug();
	void Update_Normal();
};
