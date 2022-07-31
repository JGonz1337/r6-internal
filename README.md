# R6 Internal

### Writeup
This was a small little passion project of mine, this is not intended to be a full cheat, this is a base for you to make a cheat of your own. It is not undetected because of its hooking & not spoofing functions. you can easily change this. 
This source is merely for educational purposes ONLY. I do not support anyone selling this code, or using it online.

### Features:
- Easily able to be made undetected with a few steps.
- A mildly small SDK containing enough features to make a very basic esp. 
- Easy to use keyboard system (Please do not use GetAsyncKeyState lul)
- Easy to use player list directly off game manager object.
- Very basic already implemented snapline esp.

### Before compiling
- Character Set: Multi-byte
- C++ version: 17+

### Culminating notes
- You can not use this online as is because of BattlEye, to circumvent this you will need a kernel injector. 
- To use this offline I recommend using any usermode program with injection capability (ProcessHacker, Extreme Injector, Xenos Injector)
- This does not include an sln, you have to create your own project using an ide of choice (CMake, visual studio etc.)
- "I crashed, why?" Since the cheat is internal if you dereference an invalid address you will crash, although I have not experienced any the entity loop inside 'features.cpp' doesn't look 100% secure, just add more valid checks.

### Showcase
![In Game](https://i.imgur.com/dGa7PoV.png)
``` C++
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
  ```
