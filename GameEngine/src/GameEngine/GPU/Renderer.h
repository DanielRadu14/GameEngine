#pragma once

#include "Mesh.h"
#include "Shader.h"
#include <functional>
#include <glm/mat4x4.hpp> // glm::mat4
#include "GameEngine/Application.h"

namespace GameEngine
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		Mesh* CreateMesh(std::vector<float> vertices, std::vector<unsigned int> indices);
		void RenderMesh(Mesh *mesh, Shader* shader, const glm::mat4 &modelMatrix);

		std::unordered_map<std::string, Mesh*> meshes;
		std::unordered_map<std::string, Shader*> shaders;
	};
}