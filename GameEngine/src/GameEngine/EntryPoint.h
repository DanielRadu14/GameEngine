#pragma once

#ifdef GE_PLATFORM_WINDOWS

extern GameEngine::Application* GameEngine::CreateApplication();

std::string GetParentDir(const std::string &filePath)
{
	size_t pos = filePath.find_last_of("\\/");
	return (std::string::npos == pos) ? "." : filePath.substr(0, pos);
}

int main(int agrc, char** argv) {

	printf("Game Engine initialized\n");
	printf(GetParentDir(std::string(argv[0])).c_str());

	auto app = GameEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif