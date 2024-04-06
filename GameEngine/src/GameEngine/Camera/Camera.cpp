#include "Camera.h"

namespace GameEngine
{
	Camera::Camera()
	{
	}

	Camera::~Camera() { }

	void Camera::Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
	{
		this->position = position;
		forward = glm::normalize(center - position);
		right = glm::cross(forward, up);
		this->up = glm::cross(right, forward);
	}

	void Camera::MoveForward(float distance)
	{
		glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
		position += dir * distance;
	}

	void Camera::MoveBackward(float distance)
	{
		glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
		position -= dir * distance;
	}

	void Camera::MoveRight(float distance)
	{
		glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
		position += dir * distance;
	}

	void Camera::MoveLeft(float distance)
	{
		glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
		position -= dir * distance;
	}

	void Camera::MoveUp(float distance)
	{
		glm::vec3 dir = glm::normalize(glm::vec3(0, 1, 0));
		position += dir * distance;
	}

	void Camera::MoveDown(float distance)
	{
		glm::vec3 dir = glm::normalize(glm::vec3(0, 1, 0));
		position -= dir * distance;
	}

	void Camera::RotateFirstPerson_OX(float angle)
	{
		glm::vec4 newForward = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0))
			* glm::vec4(forward, 1);
		forward = glm::normalize(glm::vec3(newForward));

		glm::vec4 newRight = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0))
			* glm::vec4(right, 1);
		right = glm::normalize(glm::vec3(newRight));
		up = glm::cross(right, forward);
	}

	void Camera::RotateFirstPerson_OY(float angle)
	{
		glm::vec4 newForward = glm::rotate(glm::mat4(1.0f), angle, right)
			* glm::vec4(forward, 1);
		forward = glm::normalize(glm::vec3(newForward));
		up = glm::cross(right, forward);
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(position, position + forward, up);
	}
}