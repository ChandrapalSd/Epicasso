#include "raylib.h"
#include <iostream>
#include <vector>
#include <format>

#include "action.h"
#include "editor.h"

#pragma region imgui
#include "imgui.h"
#include "rlImGui.h"
#include "imguiThemes.h"
#pragma endregion

bool areCollinear(Vector2 p1, Vector2 p2, Vector2 p3) {
	return (p2.y - p1.y) * (p2.x - p1.x) == (p3.y - p1.y) * (p3.x - p1.x);
}

bool operator== (Vector2 p1, Vector2 p2) {
	return (p1.x == p2.x) && (p1.y == p2.y);
}

Color toRLColor(ImColor& c) {
	return Color{
		static_cast<unsigned char>(c.Value.x * 255),
		static_cast<unsigned char>(c.Value.y * 255),
		static_cast<unsigned char>(c.Value.z * 255),
		static_cast<unsigned char>(c.Value.w * 255)
	};
}

int main(void)
{

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "Epicasso - Simple paint application in C++");

#pragma region imgui
	rlImGuiSetup(true);

	//you can use whatever imgui theme you like!
	//ImGui::StyleColorsDark();
	//imguiThemes::yellow();
	//imguiThemes::gray();
	imguiThemes::green();
	//imguiThemes::red();
	//imguiThemes::embraceTheDarkness();


	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.FontGlobalScale = 1;

	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
		//style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
	}

#pragma endregion
	std::vector<DrawAction> drawActions;

	Vector2 startPoint = { 0, 0 };
	Vector2 endPoint = { 0, 0 };
	bool hasEndPoint = false;
	bool drawing = false;
	EditorOption eOptions{
	.strokeWidth = 1,
	.strokeColor = IM_COL32(255,0,0,255) // REDs
	};


	while (!WindowShouldClose())
	{
		const Vector2 mousePos = GetMousePosition();

		BeginDrawing();
		ClearBackground(RAYWHITE);

		rlImGuiBegin();

#pragma region ImGuiDraw
		ImGui::Begin("Test");

		if (ImGui::CollapsingHeader("LineDebugInfo")) {
			int count = 0;
			for (auto& line : drawActions) {
				std::string e = std::format("{}. ({}, {}) -> ({}, {})", ++count, line.startPoint.x, line.startPoint.y, line.endPoint.x, line.endPoint.y);
				ImGui::Text(e.c_str());
			}
		}

		int strokeWidth = eOptions.strokeWidth;
		ImGui::SliderInt("Stroke Width", &strokeWidth, 1, 100);
		eOptions.strokeWidth = strokeWidth;

		ImGui::ColorPicker4("Stroke Color", (float*) & eOptions.strokeColor);

		ImGui::End();
#pragma endregion

		if (IsKeyPressed(KEY_C)) {
			startPoint = {};
			endPoint = {};
			hasEndPoint = false;
			drawing = false;
			drawActions.clear();
		}

		Vector2 newEndPoint = mousePos;

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !io.WantCaptureMouse) {
			startPoint = mousePos;
			endPoint = startPoint;
			drawing = true;
			hasEndPoint = false;
		}
		else if (drawing && startPoint != newEndPoint && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !io.WantCaptureMouse) {
			if (hasEndPoint && !areCollinear(startPoint, endPoint, newEndPoint)) {
				drawActions.push_back({ startPoint, endPoint, eOptions.strokeWidth, toRLColor(eOptions.strokeColor) });
				drawActions.push_back({ endPoint, newEndPoint, eOptions.strokeWidth, toRLColor(eOptions.strokeColor) });
				startPoint = newEndPoint;
				endPoint = newEndPoint;
				hasEndPoint = false;
			}
			else {
				endPoint = newEndPoint;
				hasEndPoint = true;
			}
		}
		else if (drawing && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && !io.WantCaptureMouse) {
			drawing = false;
			if (hasEndPoint) {
				if (areCollinear(startPoint, endPoint, newEndPoint)) {
					drawActions.push_back({ startPoint, newEndPoint,1,Color() });
				}
				else {
					drawActions.push_back({ startPoint, endPoint, eOptions.strokeWidth,  toRLColor(eOptions.strokeColor) });
					drawActions.push_back({ endPoint, newEndPoint, eOptions.strokeWidth, toRLColor(eOptions.strokeColor) });
				}
			}
		}

		DrawLineEx(startPoint, endPoint, eOptions.strokeWidth, toRLColor(eOptions.strokeColor));

		for (auto& line : drawActions) {
			DrawLineEx(line.startPoint, line.endPoint, line.strokeWidth, line.color);
		}

	#pragma region imgui
		rlImGuiEnd();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	#pragma endregion

		EndDrawing();
	}


#pragma region imgui
	rlImGuiShutdown();
#pragma endregion



	CloseWindow();

	return 0;
}