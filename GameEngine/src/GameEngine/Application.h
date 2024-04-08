#pragma once

#include "Events/Event.h"
#include "WindowInterface.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "LayerStack.h"
#include "GPU/Shader.h"
#include "GameEngine/GPU/Mesh.h"
#include "Camera/Camera.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

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

		Camera& GetCamera() { return *camera; }
		glm::mat4 GetProjectionMatrix() { return projectionMatrix; }

		void LightMovement(float deltaTime, float deltaX, float deltaY);
		void CameraMovement(float deltaTime, float deltaX, float deltaY);

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

	public:
		glm::vec3 lightPosition = glm::vec3(0.13f, 0.49, 3.74f);
		bool controlLight = false;
		bool controlCamera = false;

	protected:
		Camera *camera;
		glm::mat4 projectionMatrix;

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

