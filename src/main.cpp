#include "raylib.h"
#include <iostream>
#include <vector>
#include <format>

#pragma region imgui
#include "imgui.h"
#include "rlImGui.h"
#include "imguiThemes.h"
#pragma endregion




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



	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		rlImGuiBegin();


		ImGui::Begin("Test");
		for (auto& line : lines) {
			std::string e = std::format("({}, {}) -> ({}, {})", line.first.x, line.first.y, line.second.x, line.second.y);
			ImGui::Text(e.c_str());
		}
		ImGui::End();


		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !io.WantCaptureMouse) {
			startPoint = GetMousePosition();
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !io.WantCaptureMouse) {
			endPoint = GetMousePosition();
		}

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && !io.WantCaptureMouse) {
			lines.push_back({ startPoint, endPoint });
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