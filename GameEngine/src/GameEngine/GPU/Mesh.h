#pragma once

#include "string"
#include "vector"
#include "glm/glm.hpp"

namespace GameEngine 
{
	class Mesh
	{
	public:
		Mesh(std::string name, std::vector<float> vertices,
			std::vector<unsigned int> indices, unsigned int VAO);
		Mesh(std::string name, std::vector<float> vertices, 
			std::vector<unsigned int> indices, std::vector<glm::vec3> normals, unsigned int VAO);
		virtual ~Mesh();

		unsigned int GetVertexArray();

	public:
		std::string name;

		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::vector<glm::vec3> normals;

	private:
		unsigned int VAO = 0;
	};
}