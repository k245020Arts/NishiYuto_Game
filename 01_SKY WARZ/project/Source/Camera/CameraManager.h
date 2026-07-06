#pragma once
#include "../../Library/GameObject.h"

class Object3D;
class Camera;

class CameraManager : public GameObject
{
public:
	CameraManager();
	~CameraManager();
	void Update() override;
	void Draw() override;

	/// <summary>
	/// プレイシーンのカメラの生成
	/// </summary>
	void CreateCamera();
	/// <summary>
	/// 最初に追従するオブジェクトを設定
	/// </summary>
	/// <param name="_str">オブジェクト名</param>
	void ParentObj(std::string _str);
	/// <summary>
	/// カメラのオブジェクトの取得
	/// </summary>
	/// <returns></returns>
	Object3D* GetCamera() { return mainCamera; }
	/// <summary>
	/// タイトルシーンのカメラの生成
	/// </summary>
	void TitleCameraCreate();
	/// <summary>
	/// 最初に追従するオブジェクトを設定
	/// </summary>
	/// <param name="_str">オブジェクト名</param>
	void TitleParentObj(std::string _str);

private:
	Object3D* mainCamera;
	Camera* cameraComponentPtr; //関数ポインタ実行用
};
