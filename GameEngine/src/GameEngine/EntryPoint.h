#pragma once

#ifdef GE_PLATFORM_WINDOWS

extern GameEngine::Application* GameEngine::CreateApplication();

int main(int agrc, char** argv) {

	auto app = GameEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif