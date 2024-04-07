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

	GameEngine::GameObject *enemy;
	float enemyMovingCoordinates = 0.0f;
	bool enemiesGoingForward = true;

	glm::vec3 projectilePosition;
	glm::vec3 shootingDir;
	bool shooting = false;
	float shotTime = 0;

	float timeToExplosion;
	bool collision;

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

		timeToExplosion = 5.0f;
		collision = false;

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
	
		std::string vertexCode = R"(
			#version 330

			// Input
			layout(location = 0) in vec3 v_position;
			layout(location = 1) in vec3 v_normal;

			// Uniform properties
			uniform mat4 Model;
			uniform mat4 View;
			uniform mat4 Projection;

			uniform vec3 object_color;

			// Output
			out vec3 frag_position;
			out vec3 frag_normal;
			out vec2 frag_texture;
			out vec3 frag_color;

			vec3 mod289(vec3 x)
			{
				return x - floor(x * (1.0 / 289.0)) * 289.0;
			}

			vec4 mod289(vec4 x)
			{
				return x - floor(x * (1.0 / 289.0)) * 289.0;
			}

			vec4 permute(vec4 x)
			{
				return mod289(((x*34.0) + 10.0)*x);
			}

			vec4 taylorInvSqrt(vec4 r)
			{
				return 1.79284291400159 - 0.85373472095314 * r;
			}

			vec3 fade(vec3 t) {
				return t * t*t*(t*(t*6.0 - 15.0) + 10.0);
			}

			// Classic Perlin noise
			float cnoise(vec3 P)
			{
				vec3 Pi0 = floor(P); // Integer part for indexing
				vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
				Pi0 = mod289(Pi0);
				Pi1 = mod289(Pi1);
				vec3 Pf0 = fract(P); // Fractional part for interpolation
				vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
				vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
				vec4 iy = vec4(Pi0.yy, Pi1.yy);
				vec4 iz0 = Pi0.zzzz;
				vec4 iz1 = Pi1.zzzz;

				vec4 ixy = permute(permute(ix) + iy);
				vec4 ixy0 = permute(ixy + iz0);
				vec4 ixy1 = permute(ixy + iz1);

				vec4 gx0 = ixy0 * (1.0 / 7.0);
				vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
				gx0 = fract(gx0);
				vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
				vec4 sz0 = step(gz0, vec4(0.0));
				gx0 -= sz0 * (step(0.0, gx0) - 0.5);
				gy0 -= sz0 * (step(0.0, gy0) - 0.5);

				vec4 gx1 = ixy1 * (1.0 / 7.0);
				vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
				gx1 = fract(gx1);
				vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
				vec4 sz1 = step(gz1, vec4(0.0));
				gx1 -= sz1 * (step(0.0, gx1) - 0.5);
				gy1 -= sz1 * (step(0.0, gy1) - 0.5);

				vec3 g000 = vec3(gx0.x, gy0.x, gz0.x);
				vec3 g100 = vec3(gx0.y, gy0.y, gz0.y);
				vec3 g010 = vec3(gx0.z, gy0.z, gz0.z);
				vec3 g110 = vec3(gx0.w, gy0.w, gz0.w);
				vec3 g001 = vec3(gx1.x, gy1.x, gz1.x);
				vec3 g101 = vec3(gx1.y, gy1.y, gz1.y);
				vec3 g011 = vec3(gx1.z, gy1.z, gz1.z);
				vec3 g111 = vec3(gx1.w, gy1.w, gz1.w);

				vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
				g000 *= norm0.x;
				g010 *= norm0.y;
				g100 *= norm0.z;
				g110 *= norm0.w;
				vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
				g001 *= norm1.x;
				g011 *= norm1.y;
				g101 *= norm1.z;
				g111 *= norm1.w;

				float n000 = dot(g000, Pf0);
				float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
				float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
				float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
				float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
				float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
				float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
				float n111 = dot(g111, Pf1);

				vec3 fade_xyz = fade(Pf0);
				vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
				vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
				float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
				return 2.2 * n_xyz;
			}

			// Classic Perlin noise, periodic variant
			float pnoise(vec3 P, vec3 rep)
			{
				vec3 Pi0 = mod(floor(P), rep); // Integer part, modulo period
				vec3 Pi1 = mod(Pi0 + vec3(1.0), rep); // Integer part + 1, mod period
				Pi0 = mod289(Pi0);
				Pi1 = mod289(Pi1);
				vec3 Pf0 = fract(P); // Fractional part for interpolation
				vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
				vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
				vec4 iy = vec4(Pi0.yy, Pi1.yy);
				vec4 iz0 = Pi0.zzzz;
				vec4 iz1 = Pi1.zzzz;

				vec4 ixy = permute(permute(ix) + iy);
				vec4 ixy0 = permute(ixy + iz0);
				vec4 ixy1 = permute(ixy + iz1);

				vec4 gx0 = ixy0 * (1.0 / 7.0);
				vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
				gx0 = fract(gx0);
				vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
				vec4 sz0 = step(gz0, vec4(0.0));
				gx0 -= sz0 * (step(0.0, gx0) - 0.5);
				gy0 -= sz0 * (step(0.0, gy0) - 0.5);

				vec4 gx1 = ixy1 * (1.0 / 7.0);
				vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
				gx1 = fract(gx1);
				vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
				vec4 sz1 = step(gz1, vec4(0.0));
				gx1 -= sz1 * (step(0.0, gx1) - 0.5);
				gy1 -= sz1 * (step(0.0, gy1) - 0.5);

				vec3 g000 = vec3(gx0.x, gy0.x, gz0.x);
				vec3 g100 = vec3(gx0.y, gy0.y, gz0.y);
				vec3 g010 = vec3(gx0.z, gy0.z, gz0.z);
				vec3 g110 = vec3(gx0.w, gy0.w, gz0.w);
				vec3 g001 = vec3(gx1.x, gy1.x, gz1.x);
				vec3 g101 = vec3(gx1.y, gy1.y, gz1.y);
				vec3 g011 = vec3(gx1.z, gy1.z, gz1.z);
				vec3 g111 = vec3(gx1.w, gy1.w, gz1.w);

				vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
				g000 *= norm0.x;
				g010 *= norm0.y;
				g100 *= norm0.z;
				g110 *= norm0.w;
				vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
				g001 *= norm1.x;
				g011 *= norm1.y;
				g101 *= norm1.z;
				g111 *= norm1.w;

				float n000 = dot(g000, Pf0);
				float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
				float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
				float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
				float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
				float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
				float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
				float n111 = dot(g111, Pf1);

				vec3 fade_xyz = fade(Pf0);
				vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
				vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
				float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
				return 2.2 * n_xyz;
			}

			varying float noise;
			uniform float time;

			float turbulence(vec3 p) {

				float w = 100.0;
				float t = -.5;

				for (float f = 1.0; f <= 10.0; f++) {
					float power = pow(2.0, f);
					t += abs(pnoise(vec3(power * p), vec3(10.0, 10.0, 10.0)) / power);
				}

				return t;

			}

			void main()
			{
				frag_position = v_position;
				frag_normal = v_normal;
				frag_color = object_color;

				// add time to the noise parameters so it's animated
				noise = 10.0 *  -.10 * turbulence(.5 * v_normal + time);
				float b = 5.0 * pnoise(0.05 * v_position + vec3(2.0 * time), vec3(100.0));
				float displacement = -noise + b;

				vec3 newPosition = v_position + v_normal * displacement;

				gl_Position = Projection * View * Model * vec4(newPosition, 1.0);
			}
		)";

		std::string fragmentCode = R"(
			#version 330
			in vec3 frag_position;
			in vec3 frag_normal;
			in vec3 frag_color;

			layout(location = 0) out vec4 out_color;

			varying float noise;
			uniform sampler2D tExplosion;

			float random(vec3 scale, float seed) {
				return fract(sin(dot(gl_FragCoord.xyz + seed, scale)) * 43758.5453 + seed);
			}

			void main()
			{
				float r = .01 * random(vec3(12.9898, 78.233, 151.7182), 0.0);
				vec2 tPos = vec2(0, 1.3 * noise + r);
				vec4 color = texture2D(tExplosion, tPos);

				out_color = vec4(frag_color, 1);
			}
		)";

		renderer->shaders["ExplodingShader"] = new GameEngine::Shader(vertexCode, fragmentCode);
	}

	void OnUpdate(float deltaTime) override
	{
		if(collision)
			timeToExplosion -= deltaTime;

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
			GameEngine::GameObject *head = new GameEngine::GameObject();
			head->Translate(glm::vec3(playerMoveDirection.x, playerMoveDirection.y, playerMoveDirection.z));
			head->Rotate(glm::vec4(0, 1, 0, playerRotateAngle));
			head->Translate(glm::vec3(-playerMoveDirection.x, -playerMoveDirection.y, -playerMoveDirection.z));
			head->Translate(glm::vec3(0, 1.1f, 0) + playerMoveDirection);
			head->Scale(glm::vec3(0.25f, 0.15f, 0.15f));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], head->GetModelMatrix(), glm::vec3(0.5f, 0.2f, 0.45f));

			GameEngine::GameObject *body = new GameEngine::GameObject();
			body->Translate(glm::vec3(0, 0.77f, 0) + playerMoveDirection);
			body->Scale(glm::vec3(0.40f, 0.40f, 0.40f));
			body->Rotate(glm::vec4(0, 1, 0, playerRotateAngle));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], body->GetModelMatrix(), glm::vec3(0.2f, 0.7f, 0.9f));

			GameEngine::GameObject *leftHand = new GameEngine::GameObject();
			leftHand->Translate(glm::vec3(playerMoveDirection.x, playerMoveDirection.y, playerMoveDirection.z));
			leftHand->Rotate(glm::vec4(0, 1, 0, playerRotateAngle));
			leftHand->Translate(glm::vec3(-playerMoveDirection.x, -playerMoveDirection.y, -playerMoveDirection.z));
			leftHand->Translate(glm::vec3(-0.28f, 0.77f, 0) + playerMoveDirection);
			leftHand->Scale(glm::vec3(0.125f, 0.55f, 0.25f));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], leftHand->GetModelMatrix(), glm::vec3(0.2f, 0.7f, 0.9f));

			GameEngine::GameObject *rightHand = new GameEngine::GameObject();
			rightHand->Translate(glm::vec3(playerMoveDirection.x, playerMoveDirection.y, playerMoveDirection.z));
			rightHand->Rotate(glm::vec4(0, 1, 0, playerRotateAngle));
			rightHand->Translate(glm::vec3(-playerMoveDirection.x, -playerMoveDirection.y, -playerMoveDirection.z));
			rightHand->Translate(glm::vec3(0.28f, 0.77f, 0) + playerMoveDirection);
			rightHand->Scale(glm::vec3(0.125f, 0.55f, 0.25f));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], rightHand->GetModelMatrix(), glm::vec3(0.2f, 0.7f, 0.9f));

			GameEngine::GameObject *leftLeg = new GameEngine::GameObject();
			leftLeg->Translate(glm::vec3(playerMoveDirection.x, playerMoveDirection.y, playerMoveDirection.z));
			leftLeg->Rotate(glm::vec4(0, 1, 0, playerRotateAngle));
			leftLeg->Translate(glm::vec3(-playerMoveDirection.x, -playerMoveDirection.y, -playerMoveDirection.z));
			leftLeg->Translate(glm::vec3(-0.1f, 0.25f, 0) + playerMoveDirection);
			leftLeg->Scale(glm::vec3(0.125f, 0.5f, 0.25f));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], leftLeg->GetModelMatrix(), glm::vec3(0.5f, 0.2f, 0.45f));

			GameEngine::GameObject *rightLeg = new GameEngine::GameObject();
			rightLeg->Translate(glm::vec3(playerMoveDirection.x, playerMoveDirection.y, playerMoveDirection.z));
			rightLeg->Rotate(glm::vec4(0, 1, 0, playerRotateAngle));
			rightLeg->Translate(glm::vec3(-playerMoveDirection.x, -playerMoveDirection.y, -playerMoveDirection.z));
			rightLeg->Translate(glm::vec3(0.1f, 0.25f, 0) + playerMoveDirection);
			rightLeg->Scale(glm::vec3(0.125f, 0.5f, 0.25f));
			renderer->RenderMesh(renderer->meshes["box"], renderer->shaders["IlluminatedShader"], rightLeg->GetModelMatrix(), glm::vec3(0.5f, 0.2f, 0.45f));
		}

		//enemies
		EnemiesMovement(deltaTime);
		{
			enemy = new GameEngine::GameObject();
			enemy->Translate(glm::vec3(4.0f, 0.77f, 0 + enemyMovingCoordinates));
			enemy->Scale(glm::vec3(1.0f, 2.0f, 1.0f));
			if (timeToExplosion > 0 && collision)
				renderer->RenderMesh(renderer->meshes["sphere"], renderer->shaders["ExplodingShader"], enemy->GetModelMatrix(), timeToExplosion, glm::vec3(0.45f, 0.29f, 0.9f));
			else if (timeToExplosion > 0 && !collision)
				renderer->RenderMesh(renderer->meshes["sphere"], renderer->shaders["BasicShader"], enemy->GetModelMatrix(), glm::vec3(0.45f, 0.29f, 0.9f));
		}

		//projectile
		{
			if (shooting)
			{
				projectilePosition += shootingDir * deltaTime * 4.0f;

				GameEngine::GameObject *projectile = new GameEngine::GameObject();
				projectile->Translate(glm::vec3(projectilePosition.x, 0.75f, projectilePosition.z));
				projectile->Scale(glm::vec3(0.25f, 0.25f, 0.25f));
				renderer->RenderMesh(renderer->meshes["sphere"], renderer->shaders["BasicShader"], projectile->GetModelMatrix());

				if (projectile->CheckCollision(enemy) == true)
					collision = true;
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