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

		renderer->meshes["quad"] = renderer->CreateMesh("quad", vertices, indices);

		renderer->meshes["box"] = renderer->LoadMesh("box", "D:\\Facultate\\PPBG\\gfx-framework-master\\assets\\models\\primitives\\box.obj");
		renderer->meshes["sphere"] = renderer->LoadMesh("sphere", "D:\\Facultate\\PPBG\\gfx-framework-master\\assets\\models\\primitives\\sphere.obj");
		renderer->meshes["teapot"] = renderer->LoadMesh("teapot", "D:\\Facultate\\PPBG\\gfx-framework-master\\assets\\models\\primitives\\teapot.obj");

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

	void OnUpdate(float deltaTime) override
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.0f, 0.0f));
		renderer->RenderMesh(renderer->meshes["quad"], renderer->shaders["red"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.0f, -3.0f));
		renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["red"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(5.0f, 0.0f, -3.0f));
		renderer->RenderMesh(renderer->meshes["sphere"], renderer->shaders["red"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(5.0, 0.0f, 0.0f));
		renderer->RenderMesh(renderer->meshes["teapot"], renderer->shaders["red"], modelMatrix);
	}

	void OnEvent(GameEngine::Event& event) override
	{
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