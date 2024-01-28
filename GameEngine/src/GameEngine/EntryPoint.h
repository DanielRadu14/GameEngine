#pragma once

#ifdef GE_PLATFORM_WINDOWS

extern GameEngine::Application* GameEngine::CreateApplication();

int main(int agrc, char** argv) {

	//GameEngine::Log::Init();
	printf("Game Engine initialized\n");

	auto app = GameEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif