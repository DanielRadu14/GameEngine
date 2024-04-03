#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "WindowInterface.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "GPU/Shader.h"
#include "GameEngine/GPU/Mesh.h"
#include "Camera/Camera.h"

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

		void Application::CameraBehaviour(float deltaTime, float deltaX, float deltaY);
		Camera& GetCamera() { return *camera; }
		glm::mat4 GetProjectionMatrix() { return projectionMatrix; }

	protected:
		Camera *camera;
		glm::mat4 projectionMatrix;

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<WindowInterface> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		float m_LastCursorXPos = 0.0f;
		float m_LastCursorYPos = 0.0f;

		static Application* s_Instance;
	};

	Application* CreateApplication();
}

