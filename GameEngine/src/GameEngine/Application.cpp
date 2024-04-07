#include "Application.h"
#include "glad/glad.h"
#include "GameEngine/GPU/Renderer.h"
#include "InputInterface.h"
#include "Window.h"
#include "glm/glm.hpp"
#include <iostream>

namespace GameEngine {

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
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));

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
				if(!controlLight && !controlCamera)
					layer->OnInputUpdate(deltaTime, deltaX, deltaY);

				//light object
				{
					if (controlLight)
					{
						Renderer *renderer = &Renderer::Get();
						glm::mat4 modelMatrix = glm::mat4(1);
						modelMatrix = glm::translate(modelMatrix, lightPosition);
						modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
						renderer->RenderMesh(renderer->meshes["sphere"], renderer->shaders["BasicShader"], modelMatrix);
					}
				}
			}
				
			m_Window->OnUpdate(deltaTime);
			LightMovement(deltaTime, deltaX, deltaY);
			CameraMovement(deltaTime, deltaX, deltaY);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == 76)//L
		{
			controlCamera = false;
			controlLight = !controlLight;
		}
		if (e.GetKeyCode() == 67)//C
		{
			controlLight = false;
			controlCamera = !controlCamera;
		}
		return true;
	}

	void Application::CameraMovement(float deltaTime, float deltaX, float deltaY)
	{
		if (controlCamera)
		{
			if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_S))
			{
				camera->MoveBackward(deltaTime * camera->cameraSpeed);
			}
			if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_W))
			{
				camera->MoveForward(deltaTime * camera->cameraSpeed);
			}
			if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_A))
			{
				camera->MoveLeft(deltaTime * camera->cameraSpeed);
			}
			if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_D))
			{
				camera->MoveRight(deltaTime * camera->cameraSpeed);
			}
			if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_Q))
			{
				camera->MoveDown(deltaTime * camera->cameraSpeed);
			}
			if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_E))
			{
				camera->MoveUp(deltaTime * camera->cameraSpeed);
			}

			camera->RotateFirstPerson_OX(-deltaX * camera->sensivityOX);
			camera->RotateFirstPerson_OY(-deltaY * camera->sensivityOY);
		}
	}
	void Application::LightMovement(float deltaTime, float deltaX, float deltaY)
	{
		float speed = 2;

		if (controlLight)
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