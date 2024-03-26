#include <GameEngine.h>

class ExampleLayer : public GameEngine::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void OnUpdate() override
	{
		//printf("ExampleLayer::Update\n");
		std::vector<float> vertices = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		std::vector<unsigned int> indices = { 0, 1, 2 };

		GameEngine::Renderer *renderer = new GameEngine::Renderer();
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


		GameEngine::Shader* m_Shader = new GameEngine::Shader(vertexCode, fragmentCode);
		renderer->RenderMesh(renderer->meshes["triangle"], m_Shader);
	}

	void OnEvent(GameEngine::Event& event) override
	{
		printf(event.ToString().c_str());
		printf("\n");
	}
};

class Sandbox : public GameEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

GameEngine::Application* GameEngine::CreateApplication()
{
	return new Sandbox();
}