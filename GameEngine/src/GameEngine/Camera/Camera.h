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
		Camera()
		{
			position = glm::vec3(0, 2, 5);
			forward = glm::vec3(0, 0, -1);
			up = glm::vec3(0, 1, 0);
			right = glm::vec3(1, 0, 0);
			distanceToTarget = 2;
		}

		Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			Set(position, center, up);
		}

		~Camera()
		{ }

		void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			this->position = position;
			forward = glm::normalize(center - position);
			right = glm::cross(forward, up);
			this->up = glm::cross(right, forward);
		}

		void MoveForward(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
			position += dir * distance;
		}

		void MoveBackward(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
			position -= dir * distance;
		}

		void MoveRight(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
			position += dir * distance;
		}

		void MoveLeft(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
			position -= dir * distance;
		}

		void MoveUp(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(0, 1, 0));
			position += dir * distance;
		}

		void MoveDown(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(0, 1, 0));
			position -= dir * distance;
		}

		void TranslateForward(float distance)
		{
			position += distance * glm::normalize(forward);

		}

		void TranslateUpward(float distance)
		{
			position += distance * glm::normalize(up);

		}

		void TranslateRight(float distance)
		{
			position += distance * glm::normalize(right);

		}

		void RotateFirstPerson_OX(float angle)
		{
			glm::vec4 newForward = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0))
				* glm::vec4(forward, 1);
			forward = glm::normalize(glm::vec3(newForward));

			glm::vec4 newRight = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0))
				* glm::vec4(right, 1);
			right = glm::normalize(glm::vec3(newRight));
			up = glm::cross(right, forward);
		}

		void RotateFirstPerson_OY(float angle)
		{
			glm::vec4 newForward = glm::rotate(glm::mat4(1.0f), angle, right)
				* glm::vec4(forward, 1);
			forward = glm::normalize(glm::vec3(newForward));
			up = glm::cross(right, forward);
		}

		void RotateFirstPerson_OZ(float angle)
		{
			glm::vec4 newRight = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0))
				* glm::vec4(right, 1);
			right = glm::normalize(glm::vec3(newRight));

			glm::vec4 newUp = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0))
				* glm::vec4(up, 1);
			up = glm::normalize(glm::vec3(newUp));

		}

		void RotateThirdPerson_OX(float angle)
		{
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OX(angle);
			TranslateForward(-distanceToTarget);
		}

		void RotateThirdPerson_OY(float angle)
		{
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OY(angle);
			TranslateForward(-distanceToTarget);

		}

		void RotateThirdPerson_OZ(float angle)
		{
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OZ(angle);
			TranslateForward(-distanceToTarget);

		}

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(position, position + forward, up);
		}

		glm::vec3 GetTargetPosition()
		{
			return position + forward * distanceToTarget;
		}

	public:
		float distanceToTarget;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;

		float cameraSpeed = 2.0f;
		float sensivityOX = 0.005f;
		float sensivityOY = 0.005f;
	};
}