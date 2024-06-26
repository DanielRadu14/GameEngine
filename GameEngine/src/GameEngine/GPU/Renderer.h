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

		Mesh* LoadMesh(std::string name, std::string filePath);
		Mesh* CreateMesh(std::string name, std::vector<float> vertices, std::vector<unsigned int> indices);
		Mesh* CreateMesh(std::string name, std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<glm::vec3> normals);
		void RenderMesh(Mesh *mesh, Shader* shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));
		void RenderMesh(Mesh *mesh, Shader* shader, const glm::mat4 &modelMatrix, float deltaTime, const glm::vec3 &color = glm::vec3(1));

		inline static Renderer& Get() { return *s_Instance; }

	public:
		std::unordered_map<std::string, Mesh*> meshes;
		std::unordered_map<std::string, Shader*> shaders;

		unsigned int materialShininess = 30;
		float materialKd = 0.5;
		float materialKs = 0.5;

	private:
		static Renderer* s_Instance;
	};
}