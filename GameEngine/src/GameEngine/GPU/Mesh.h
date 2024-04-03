#pragma once

#include "string"
#include "vector"

namespace GameEngine 
{
	class Mesh
	{
	public:
		Mesh(std::string name, std::vector<float> vertices, std::vector<unsigned int> indices, unsigned int VAO);
		virtual ~Mesh();

		unsigned int GetVertexArray();
		std::vector<unsigned int> indices;
		std::vector<float> vertices;
		std::string name;
	private:
		unsigned int VAO = 0;
	};
}