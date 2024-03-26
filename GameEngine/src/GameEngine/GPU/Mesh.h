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

		unsigned int getVertexArray();
	private:
		std::string name;
		unsigned int VAO = 0;

		std::vector<float> vertices;
		std::vector<unsigned int> indices;
	};
}