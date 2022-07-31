#include "features.hpp"
#include "../sdk/sdk.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"

void features::run()
{
	auto overlay = ImGui::GetForegroundDrawList();
	auto game_manager = sdk::game_manager().get();
	auto round_manager = sdk::round_manager().get();
	auto players = game_manager->get_players();

	overlay->AddText({ 5, 5 }, ImColor(255, 255, 255), "R6 Internal");

	if (round_manager->state() != sdk::round_state::prep && round_manager->state() != sdk::round_state::active)
		return;

	auto camera = sdk::camera().get();

	for (auto player : players)
	{
		auto pos = player->pawn()->actor()->position();
		auto screen = camera->w2s(pos).as<ImVec2>();
		if (!screen.x && !screen.y)
			continue;

		auto res = camera->res();
		auto color = ImColor(0, 0, 0, 100);

		overlay->AddLine({ res.x / 2.f, res.y }, { screen.x, screen.y }, ImColor(255, 255, 255), 2.f);
	}

}