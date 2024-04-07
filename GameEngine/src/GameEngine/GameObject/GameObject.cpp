#include "GameObject.h"
#include <iostream>

namespace GameEngine
{
	GameObject::GameObject()
	{
		modelMatrix = glm::mat4(1);
	}

	GameObject::~GameObject() {}

	glm::mat4 GameObject::GetModelMatrix()
	{
		return modelMatrix;
	}

	glm::vec3 GameObject::GetPosition()
	{
		return position;
	}

	glm::vec4 GameObject::GetRotation()
	{
		return rotation;
	}

	glm::vec3 GameObject::GetScale()
	{
		return scale;
	}

	void GameObject::Translate(glm::vec3 position)
	{
		this->position = position;
		modelMatrix = glm::translate(modelMatrix, position);
	}
	void GameObject::Rotate(glm::vec4 rotation)
	{
		this->rotation = rotation;
		modelMatrix = glm::rotate(modelMatrix, rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z));
	}

	void GameObject::Scale(glm::vec3 scale)
	{
		this->scale = scale;
		modelMatrix = glm::scale(modelMatrix, scale);
	}

	bool GameObject::CheckCollision(GameObject *gameObject)
	{
		bool collisionX = this->position.x + this->scale.x >= gameObject->GetPosition().x &&
			gameObject->GetPosition().x + gameObject->GetScale().x >= this->position.x;

		bool collisionY = this->position.y + this->scale.y >= gameObject->GetPosition().y &&
			gameObject->GetPosition().y + gameObject->GetScale().y >= this->position.y;

		bool collisionZ = this->position.z + this->scale.z >= gameObject->GetPosition().z &&
			gameObject->GetPosition().z + gameObject->GetScale().z >= this->position.z;

		return collisionX && collisionY && collisionZ;
	}
}