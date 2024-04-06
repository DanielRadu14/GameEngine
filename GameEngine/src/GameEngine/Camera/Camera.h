#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include "glm/gtc/type_ptr.hpp"

#define RADIANS(angle)      ((angle) * 0.0174532925194444f)     // a * M_PI / 180

namespace GameEngine
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up);
		void MoveForward(float distance);
		void MoveBackward(float distance);
		void MoveRight(float distance);
		void MoveLeft(float distance);
		void MoveUp(float distance);
		void MoveDown(float distance);
		void RotateFirstPerson_OX(float angle);
		void RotateFirstPerson_OY(float angle);
		glm::mat4 GetViewMatrix();

	public:
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;

		float cameraSpeed = 2.0f;
		float sensivityOX = 0.005f;
		float sensivityOY = 0.005f;
	};
}