#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "WindowInterface.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "GPU/Shader.h"

namespace GameEngine {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<WindowInterface> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int VAO = 0;
		std::unique_ptr<Shader> m_Shader;
	};

	Application* CreateApplication();
}

