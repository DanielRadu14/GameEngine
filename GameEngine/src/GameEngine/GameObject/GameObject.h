#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include "glm/gtc/type_ptr.hpp"

namespace GameEngine
{
	class GameObject
	{
	public:
		GameObject();
		~GameObject();

		glm::mat4 GetModelMatrix();
		glm::vec3 GetPosition();
		glm::vec4 GetRotation();
		glm::vec3 GetScale();

		void Translate(glm::vec3 position);
		void Rotate(glm::vec4 rotation);
		void Scale(glm::vec3 scale);

		bool CheckCollision(GameObject *gameObject);

	protected:
		glm::mat4 modelMatrix;
		glm::vec3 position;
		glm::vec4 rotation;
		glm::vec3 scale;
	};
}