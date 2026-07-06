#include "ID.h"
#include <unordered_map>
#include <assert.h>
#include "../Debug/Debug.h"

namespace {
	std::unordered_map<ID::IDType,std::string>* modelId;

	/*std::string InitID[ID::ID_MAX] = {
		"enemy",
		"player",
		"stage",
		"p_idol",
		"p_run",
		"p_avoid",
		"p_just_avoid",
		"p_attack1",
		"p_just_avoid_attack1",
		"p_attack2",
		"p_just_avoid_attack2",
		"p_attack3",
		"p_just_avoid_attack3",
		"p_attack4",
		"p_just_avoid_attack4",
		"p_just_avoid_attack5",
		"p_attack5",
		"p_damage",
		"p_blowAway",
		"p_fall",
		"p_getUp",
		"p_weapon",
		"e_idol",
		"e_attack",
		"e_damage",
		"e_blowaway",
		"e_fall",
		"e_getUp",
		"e_weapon",
		"c_follow",
		"c_avoid_attack",
		"e_run",
		"c_hit",
		"sky",
		"swordLoad",
		"wall",
		"p_hp_guage",
		"e_die",
		"p_die",
		"start_num",
		"b_model",
		"c_free",
		"b_idol",
		"b_run",
		"b_coolTime",
		"b_attackSorting",
		"b_n_attack1",
		"b_n_attack2",
		"b_n_attack3",
		"b_n_attack4",
		"b_n_attack5",
		"b_s_attack1",
		"b_s_attack2",
		"b_s_attack3",
		"b_s_attack4",
		"sword_effect_b",
		"bossPush",
		"boss_die",
		"hp_edge",
		"boss_hp_edge",
		"title_back",
		"title",
		"push_button",
		"win",
		"lose",
		"b_s_attack2Stop",
		"b_s_attack2Before",
		"b_s_attack1small",
		"te_idol",
		"te_run",
		"te_attack",
		"shadow",
	};*/
}

void ID::Init() {

	/*for (int i = 0; i < ID_MAX; i++) {
		modelId[i] = InitID[i];
	}*/
	modelId = new std::unordered_map<ID::IDType, std::string>;
}

void ID::DeleteID()
{
	modelId->clear();
	delete modelId;
}

std::string ID::GetID(IDType _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)ID_MAX) {
		Debug::CreateMessageBox("modelHandleMiss", "miss");
	}
	return (*modelId)[_type];
}

void ID::SetID(std::string _model, IDType _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)ID_MAX) {
		Debug::CreateMessageBox("modelHandleSetMiss", "miss");
	}
	(*modelId)[_type] = _model;
}

ID::IDType ID::StringToID(std::string _str)
{
	for (const auto& pair : *modelId) {
		if (pair.second == _str) {
			return pair.first; // ílÇ™å©Ç¬Ç©Ç¡ÇΩÇÁÉLÅ[Çï‘Ç∑
		}
	}
	return ID::ID_MAX;
}


