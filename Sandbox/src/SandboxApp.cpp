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

	glm::mat4 head;
	glm::mat4 body;
	glm::mat4 leftHand;
	glm::mat4 rightHand;
	glm::mat4 leftLeg;
	glm::mat4 rightLeg;

	glm::vec3 playerMoveDirection;
	float playerRotateAngle;

	glm::mat4 enemy;
	float enemyMovingCoordinates = 0.0f;
	bool enemiesGoingForward = true;

	glm::vec3 projectilePosition;
	glm::vec3 shootingDir;
	bool shooting = false;
	float shotTime = 0;

	ExampleLayer() : Layer("Example")
	{
		renderer = &GameEngine::Renderer::Get();
		application = &GameEngine::Application::Get();
		camera = &application->GetCamera();

		GameEngine::ImportShaders(renderer);

		playerRotateAngle = 0;
		playerMoveDirection = glm::vec3(0.5f, 0, -1);

		projectilePosition = glm::vec3(0, 0, 0);
		shootingDir = glm::vec3(0);

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
				2, 3, 1
			};

			std::vector<glm::vec3> normals;
			normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
			normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
			renderer->meshes["quad"] = renderer->CreateMesh("quad", vertices, indices, normals);
		}

		renderer->meshes["box"] = renderer->LoadMesh("box", "D:\\Facultate\\PPBG\\gfx-framework-master\\assets\\models\\primitives\\box.obj");
		renderer->meshes["sphere"] = renderer->LoadMesh("sphere", "D:\\Facultate\\PPBG\\gfx-framework-master\\assets\\models\\primitives\\sphere.obj");
	}

	void OnUpdate(float deltaTime) override
	{
		//plane
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 1.0f, 10.0f));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(1, 0, 0));
			renderer->RenderMesh(renderer->meshes["quad"], renderer->shaders["BasicShader"], modelMatrix, glm::vec3(0.51f, 0.51f, 2.04f));
		}

		//player
		{
			head = glm::mat4(1);
			head = glm::translate(head, glm::vec3(playerMoveDirection.x, playerMoveDirection.y, playerMoveDirection.z));
			head = glm::rotate(head, playerRotateAngle, glm::vec3(0, 1, 0));
			head = glm::translate(head, glm::vec3(-playerMoveDirection.x, -playerMoveDirection.y, -playerMoveDirection.z));
			head = glm::translate(head, glm::vec3(0, 1.1f, 0) + playerMoveDirection);
			head = glm::scale(head, glm::vec3(0.25f, 0.15f, 0.15f));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], head, glm::vec3(0.5f, 0.2f, 0.45f));

			body = glm::mat4(1);
			body = glm::translate(body, glm::vec3(0, 0.77f, 0) + playerMoveDirection);
			body = glm::scale(body, glm::vec3(0.40f, 0.40f, 0.40f));
			body = glm::rotate(body, playerRotateAngle, glm::vec3(0, 1, 0));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], body, glm::vec3(0.2f, 0.7f, 0.9f));

			leftHand = glm::mat4(1);
			leftHand = glm::translate(leftHand, glm::vec3(playerMoveDirection.x, playerMoveDirection.y, playerMoveDirection.z));
			leftHand = glm::rotate(leftHand, playerRotateAngle, glm::vec3(0, 1, 0));
			leftHand = glm::translate(leftHand, glm::vec3(-playerMoveDirection.x, -playerMoveDirection.y, -playerMoveDirection.z));
			leftHand = glm::translate(leftHand, glm::vec3(-0.28f, 0.77f, 0) + playerMoveDirection);
			leftHand = glm::scale(leftHand, glm::vec3(0.125f, 0.55f, 0.25f));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], leftHand, glm::vec3(0.2f, 0.7f, 0.9f));

			rightHand = glm::mat4(1);
			rightHand = glm::translate(rightHand, glm::vec3(playerMoveDirection.x, playerMoveDirection.y, playerMoveDirection.z));
			rightHand = glm::rotate(rightHand, playerRotateAngle, glm::vec3(0, 1, 0));
			rightHand = glm::translate(rightHand, glm::vec3(-playerMoveDirection.x, -playerMoveDirection.y, -playerMoveDirection.z));
			rightHand = glm::translate(rightHand, glm::vec3(0.28f, 0.77f, 0) + playerMoveDirection);
			rightHand = glm::scale(rightHand, glm::vec3(0.125f, 0.55f, 0.25f));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], rightHand, glm::vec3(0.2f, 0.7f, 0.9f));

			leftLeg = glm::mat4(1);
			leftLeg = glm::translate(leftLeg, glm::vec3(playerMoveDirection.x, playerMoveDirection.y, playerMoveDirection.z));
			leftLeg = glm::rotate(leftLeg, playerRotateAngle, glm::vec3(0, 1, 0));
			leftLeg = glm::translate(leftLeg, glm::vec3(-playerMoveDirection.x, -playerMoveDirection.y, -playerMoveDirection.z));
			leftLeg = glm::translate(leftLeg, glm::vec3(-0.1f, 0.25f, 0) + playerMoveDirection);
			leftLeg = glm::scale(leftLeg, glm::vec3(0.125f, 0.5f, 0.25f));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], leftLeg, glm::vec3(0.5f, 0.2f, 0.45f));

			rightLeg = glm::mat4(1);
			rightLeg = glm::translate(rightLeg, glm::vec3(playerMoveDirection.x, playerMoveDirection.y, playerMoveDirection.z));
			rightLeg = glm::rotate(rightLeg, playerRotateAngle, glm::vec3(0, 1, 0));
			rightLeg = glm::translate(rightLeg, glm::vec3(-playerMoveDirection.x, -playerMoveDirection.y, -playerMoveDirection.z));
			rightLeg = glm::translate(rightLeg, glm::vec3(0.1f, 0.25f, 0) + playerMoveDirection);
			rightLeg = glm::scale(rightLeg, glm::vec3(0.125f, 0.5f, 0.25f));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], rightLeg, glm::vec3(0.5f, 0.2f, 0.45f));
		}

		//enemies
		EnemiesMovement(deltaTime);
		{
			enemy = glm::mat4(1);
			enemy = glm::translate(enemy, glm::vec3(4.0f, 0.77f, 0 + enemyMovingCoordinates));
			enemy = glm::scale(enemy, glm::vec3(1.0f, 2.0f, 1.0f));
			renderer->RenderMesh(renderer->meshes["sphere"], renderer->shaders["BasicShader"], enemy, glm::vec3(0.45f, 0.29f, 0.9f));
		}

		//projectile
		{
			if (shooting)
			{
				projectilePosition += shootingDir * deltaTime * 4.0f;

				glm::mat4 projectile = glm::mat4(1);
				projectile = glm::translate(projectile, glm::vec3(projectilePosition.x, 0.75f, projectilePosition.z));
				projectile = glm::scale(projectile, glm::vec3(0.25f, 0.25f, 0.25f));
				renderer->RenderMesh(renderer->meshes["sphere"], renderer->shaders["BasicShader"], projectile);
			}

			shotTime -= deltaTime;
			if (shotTime <= 0)
				shooting = false;
		}
	}

	void OnInputUpdate(float deltaTime, float deltaX, float deltaY) override
	{
		//player movement
		float playerSpeed = 2.0f;
		float playerRotateFactor = 4.0f;
		if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_W))
		{
			camera->MoveForward(camera->cameraSpeed * deltaTime);
			glm::vec3 moveFactor = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z)) * playerSpeed * deltaTime;
			moveFactor.y = 0;
			playerMoveDirection += moveFactor;
		}
		if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_S))
		{
			camera->MoveBackward(camera->cameraSpeed * deltaTime);
			glm::vec3 moveFactor = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z)) * playerSpeed * deltaTime;
			moveFactor.y = 0;
			playerMoveDirection -= moveFactor;
		}
		if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_A))
		{
			camera->MoveLeft(camera->cameraSpeed * deltaTime);
			glm::vec3 moveFactor = glm::normalize(glm::vec3(camera->right.x, 0, camera->right.z)) * playerSpeed * deltaTime;
			moveFactor.y = 0;
			playerMoveDirection -= moveFactor;
		}
		if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_D))
		{
			camera->MoveRight(camera->cameraSpeed * deltaTime);
			glm::vec3 moveFactor = glm::normalize(glm::vec3(camera->right.x, 0, camera->right.z)) * playerSpeed * deltaTime;
			moveFactor.y = 0;
			playerMoveDirection += moveFactor;
		}
		if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_Q))
		{
			playerRotateAngle += playerRotateFactor * deltaTime;
			camera->RotateFirstPerson_OX(playerRotateFactor * deltaTime);
		}
		if (GameEngine::InputInterface::IsKeyPressed(GLFW_KEY_E))
		{
			playerRotateAngle -= playerRotateFactor * deltaTime;
			camera->RotateFirstPerson_OX(-playerRotateFactor * deltaTime);
		}

		if (GameEngine::InputInterface::IsMouseBtnPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			shooting = true;
			shotTime = 2.0f;
			projectilePosition = playerMoveDirection;
			shootingDir = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));
		}
	}

	void OnEvent(GameEngine::Event& event) override
	{
		if (event.GetEventType() == GameEngine::EventType::MouseButtonPressed)
		{
			GameEngine::MouseButtonPressedEvent& e = (GameEngine::MouseButtonPressedEvent&)event;
			if (e.GetMouseButton() == 1)
			{
				camera->Set(glm::vec3(playerMoveDirection.x, 1.25f, playerMoveDirection.z),
					glm::vec3(4.0f,
						1.0f,
						0.0f + enemyMovingCoordinates),
					glm::vec3(0, 1, 0));

				glm::vec3 forward = glm::normalize(glm::vec3(playerMoveDirection.x, 1.25f, playerMoveDirection.z) - glm::vec3(4.0f,
					1.0f,
					0.0f + enemyMovingCoordinates));

				playerRotateAngle = glm::acos(glm::dot(forward, glm::vec3(1, 0, 0)));
			}
		}
	}

	void EnemiesMovement(float deltaTime)
	{
		if (enemiesGoingForward)
		{
			enemyMovingCoordinates += 0.5f * deltaTime;
			if (enemyMovingCoordinates >= 2.0f)
			{
				enemiesGoingForward = false;
			}
		}
		else
		{
			enemyMovingCoordinates -= 0.5f * deltaTime;
			if (enemyMovingCoordinates <= 0.2f)
			{
				enemiesGoingForward = true;
			}
		}
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