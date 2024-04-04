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

		//renderer->meshes["quad"] = renderer->CreateMesh("quad", vertices, indices);

		renderer->meshes["box"] = renderer->LoadMesh("box", "D:\\Facultate\\PPBG\\gfx-framework-master\\assets\\models\\primitives\\box.obj");
		//renderer->meshes["sphere"] = renderer->LoadMesh("sphere", "D:\\Facultate\\PPBG\\gfx-framework-master\\assets\\models\\primitives\\sphere.obj");
		//renderer->meshes["teapot"] = renderer->LoadMesh("teapot", "D:\\Facultate\\PPBG\\gfx-framework-master\\assets\\models\\primitives\\teapot.obj");
		//renderer->meshes["gun"] = renderer->LoadMesh("gun", "D:\\Facultate\\Master\\xm177_with_gl.obj");

		std::string vertexCode = R"(
				#version 330

				layout(location = 0) in vec3 v_position;
				layout(location = 1) in vec3 v_normal;

				// Uniform properties
				uniform mat4 Model;
				uniform mat4 View;
				uniform mat4 Projection;

				// Uniforms for light properties
				uniform vec3 light_position;
				uniform vec3 eye_position;
				uniform float material_kd;
				uniform float material_ks;
				uniform int material_shininess;

				uniform vec3 object_color;

				out vec3 color;

				void main()
				{
					vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
					vec3 world_normal = normalize(mat3(Model) * v_normal);

					vec3 L = normalize(light_position - world_pos);
					vec3 V = normalize(eye_position - world_pos);
					vec3 H = normalize(L + V);

					float ambient_light = material_kd * 0.25;

					float diffuse_light = material_kd * max(dot(world_normal, L), 0);

					float rez = dot(world_normal, V);
					int primesteLumina = 1;
					if (rez < 0) {
						primesteLumina = 0;
					}

					float specular_light = 0;

					if (diffuse_light > 0)
					{
						specular_light = material_ks * primesteLumina * pow(max(dot(world_normal, H), 0), material_shininess);
					}

					float factorAtenuare = 1 / (distance(light_position, world_pos)*distance(light_position, world_pos));

					color = ambient_light + factorAtenuare * (diffuse_light + specular_light) * object_color;

					gl_Position = Projection * View * Model * vec4(v_position, 1.0);
				}

			)";

		std::string fragmentCode = R"(
				#version 330
	
				in vec3 color;

				layout(location = 0) out vec4 out_color;

				void main()
				{
					out_color = vec4(color, 1.0);
				}

			)";

		renderer->shaders["SimpleShader"] = new GameEngine::Shader(vertexCode, fragmentCode);
	}

	void OnUpdate(float deltaTime) override
	{
		/*glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.0f, 0.0f));
		renderer->RenderMesh(renderer->meshes["quad"], renderer->shaders["red"], modelMatrix);*/

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 2, 2.5f));
		renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["SimpleShader"], modelMatrix, glm::vec3(0.1f, 0.5f, 0.87f));

		/*modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(5.0f, 0.0f, -3.0f));
		renderer->RenderMesh(renderer->meshes["sphere"], renderer->shaders["red"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(5.0, 0.0f, 0.0f));
		renderer->RenderMesh(renderer->meshes["teapot"], renderer->shaders["red"], modelMatrix);*/
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