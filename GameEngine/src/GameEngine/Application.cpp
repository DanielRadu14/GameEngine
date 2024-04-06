#include "Application.h"
#include "glad/glad.h"
#include "GameEngine/GPU/Renderer.h"
#include "InputInterface.h"
#include "Window.h"
#include "glm/glm.hpp"

namespace GameEngine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		m_Window = std::unique_ptr<WindowInterface>(WindowInterface::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		camera = new Camera();
		camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

		Renderer *renderer = new Renderer();

		projectionMatrix = glm::perspective(RADIANS(60), m_Window->GetAspectRatio(), 0.01f, 200.0f);
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//printf(e.ToString().c_str());
		//printf("\n");

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f , 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());

			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);

			float time = (float)glfwGetTime();
			float deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			float deltaX = InputInterface::GetMouseX() - m_LastCursorXPos;
			float deltaY = InputInterface::GetMouseY() - m_LastCursorYPos;
			m_LastCursorXPos = InputInterface::GetMouseX();
			m_LastCursorYPos = InputInterface::GetMouseY();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(deltaTime);
				layer->OnInputUpdate(deltaTime, deltaX, deltaY);
			}
				
			m_Window->OnUpdate(deltaTime);
			LightMovement(deltaTime, deltaX, deltaY);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::LightMovement(float deltaTime, float deltaX, float deltaY)
	{
		float speed = 2;

		if (InputInterface::IsMouseBtnPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			glm::vec3 up = glm::vec3(0, 1, 0);
			glm::vec3 right = camera->right;
			glm::vec3 forward = camera->forward;
			forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

			if (InputInterface::IsKeyPressed(GLFW_KEY_S))
			{
				lightPosition -= forward * deltaTime * speed;
			}
			if (InputInterface::IsKeyPressed(GLFW_KEY_W))
			{
				lightPosition += forward * deltaTime * speed;
			}
			if (InputInterface::IsKeyPressed(GLFW_KEY_A))
			{
				lightPosition -= right * deltaTime * speed;
			}
			if (InputInterface::IsKeyPressed(GLFW_KEY_D))
			{
				lightPosition += right * deltaTime * speed;
			}
			if (InputInterface::IsKeyPressed(GLFW_KEY_Q))
			{
				lightPosition -= up * deltaTime * speed;
			}
			if (InputInterface::IsKeyPressed(GLFW_KEY_E))
			{
				lightPosition += up * deltaTime * speed;
			}
		}
	}
}