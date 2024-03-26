#pragma once

#include "Mesh.h"
#include "Shader.h"
#include <functional>

namespace GameEngine
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		Mesh* CreateMesh(std::vector<float> vertices, std::vector<unsigned int> indices);
		void RenderMesh(Mesh *mesh, Shader* shader);

		std::unordered_map<std::string, Mesh*> meshes;
	};
}