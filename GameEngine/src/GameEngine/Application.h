#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "WindowInterface.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "GPU/Shader.h"
#include "GameEngine/GPU/Mesh.h"

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

		inline WindowInterface& GetWindow() { return *m_Window;}
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<WindowInterface> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	Application* CreateApplication();
}

