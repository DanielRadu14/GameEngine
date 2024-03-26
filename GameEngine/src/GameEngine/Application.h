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
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<WindowInterface> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::unordered_map<std::string, Mesh*> meshes;
		std::unique_ptr<Shader> m_Shader;
	};

	Application* CreateApplication();
}

