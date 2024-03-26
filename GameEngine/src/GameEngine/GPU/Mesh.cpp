#include "Mesh.h"
#include "glad/glad.h"

namespace GameEngine
{
	Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, unsigned int VAO)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->VAO = VAO;
	}

	Mesh::~Mesh()
	{

	}

	unsigned int Mesh::GetVertexArray()
	{
		return VAO;
	}
}