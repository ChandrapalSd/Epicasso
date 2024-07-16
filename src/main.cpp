#include "raylib.h"
#include <iostream>
#include <vector>
#include <format>

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
	std::vector<std::pair<Vector2, Vector2>> lines;

	Vector2 startPoint = { 0, 0 };
	Vector2 endPoint = { 0, 0 };
	bool hasEndPoint = false;
	bool drawing = false;
	

	while (!WindowShouldClose())
	{
		const Vector2 mousePos = GetMousePosition();

		BeginDrawing();
		ClearBackground(RAYWHITE);

		rlImGuiBegin();


		ImGui::Begin("Test");
		int count = 0;
		for (auto& line : lines) {
			std::string e = std::format("{}. ({}, {}) -> ({}, {})", ++count, line.first.x, line.first.y, line.second.x, line.second.y);
			ImGui::Text(e.c_str());
		}
		ImGui::End();


		if (IsKeyPressed(KEY_C)) {
			startPoint = {};
			endPoint = {};
			hasEndPoint = false;
			drawing = false;
			lines.clear();
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
				lines.push_back({ startPoint, endPoint });
				lines.push_back({ endPoint, newEndPoint });
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
					lines.push_back({ startPoint, newEndPoint });
				}
				else {
					lines.push_back({ startPoint, endPoint });
					lines.push_back({ endPoint, newEndPoint });
				}
			}
		}

		DrawLineV(startPoint, endPoint, BLACK);

		for (auto& line : lines) {
			DrawLineV(line.first, line.second, RED);
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