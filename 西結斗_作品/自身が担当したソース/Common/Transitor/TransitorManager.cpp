#include "TransitorManager.h"
#include "FadeTransitor.h"
#include "PushTransitor.h"
#include "WipeTransitor.h"
#include "StripTransitor.h"
#include "TitleTransitor.h"
#include "IrisTransitor.h"

TransitorManager::TransitorManager()
{
	FadeTransitor* fade = new FadeTransitor();
	PushTransitor* push = new PushTransitor();
	WipeTransitor* wipe = new WipeTransitor();
	StripTransitor* strip = new StripTransitor();
	TitleTransitor* title = new TitleTransitor();
	IrisTransitor* iris = new IrisTransitor();
	//ÅŒã‚Ì•û‚É•`‰æ‚³‚¹‚½‚¢‚Ì‚Å‹É’[‚É¬‚³‚È’l‚É‚µ‚Ä‚¢‚é
	fade->SetDrawOrder(-5000);
	push->SetDrawOrder(-5000);
	wipe->SetDrawOrder(-5000);
	strip->SetDrawOrder(-5000);
	title->SetDrawOrder(-5000);
	iris->SetDrawOrder(-5000);

	list.emplace_back(fade);
	list.emplace_back(push);
	list.emplace_back(wipe);
	list.emplace_back(strip);
	list.emplace_back(title);
	list.emplace_back(iris);

	DontDestroyOnSceneChange();
}

TransitorManager::~TransitorManager()
{
	for (Transitor* transitor : list) {
		transitor->DestroyMe();
	}
	list.clear();
}

bool TransitorManager::IsTransitor()
{
	for (Transitor* t : list) {
		if (!t->IsEnd()) {
			return false;
		}
	}
    return true;
}
