#include "Application.h"
#include "glad/glad.h"
#include "GameEngine/GPU/Renderer.h"

namespace GameEngine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<WindowInterface>(WindowInterface::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
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

		printf(e.ToString().c_str());
		printf("\n");

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
			glClear(GL_COLOR_BUFFER_BIT);

			std::vector<float> vertices = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
			};

			std::vector<unsigned int> indices = { 0, 1, 2 };

			Renderer *renderer = new Renderer();
			renderer->meshes["triangle"] = renderer->CreateMesh(vertices, indices);

			std::string vertexCode = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;

				void main()
				{
					gl_Position = vec4(a_Position, 1.0);
				}

			)";

				std::string fragmentCode = R"(
				#version 330 core

				layout(location = 0) out vec4 color;

				void main()
				{
					color = vec4(0.8, 0.2, 0.3, 1.0);
				}

			)";


			Shader* m_Shader = new Shader(vertexCode, fragmentCode);
			renderer->RenderMesh(renderer->meshes["triangle"], m_Shader);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}