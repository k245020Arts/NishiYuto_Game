#include "PlayerParamWindow.h"
#include "../Common/InputManager/InputManager.h"
#include "../../ImGui/imgui.h"

PlayerParamWindow::PlayerParamWindow() : PlayerParamWindow(nullptr)
{
	
}

PlayerParamWindow::PlayerParamWindow(Player* _player)
{
	windowMode = false;
	player = _player;
}

PlayerParamWindow::~PlayerParamWindow()
{
}

void PlayerParamWindow::PlayerParamWindowView()
{
	if (InputManager::GetInstance()->KeyInputDown("playerParam")) {
		windowMode = !windowMode;
	}
	if (!windowMode) {//Windowを開いてないならリターン
		return;
	}
    ImGui::Begin("Player Param");

    auto& attackData = player->GetAllAttackData();

    for (auto& [state, data] : attackData)
    {
        std::string label = StateID::GetID(state);

        if (ImGui::TreeNode(label.c_str()))
        {
            ImGui::DragFloat("Damage", &data.hitDamage, 1.0f);
            ImGui::DragFloat3("AttackMove", &data.attackMove.x, 10.0f);

            ImGui::DragFloat("StartTime", &data.collsionStartTime, 0.01f);
            ImGui::DragFloat("EndTime", &data.collsionFinishTime, 0.01f);

            //トランスフォーム
            ImGui::DragFloat3("Pos", &data.collTrans.position.x, 1.0f);
            ImGui::DragFloat3("Scale", &data.collTrans.scale.x, 1.0f);

            ImGui::DragFloat("noAttackRunTimer", &data.noAttackRunTimer, 0.01f);
            ImGui::DragFloat("nextAttackRunTimer", &data.nextAttackRunTimer, 0.01f);

            ImGui::DragInt("attackNum", &data.attackNum, 1);
            ImGui::DragFloat("attackAgainStartCounterMax", &data.attackAgainStartCounterMax, 1);


            ImGui::DragFloat("motionCancelTime", &data.motionCancelStartTime, 0.01f);
            ImGui::DragFloat("AttackInputStartTime", &data.attackInputStartTime, 0.01f);

            // チャージ設定
            if (ImGui::TreeNode("ChargeLevels"))
            {
                for (int i = 0; i < data.chargeLevels.size(); i++)
                {
                    auto& level = data.chargeLevels[i];

                    std::string lv = "Level " + std::to_string(i);

                    if (ImGui::TreeNode(lv.c_str()))
                    {
                        ImGui::DragFloat("Damage", &level.hitDamage, 1.0f);
                        ImGui::DragFloat3("MoveSpeed", &level.moveSpeed.x, 10.0f);
                        ImGui::DragFloat("AgainTimer", &level.againTimer, 0.01f);
                        ImGui::DragInt("AgainTimerFlag", &level.againTimerFlag, 1);
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }
    //データの保存
    if (ImGui::Button("Save"))
    {
        SaveAttackData();
    }
    //データのリロード
    if (ImGui::Button("Reload"))
    {
        LoadAttackData();
    }

    ImGui::End();

    ImGui::Begin("Player Reaction Editor");

    auto& reactions = player->GetReactionMap();

    for (auto& [state, r] : reactions)
    {
        std::string label = StateID::GetID(state);

        if (ImGui::TreeNode(label.c_str()))
        {
            //基本
            ImGui::DragFloat3("PlayerShakePower", &r.shakePower.x, 1.0f);
            ImGui::DragFloat("PlayerShakeTime", &r.shakeTime, 0.01f);

            ImGui::DragFloat3("CameraShakePower", &r.cameraShakePower.x, 1.0f);
            ImGui::DragFloat("CameraShakeTime", &r.cameraShakeTime, 0.01f);

            //ImGui::InputText("SoundName", &r.soundName);
            ImGui::DragInt("SoundKind", &r.soundKind);

            ImGui::Checkbox("Loop", &r.shakerLoop);

            int pattern = static_cast<int>(r.shakePattern);
            ImGui::DragInt("ShakePattern", &pattern, 1, 0, 10);
            r.shakePattern = static_cast<Shaker::ShakePattern>(pattern);

            //ChargeLevels
            if (ImGui::TreeNode("ChargeLevels"))
            {
                for (int i = 0; i < r.chargeLevels.size(); i++)
                {
                    auto& c = r.chargeLevels[i];

                    std::string name = "Level " + std::to_string(i);

                    if (ImGui::TreeNode(name.c_str()))
                    {
                        ImGui::DragFloat3("ShakePower", &c.shakePower.x, 1.0f);
                        ImGui::DragFloat("ShakeTime", &c.shakeTime, 0.01f);

                        ImGui::DragFloat3("CameraPower", &c.cameraShakePower.x, 1.0f);
                        ImGui::DragFloat("CameraTime", &c.cameraShakeTime, 0.01f);

                        ImGui::TreePop();
                    }
                }

                //追加と削除
                if (ImGui::Button("Add Level"))
                {
                    r.chargeLevels.emplace_back();
                }

                ImGui::SameLine();

                if (ImGui::Button("Remove Level") && !r.chargeLevels.empty())
                {
                    r.chargeLevels.pop_back();
                }

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }

    
    //セーブ
    if (ImGui::Button("Save Reaction"))
    {
        SaveReaction();
    }
    //リロード
    if (ImGui::Button("Reload"))
    {
        LoadReaction();
    }

    ImGui::End();

}

void PlayerParamWindow::SaveReaction()
{
    nlohmann::json root;

    auto reactions = player->GetReactionMap();

    for (auto& [state, r] : reactions)
    {
        std::string key = StateID::GetID(state);
        root["PlayerReactions"][key] = r;
    }

    JsonReader json;
    json.Save("data/json/playerReaction.json", root);
}

void PlayerParamWindow::SaveAttackData()
{
    nlohmann::json root;

    auto attackData = player->GetAllAttackData();

    for (auto& [state, data] : attackData)
    {
        std::string key = StateID::GetID(state);
        root["PlayerAttackData"][key] = data;
    }

    JsonReader json;
    json.Save("data/json/playerAttackData.json", root);
}

void PlayerParamWindow::LoadAttackData()
{
    player->ParamReLoad();
}

void PlayerParamWindow::LoadReaction()
{

}
