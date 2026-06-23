#include "StateID.h"
#include "ID.h"
#include <unordered_map>
#include <assert.h>
#include "../Debug/Debug.h"

namespace {
	std::unordered_map<StateID::State_ID, std::string> stateId;
}

void StateID::DeleteStateID()
{
	stateId.clear();
}

std::string StateID::GetID(State_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)STATE_MAX) {
		Debug::CreateMessageBox("StateHandleMiss", "miss");
	}
	return stateId[_type];
}

void StateID::SetID(std::string _state, State_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)STATE_MAX) {
		Debug::CreateMessageBox("StateSetMiss", "miss");
	}
	stateId[_type] = _state;
}

StateID::State_ID StateID::StringToID(std::string _str)
{
	for (const auto& pair : stateId) {
		if (pair.second == _str) {
			return pair.first; // ’l‚ªŒ©‚Â‚©‚Á‚½‚çƒL[‚ğ•Ô‚·
		}
	}
	return StateID::STATE_MAX;
}