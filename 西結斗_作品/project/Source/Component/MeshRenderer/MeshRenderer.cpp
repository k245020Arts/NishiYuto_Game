#include "MeshRenderer.h"
#include "../Transform/Transform.h"
#include "../../../ImGui/imgui.h"
#include "../ComponentManager.h"
#include "../Shaker/Shaker.h"

MeshRenderer::MeshRenderer()
{
	hModel			= -1;
	draw			= true;
	shaker			= nullptr;
	debugId			= 6;
	tag				= Function::GetClassNameC<MeshRenderer>();
	angle			= 0.0f;
	color			= nullptr;
	meshNum			= 0;
	worldTransform	= true;
	depricate		= false;
}

MeshRenderer::~MeshRenderer()
{
	if (depricate) { //デプリケートモデルで設定しているなら
		if (hModel != -1) {
			MV1DeleteModel(hModel);
		}
	}
}

void MeshRenderer::Update()
{
	
}

void MeshRenderer::Draw()
{
	if (!draw) { //描画をしないならリターン
		return;
	}
	if (hModel < 0) { //モデルがロード出来てなかったら
		return;
	}
	//描画専用のトランスフォームを作る
	Transform tra = *obj->GetTransform();
	if (shaker != nullptr) {
		//ヒットストップなどでシェイクしているときは描画用のトランスフォームだけを動かしている
		tra.position += shaker->GetShakePower();
	}
	if (worldTransform) {
		MV1SetMatrix(hModel, tra.WorldTransform().GetMatrix());
	}
	else {
		MV1SetMatrix(hModel, tra.GetMatrix());
	}
	obj->GetTransform()->GetMatrix();
	MV1SetDifColorScale(hModel, color->GetColorF());
	MV1DrawModel(hModel);
}

void MeshRenderer::ModelHandle(int _hHandle, bool _depricate)
{
	hModel = _hHandle;
	color = obj->Component()->GetComponent<Color>();
	shaker = obj->Component()->GetComponent<Shaker>();
	depricate = _depricate;
}

void MeshRenderer::ModelHandle(int _hHandle)
{
	ModelHandle(_hHandle, false);
}

void MeshRenderer::ImguiDraw()
{
	ImGui::DragInt("mesh", &meshNum, 1.0f, 0, 32);
	ImGui::DragFloat("ang", &angle, 1.0f, 0, 360);
	if (ImGui::Button("button")) {
		
		RotationMesh(meshNum, angle * DegToRad);
			
	}
	if (ImGui::Button("reset")) {
		MV1ResetFrameUserLocalMatrix(hModel,meshNum);
	}
	if (ImGui::Button("+")) {
		meshNum++;
	}
	if(ImGui::Button("noDraw")) {
		draw = !draw;
	}
}

void MeshRenderer::RotationMesh(int _meshNum, float _angle)
{
	MV1SetFrameUserLocalMatrix(hModel, _meshNum, MGetRotY(_angle));
}
