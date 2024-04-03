#include <GameEngine.h>

GameEngine::Renderer *renderer;

class ExampleLayer : public GameEngine::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
		renderer = new GameEngine::Renderer();

		std::vector<float> vertices = {
			-0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
		};

		std::vector<unsigned int> indices =
		{
			0, 1, 2,
			2, 1, 3
		};

		renderer->meshes["triangle"] = renderer->CreateMesh(vertices, indices);

		std::string vertexCode = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;

				// Uniform properties
				uniform mat4 Model;
				uniform mat4 View;
				uniform mat4 Projection;

				void main()
				{
					gl_Position = Projection * View * Model * vec4(a_Position, 1.0);
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

		renderer->shaders["red"] = new GameEngine::Shader(vertexCode, fragmentCode);
	}

	void OnUpdate() override
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.0f, 0.0f));
		renderer->RenderMesh(renderer->meshes["triangle"], renderer->shaders["red"], modelMatrix);
	}

	void OnEvent(GameEngine::Event& event) override
	{
		//episod Moving to Sandbox pentru exemple
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