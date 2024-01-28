#include "Application.h"

#include "Events/ApplicationEvent.h"

namespace GameEngine {
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run() {

		WindowResizeEvent e(1280, 720);
		printf(e.ToString().c_str());

		while (true);
	}
}