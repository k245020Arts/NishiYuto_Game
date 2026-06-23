#include "color.h"
#include "../../../ImGui/imgui.h"

void Color::ImguiDraw()
{
	ImGui::RadioButton("false", &changeMode, 0);
	ImGui::RadioButton("true", &changeMode, 1);
	if (changeMode) {
		if (ImGui::TreeNode(("CHANGE_COLOR" + obj->GetTag()).c_str())) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(colorfRgb.r, colorfRgb.g, colorfRgb.b, 1.0f));
			ImGui::ColorEdit4("Color", &colorfRgb.r,1);
			ColorFToRGB();
			ImGui::TreePop();
			ImGui::PopStyleColor();
		}
	}
}
