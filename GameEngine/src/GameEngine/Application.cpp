#include "Application.h"
#include "glad/glad.h"
#include "GameEngine/GPU/Renderer.h"
#include "InputInterface.h"
#include "Window.h"

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
			//de facut functii separate si mutate in Sandbox (episod : Moving to SandBox)
			glClearColor(0.1f, 0.1f, 0.1f , 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());

			float time = (float)glfwGetTime();
			float deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			float deltaX = InputInterface::GetMouseX() - m_LastCursorXPos;
			float deltaY = InputInterface::GetMouseY() - m_LastCursorYPos;
			m_LastCursorXPos = InputInterface::GetMouseX();
			m_LastCursorYPos = InputInterface::GetMouseY();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(deltaTime);

			m_Window->OnUpdate(deltaTime);
			CameraBehaviour(deltaTime, deltaX, deltaY);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::CameraBehaviour(float deltaTime, float deltaX, float deltaY)
	{
		float cameraSpeed = 2.0f;
		float sensivityOX = 0.005f;
		float sensivityOY = 0.005f;

		if (InputInterface::IsKeyPressed(GLFW_KEY_S))
		{
			camera->MoveBackward(deltaTime * cameraSpeed);
		}
		if (InputInterface::IsKeyPressed(GLFW_KEY_W))
		{
			camera->MoveForward(deltaTime * cameraSpeed);
		}
		if (InputInterface::IsKeyPressed(GLFW_KEY_A))
		{
			camera->MoveLeft(deltaTime * cameraSpeed);
		}
		if (InputInterface::IsKeyPressed(GLFW_KEY_D))
		{
			camera->MoveRight(deltaTime * cameraSpeed);
		}
		
		if (InputInterface::IsMouseBtnPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			camera->RotateFirstPerson_OX(-deltaX * sensivityOX);
			camera->RotateFirstPerson_OY(-deltaY * sensivityOY);
		}
		
	}
}