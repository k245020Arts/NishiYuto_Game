#pragma once
#include "../../Library/GameObject.h"
#include "vector"
#include "../Component/Transform/Transform.h"
#include "../Common/JsonReader.h"

class Object3D;

static const float WALL_EDGE_POS = 13000.0f;

struct StageModelData
{
	std::string mapFile;
	int stageModelID;
	Transform transform;
	StageModelData() {
		mapFile = "";
		stageModelID = -1;
		transform = Transform();
	}
};

class StageManager : public GameObject
{
public:
	StageManager();
	~StageManager();
	void Update() override;
	void Draw() override;
	/// <summary>
	/// ステージの生成
	/// </summary>
	void CreateStage();
	/// <summary>
	/// ステージの壁の生成
	/// </summary>
	void CreateWall();
	/// <summary>
	/// ステージの床の生成
	/// </summary>
	void CreateFloor();

private:

	bool Load(const std::string& filename)
	{
		if (!jsonReader.Load(filename)) {
			return false;
		}

		auto& json = jsonReader.Data();

		//"stageModelDatas"が存在するかチェック
		if (!json.contains("stageModelDatas")) {
			return false;
		}

		stageModelData.clear();

		for (const auto& elem : json["stageModelDatas"])
		{
			StageModelData stage;

			stage.stageModelID = elem.value("id", -1);
			stage.mapFile = elem.value("map", "");
			stage.transform = elem.value("Transform", Transform());

			stageModelData.push_back(stage);
		}

		return true;
	}
	
	std::vector<Object3D*> maps;
	Object3D* stage;
	int modelHandle;

	std::vector<StageModelData> stageModelData;

	JsonReader jsonReader;
};
