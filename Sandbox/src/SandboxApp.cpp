#include <GameEngine.h>
#include "GLFW/glfw3.h"
#include <stdio.h>
#include <iostream>

class ExampleLayer : public GameEngine::Layer
{
public:
	GameEngine::Renderer *renderer;
	GameEngine::Application* application;
	GameEngine::Camera *camera;

	ExampleLayer() : Layer("Example")
	{
		renderer = new GameEngine::Renderer();
		application = &GameEngine::Application::Get();
		camera = &application->GetCamera();

		GameEngine::ImportShaders(renderer);

		{
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
		}

		renderer->meshes["box"] = renderer->LoadMesh("box", "D:\\Facultate\\PPBG\\gfx-framework-master\\assets\\models\\primitives\\box.obj");
		renderer->meshes["sphere"] = renderer->LoadMesh("sphere", "D:\\Facultate\\PPBG\\gfx-framework-master\\assets\\models\\primitives\\sphere.obj");
		//renderer->meshes["teapot"] = renderer->LoadMesh("teapot", "D:\\Facultate\\PPBG\\gfx-framework-master\\assets\\models\\primitives\\teapot.obj");
		//renderer->meshes["gun"] = renderer->LoadMesh("gun", "D:\\Facultate\\Master\\xm177_with_gl.obj");
	}

	void OnUpdate(float deltaTime) override
	{
		/*glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.0f, 0.0f));
		renderer->RenderMesh(renderer->meshes["quad"], renderer->shaders["red"], modelMatrix);*/

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 2, 2.5f));
		renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["BasicShader"], modelMatrix, glm::vec3(0.1f, 0.5f, 0.87f));

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(5.0f, 0.0f, -3.0f));
		renderer->RenderMesh(renderer->meshes["sphere"], renderer->shaders["BasicShader"], modelMatrix);

		/*modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(5.0, 0.0f, 0.0f));
		renderer->RenderMesh(renderer->meshes["teapot"], renderer->shaders["red"], modelMatrix);*/
	}

	void OnInputUpdate(float deltaTime, float deltaX, float deltaY) override
	{
		if (GameEngine::InputInterface::IsMouseBtnPressed(GLFW_MOUSE_BUTTON_RIGHT))
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