#include "Mesh.h"
#include "glad/glad.h"

namespace GameEngine
{
	Mesh::Mesh(std::string name, std::vector<float> vertices, std::vector<unsigned int> indices, unsigned int VAO)
	{
		this->name = name;
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