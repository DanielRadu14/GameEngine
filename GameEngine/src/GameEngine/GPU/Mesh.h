#pragma once

#include "string"
#include "vector"

namespace GameEngine 
{
	class Mesh
	{
	public:
		Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, unsigned int VAO);
		virtual ~Mesh();

		unsigned int GetVertexArray();
		std::vector<unsigned int> indices;
	private:
		unsigned int VAO = 0;

		std::vector<float> vertices;
		
	};
}