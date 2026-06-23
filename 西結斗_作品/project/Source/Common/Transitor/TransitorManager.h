#pragma once
#include "../../../Library/GameObject.h"
#include "Transitor.h"

class TransitorManager : public GameObject
{
public:
	TransitorManager();
	~TransitorManager();
	/// <summary>
	/// ê–Ê‘JˆÚ’†‚©”»’è‚ğ‚·‚é
	/// </summary>
	/// <returns>true‚È‚ç‘JˆÚ’†,false‚È‚ç‘JˆÚ‚µ‚Ä‚¢‚È‚¢</returns>
	bool IsTransitor();

private:
	std::list<Transitor*> list;
};