#pragma once
#include "Camera.h"

struct ImFont;

class CameraEditorGui
{
public:
	CameraEditorGui();
	CameraEditorGui(Camera* _camera);
	~CameraEditorGui();
	/// <summary>
	/// EditorWindowの表示
	/// </summary>
	void EditorWindow();
private:
	Camera* camera;
	std::string fileName;
	VECTOR3 basePosition;
	VECTOR3 offset;
	VECTOR3 baseTarget;
	int cutNum; //今どのカットを編集しているかの項目
	std::string cutSceneName;

	CutSceneSpece::CutScene cutScene; //ここの値でパラメータを調整する

	char cutSceneFileName[128]; //カットシーンのファイル名を記入したいのでcharの配列で行う

	int  currentLoadedCut;
	bool isLoaded;
	std::string lastFileName;

	bool IsUsingCutScene(int cutNum, const nlohmann::json& root);

	void SavePopUp(bool _using);

	void DataSave();

	void LoadCutScene();

	ImFont* font;
	
};