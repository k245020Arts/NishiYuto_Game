#pragma once
#include "../../../Library/GameObject.h"
#include "../Component.h"

class Color;
class Object3D;
class InputManager;
class FadeTransitor;
class StateManager;
class Camera;

//デバックウィンドウを出すときに使うクラス。

class Hierachy : public GameObject
{
public:
	Hierachy();
	~Hierachy();
	void Update() override;
	void Draw() override;
	/// <summary>
	/// デバック画面リストに追加
	/// </summary>
	/// <param name="_name"></param>
	/// <param name="_obj"></param>
	void Start(std::string _name, BaseObject* _obj);

	/// <summary>
	/// 実際に描画をしているところunityでいうヒエラルキーの描画
	/// </summary>
	/// <param name="_obj"></param>
	void DrawHierarchy(BaseObject* _obj);
	/// <summary>
	/// 実際にパラメータとかを出しているところ、unityでいうところのインスペクターの描画
	/// </summary>
	void InspectorDraw();

	/// <summary>
	/// ヒエラルキーの要素を削除する
	/// </summary>
	/// <param name="_obj">ヒエラルキーの要素を削除したいオブジェクト</param>
	void RemoveHierachy(BaseObject* _obj);
	/// <summary>
	/// すべてのデータを削除
	/// </summary>
	void AllDeleteList();

	/// <summary>
	/// カメラEditor用のウィンドウをセット
	/// </summary>
	/// <param name="_camera"></param>
	void SetCameraEditor(Camera* _camera);

private:
	//Transform* transform;
	std::string name;
	Color* color;
	std::list<BaseObject*> debugOutPutObjectList;

	bool windowMode;
	FadeTransitor* fade;
	int selectId;
	BaseObject* selectObj;
	Camera* camera;
};