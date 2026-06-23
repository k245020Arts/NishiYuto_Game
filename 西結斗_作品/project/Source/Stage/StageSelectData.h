#pragma once
#include "../Common/Singleton/SingletonBase.h"
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../Common/JsonReader.h"
#include "../Common/UTFConverter.h"

struct StageData
{
	int id;
	std::string name;
    int stageModelID;
	std::string bgm;
    int bossID;
    std::string stageImageHandleName;

    StageData() {
        id = -1;
        name = "";
        stageModelID = -1;
        bgm = "";
        bossID = -1;
        stageImageHandleName = "";
    }
};

class StageSelectData : public SingletonBase<StageSelectData>
{
public:
    void Update() override {}
    void Draw() override {}

    bool Load(const std::string& filename)
    {
        if (!jsonReader.Load(filename)) {
            return false;
        }

        auto& json = jsonReader.Data();

        // "stages" が存在するかチェック
        if (!json.contains("stages")) {
            return false;
        }

        stages.clear();

        for (const auto& elem : json["stages"])
        {
            StageData stage;

            stage.id = elem.value("id", -1);
            stage.name = UTFConverter::Utf8ToSjis(elem.value("name", ""));
            stage.stageModelID = elem.value("map", -1);
            stage.bgm = elem.value("bgm", "");
            stage.bossID = elem.value("bossID", -1);
            stage.stageImageHandleName = elem.value("stageImageHandleName", "");
            
            stages.push_back(stage);
        }

        return true;
    }

    int GetStageMax() const
    {
        return static_cast<int>(stages.size());
    }

    const StageData& GetStageData(int index) const{
        return stages.at(index);
    }

    const StageData&  GetNowStageData() const {
        return stages.at(stageID);
    }

    const std::vector<StageData>& GetAllStageData() { return stages; }

    int GetStageID() const { return stageID; }
    void SetStageID(int id) { stageID = id; }

private:
    friend class SingletonBase<StageSelectData>;

    StageSelectData();
    ~StageSelectData() {}

    JsonReader jsonReader;
    std::vector<StageData> stages;
    int stageID;
};